#include "GamePage.h"
#include "Constants.h"
#include <QKeyEvent>
#include <QDebug>
#include <QPixmap>
#include <QMenu>
#include <QApplication>
#include "Font.h"
#include "Note.h"
#include "GameConfig.h"
#include <QGraphicsBlurEffect>

GamePage::GamePage(QWidget* parent)
    : QWidget(parent), m_updateTimer(new QTimer(this)), m_player(new QMediaPlayer(this)), m_gameConfig(GameConfig::instance()) {
    setFocusPolicy(Qt::StrongFocus);
    m_updateTimer->setInterval(16); // Approximately 60 FPS
    connect(m_updateTimer, &QTimer::timeout, this, &GamePage::updateDisplay);
    m_updateTimer->start();

    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setPixmap(QPixmap(":/images/game_background.png"));
    m_backgroundLabel->setScaledContents(true);
    m_backgroundLabel->resize(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);

    m_pauseButton = new QPushButton(this);
    m_pauseButton->setText("⏸");
    m_pauseButton->setGeometry(GameConstants::WINDOW_WIDTH - 75, 30, 40, 40);
    m_pauseButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgb(0, 174, 239);
            color: white;
            border: none;
            border-radius: 6px; 
            font-size: 36px;
        }
    )");
    connect(m_pauseButton, &QPushButton::clicked, this, [this]() {
        emit pauseGame(); 
        this->onPauseButtonClicked(); 
    });

    // Connect audio player
    connect(m_player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState) {
            qDebug() << "Music playback finished.";
            m_updateTimer->stop(); // Stop the update timer
            emit finishGame(); // Game end signal
        }
    });

    m_pauseOverlay = new QWidget(this);
    m_pauseOverlay->setGeometry(0, 0, GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    m_pauseOverlay->hide();

    m_pauseLabel = new QLabel(m_pauseOverlay);
    m_pauseLabel->setGeometry(0, 0, GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    m_pauseLabel->setScaledContents(true);

    m_pauseMaskLabel = new QLabel(m_pauseOverlay);
    m_pauseMaskLabel->setGeometry(0, 0, GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    m_pauseMaskLabel->setStyleSheet("background: rgba(0,0,0,100);");

    // three buttons in middle of the pause overlay,in one row
    m_pauseBackButton = new QPushButton(m_pauseOverlay);
    m_pauseBackButton->setGeometry(GameConstants::WINDOW_WIDTH / 2 - 300, GameConstants::WINDOW_HEIGHT / 2 - 50, 100, 100);
    m_pauseBackButton->setStyleSheet("background-color: transparent; border: none;");
    m_pauseBackButton->setIcon(QIcon(":/images/back_icon.png"));
    m_pauseBackButton->setIconSize(QSize(100, 100));
    m_pauseQuitButton = new QPushButton(m_pauseOverlay);
    m_pauseQuitButton->setGeometry(GameConstants::WINDOW_WIDTH / 2 - 50, GameConstants::WINDOW_HEIGHT / 2 - 50, 100, 100);
    m_pauseQuitButton->setStyleSheet("background-color: transparent; border: none;");
    m_pauseQuitButton->setIcon(QIcon(":/images/exit_icon.png"));
    m_pauseQuitButton->setIconSize(QSize(75, 75));
    m_pauseResumeButton = new QPushButton(m_pauseOverlay);
    m_pauseResumeButton->setGeometry(GameConstants::WINDOW_WIDTH / 2 + 200, GameConstants::WINDOW_HEIGHT / 2 - 50, 100, 100);
    m_pauseResumeButton->setStyleSheet("background-color: transparent; border: none;");
    m_pauseResumeButton->setIcon(QIcon(":/images/continue_icon.png"));
    m_pauseResumeButton->setIconSize(QSize(100, 100));

}

void GamePage::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0, 0, QPixmap(":/images/game_background.jpg"));

    drawTrackLines(painter);
    drawNotes(painter);
    drawJudgeLine(painter);
    drawScore(painter);
    drawHitEffects(painter);
    drawHitFeedBack(painter);
}

void GamePage::updateDisplay() {
    update();
    // Update game time
    m_currentGameTime += 0.016; // Update of 0.016 seconds per frame (60 FPS)
    emit timeUpdated(0.016);
}

