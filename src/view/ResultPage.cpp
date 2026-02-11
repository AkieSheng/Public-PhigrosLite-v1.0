#include "ResultPage.h"
#include "Constants.h"
#include "Font.h"

ResultPage::ResultPage(QWidget* parent)
    : QWidget(parent) {

    QFont font = Font::getGamingFont();

    m_BackgroundLabel = new QLabel(this);
    m_BackgroundLabel->setPixmap(QPixmap(":/images/result_background.jpg"));
    m_BackgroundLabel->setScaledContents(true);
    m_BackgroundLabel->resize(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);

    m_backButton = new QPushButton("Back to Menu", this);
    m_backButton->setFont(font);
    m_backButton->setGeometry(20, 20, 150, 40);
    connect(m_backButton, &QPushButton::clicked, this, [this]() {
        emit backToMenuClicked();
    });

    m_retryButton = new QPushButton("Retry", this);
    m_retryButton->setFont(font);
    m_retryButton->setGeometry(20, 70, 150, 40);
    connect(m_retryButton, &QPushButton::clicked, this, [this]() {
        emit retryClicked();
    });

    QFont titleFont = Font::getTitleFont();
    m_ScoreLabel = new QLabel(this);
    m_ScoreLabel->setFont(titleFont);
    m_ScoreLabel->setGeometry(0, 250, GameConstants::WINDOW_WIDTH, 50);
    m_ScoreLabel->setStyleSheet("QLabel { background-color: transparent; color: white; }");
    m_ScoreLabel->setAlignment(Qt::AlignCenter);

    m_GradeLabel = new QLabel(this);
    m_GradeLabel->setFont(titleFont);
    m_GradeLabel->setGeometry(0, 350, GameConstants::WINDOW_WIDTH, 100);
    m_GradeLabel->setAlignment(Qt::AlignCenter);
}

void ResultPage::updateScore(const GameScore& score) {
    m_ScoreLabel->setText(QString("Score: %1").arg(score.getScore()));
}

void ResultPage::updateGrade(const QString& grade) {
    m_GradeLabel->setText(QString("Grade: %1").arg(grade));

    if (grade == "S") {
        m_GradeLabel->setStyleSheet("QLabel { background-color: transparent; color: gold; font-weight: bold; font-size: 80px; }");
    } else if (grade == "A") {
        m_GradeLabel->setStyleSheet("QLabel { background-color: transparent; color: silver; font-weight: bold; font-size: 80px;}");
    } else if (grade == "B") {
        m_GradeLabel->setStyleSheet("QLabel { background-color: transparent; color: #cd853f; font-weight: bold; font-size: 80px;}");
    } else if (grade == "C") {
        m_GradeLabel->setStyleSheet("QLabel { background-color: transparent; color: #8b4513; font-weight: bold; font-size: 80px;}");
    } else {
        m_GradeLabel->setStyleSheet("QLabel { background-color: transparent; color: white; font-weight: bold; font-size: 80px;}");
    }
}

