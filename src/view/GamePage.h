#pragma once

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QList>
#include <QMediaPlayer>
#include <QVector>
#include <QPushbutton>
#include "Constants.h"
#include "GameScore.h"
#include "Note.h"
#include "Timer.h"
#include "GameConfig.h"
#include "HitEffect.h"

class GamePage : public QWidget {
    Q_OBJECT
    
public:
    explicit GamePage(QWidget* parent = nullptr);
    double getCurrentGameTime() const;
    void setCurrentGameTime(double time);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    
public slots:
    void updateDisplay();
    void onNoteHit(int track, int accuracy);
    void onUpdateActiveNotes(QList<Note*> notes);
    void onScoreChanged(const GameScore& score);
    void onGameStart(const QString& musicPath);
    void onNotePressed(int track, double duration);
    void onNoteReleased(int track);
    void onPauseButtonClicked();

signals:
    // signal to result page
    void gameFinished(GameScore score);
    // signal to game view model
    void keyPressed(int track);
    void keyReleased(int track);
    void timeUpdated(double time);
    void finishGame();
    void pauseGame();
    void resumeGame();
    void backToMainMenu();
    
private:
    QTimer* m_updateTimer;
    QMediaPlayer* m_player;
    QLabel* m_backgroundLabel;
    QList<Note*> m_activeNotes;
    GameScore m_score;
    double m_currentGameTime = 0.0;
    GameConfig* m_gameConfig;
    QVector<HitEffect> m_hitEffects;
    QPushButton* m_pauseButton;
    QList<int> m_pressedTracks;
    QWidget *m_pauseOverlay; 
    QLabel* m_pauseLabel;
    QLabel* m_pauseMaskLabel;
    QPushButton* m_pauseResumeButton;
    QPushButton* m_pauseBackButton;
    QPushButton* m_pauseQuitButton;
    
    void drawJudgeLine(QPainter& painter);
    void drawNotes(QPainter& painter);
    void drawTrackLines(QPainter& painter);
    void drawScore(QPainter& painter);
    void drawHitEffects(QPainter& painter);
    void drawHitFeedBack(QPainter& painter);
};