void GamePage::onNoteHit(int track, int accuracy) {
    Q_UNUSED(track);
    Q_UNUSED(accuracy);
    qDebug() << "Note hit on track" << track << "with accuracy" << accuracy;
    // Hit effect animation
    QColor hitColor;
    if (accuracy == 0) {
        // Miss
        return;
    } else if (accuracy == 1) {
        // Good
        hitColor = Qt::white;
    } else if (accuracy == 2) {
        // Perfect
        hitColor = QColor(255, 215, 0); // Gold color
    }
    HitEffect effect(QPointF(GameConstants::TRACK_START_X + track * GameConstants::TRACK_WIDTH + GameConstants::TRACK_WIDTH / 2, 
                                             GameConstants::JUDGE_LINE_Y), hitColor);
    m_hitEffects.append(effect);
}

void GamePage::keyPressEvent(QKeyEvent* event) {
    int track = m_gameConfig->getTrackForKey(static_cast<Qt::Key>(event->key()));
    m_pressedTracks.append(track);
    emit keyPressed(m_gameConfig->getTrackForKey(static_cast<Qt::Key>(event->key())));
    QWidget::keyPressEvent(event);
}

void GamePage::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return; // Ignore automatic repeat events
    }
    int track = m_gameConfig->getTrackForKey(static_cast<Qt::Key>(event->key()));
    m_pressedTracks.removeAll(track);
    emit keyReleased(m_gameConfig->getTrackForKey(static_cast<Qt::Key>(event->key())));
    QWidget::keyReleaseEvent(event);
}

void GamePage::drawJudgeLine(QPainter& painter) {
    painter.setPen(QPen(Qt::white, 3));
    painter.drawLine(0, GameConstants::JUDGE_LINE_Y,
                     GameConstants::WINDOW_WIDTH, GameConstants::JUDGE_LINE_Y);
}

void GamePage::drawNotes(QPainter& painter) {
    painter.setPen(QPen(Qt::yellow, 2));
    painter.setBrush(QBrush(Qt::yellow));

    // Draw Note
    for (const Note* note : m_activeNotes) {
        if (note->isActive()) {
            // Calculate the display position of musical notes
            int x = GameConstants::TRACK_START_X + note->getTrack() * GameConstants::TRACK_WIDTH;
            int y = note->getY();
            int height = note->getHeight();
            if (note->getType() == Note::TAP) {
                // TAP note
                painter.drawPixmap(x + 5, y - height, GameConstants::TRACK_WIDTH - 10, height, QPixmap(":/images/tap_note.jpg"));
            } else if (note->getType() == Note::HOLD ) {
                // HOLD note
                int screenHeight = (y > GameConstants::JUDGE_LINE_Y) ? GameConstants::JUDGE_LINE_Y - y + height : height; 
                painter.drawPixmap(x + 5, y - height, GameConstants::TRACK_WIDTH - 10, screenHeight, QPixmap(":/images/hold_note_body.jpg"));
            }
        }
    }
}

void GamePage::drawTrackLines(QPainter& painter) {
    
    for (int i = 0; i <= 8; ++i) {
        int x = GameConstants::TRACK_START_X + i * GameConstants::TRACK_WIDTH;
        if (i == 4) {
            // Thicken the middle track line
            painter.setPen(QPen(Qt::gray, 5));
            painter.drawLine(x, 0, x, GameConstants::WINDOW_HEIGHT);
        } else {
            painter.setPen(QPen(Qt::gray, 1));
            painter.drawLine(x, 0, x, GameConstants::WINDOW_HEIGHT);
        }
    }

    //QColor shadowColor(255, 255, 255, 80);
    QColor shadowColor(0, 0, 0, 80);

    for (int i = 0; i < 8; ++i) {
        int x = GameConstants::TRACK_START_X + i * GameConstants::TRACK_WIDTH;
        QRect shadowRect(x, 0, GameConstants::TRACK_WIDTH, GameConstants::WINDOW_HEIGHT);
        painter.fillRect(shadowRect, shadowColor);
    }

}

void GamePage::drawScore(QPainter& painter) { 
    painter.setPen(QPen(Qt::white, 2));
    painter.setFont(Font::getDefaultFont());
    
    QString scoreText = QString("Score: %1").arg(m_score.getScore());
    QString comboText = QString("Combo: %1").arg(m_score.getCombo());
    QString timeText = QString("Time: %1").arg(getCurrentGameTime(), 0, 'f', 1);
    
    painter.drawText(10, 40, scoreText);
    painter.drawText(10, 80, comboText);
    painter.drawText(10, 120, timeText);
}

void GamePage::drawHitEffects(QPainter& painter) {
    for (HitEffect& effect : m_hitEffects) {
        if (!effect.isExpired()) {
            effect.update();
            painter.setOpacity(effect.getOpacity());
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(effect.getColor(), 4));
            painter.drawEllipse(effect.getPosition(), effect.getRadius(), effect.getRadius());
            painter.setOpacity(1.0); // Reset Transparency
        }
    }
    
    // Remove expired hit effects
    m_hitEffects.erase(std::remove_if(m_hitEffects.begin(), m_hitEffects.end(),
                                       [](const HitEffect& effect) { return effect.isExpired(); }),
                       m_hitEffects.end());
}

void GamePage::onUpdateActiveNotes(QList<Note*> notes) {
    m_activeNotes = notes;
    update(); 
}

void GamePage::onScoreChanged(const GameScore& score) {
    m_score = score;
    update();
}

double GamePage::getCurrentGameTime() const {
    return m_currentGameTime;
}

void GamePage::setCurrentGameTime(double time) {
    m_currentGameTime = time;
}

void GamePage::onGameStart(const QString& musicPath) {
    m_currentGameTime = 0.0; // Reset game time
    m_updateTimer->start();
    m_player->setMedia(QUrl(musicPath));
    m_player->setVolume(m_gameConfig->musicVolume() * 100); // set volume
    m_player->play(); // Play music
}

void GamePage::onNotePressed(int track, double duration) {
    for (auto& effect : m_hitEffects) {
        if (effect.getPosition().x() >= GameConstants::TRACK_START_X + track * GameConstants::TRACK_WIDTH &&
            effect.getPosition().x() < GameConstants::TRACK_START_X + (track + 1) * GameConstants::TRACK_WIDTH) {
            effect.setLifetime(duration * 1000 + 500); // Set the lifecycle of hit effects
        }
    }
}

void GamePage::onNoteReleased(int track) {
    for (auto& effect : m_hitEffects) {
        if (effect.getPosition().x() >= GameConstants::TRACK_START_X + track * GameConstants::TRACK_WIDTH &&
            effect.getPosition().x() < GameConstants::TRACK_START_X + (track + 1) * GameConstants::TRACK_WIDTH) {
            effect.setLifetime(100);
        }
    }
}

void GamePage::drawHitFeedBack(QPainter& painter) {
    //定义画笔
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor("#00B0AE"));
    painter.setPen(pen);

    //创建画刷
    QBrush brush;
    brush.setColor(QColor("#00B0AE"));
    brush.setStyle(Qt::Dense6Pattern);
    painter.setBrush(brush);

    for (int track : m_pressedTracks) {
        QRect tempRt(GameConstants::TRACK_START_X + track * GameConstants::TRACK_WIDTH, GameConstants::JUDGE_LINE_Y, 
                     GameConstants::TRACK_WIDTH, 300);
        painter.drawRect(tempRt);
    }
}

void GamePage::onPauseButtonClicked() {
    m_updateTimer->stop();
    m_player->pause();
    
    QPixmap snap = this->grab();
    m_pauseLabel->setPixmap(snap);
    auto blur = new QGraphicsBlurEffect(m_pauseOverlay);
    blur->setBlurRadius(15);
    m_pauseLabel->setGraphicsEffect(blur);

    m_pauseOverlay->show();

    connect(m_pauseResumeButton, &QPushButton::clicked, this, [this]() {
        m_pauseOverlay->hide();
        m_updateTimer->start();
        m_player->play();
        emit resumeGame();
    });

    connect(m_pauseBackButton, &QPushButton::clicked, this, [this]() {
        m_pauseOverlay->hide();
        m_updateTimer->stop();
        m_player->stop();
        emit backToMainMenu();
    });

    connect(m_pauseQuitButton, &QPushButton::clicked, this, [this]() {
        QApplication::quit();
    });
}