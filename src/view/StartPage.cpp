#include <StartPage.h>
#include "Font.h"
#include "Constants.h"

StartPage::StartPage(QWidget* parent)
    : QWidget(parent) {
        
    QFont font = Font::getDefaultFont();

    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setPixmap(QPixmap(":/images/start_background.jpg"));
    m_backgroundLabel->setScaledContents(true);
    m_backgroundLabel->resize(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    
    m_startButton = new QPushButton("Start", this);
    m_startButton->setFont(font);
    m_startButton->setGeometry(500, 495, 200, 50);
    connect(m_startButton, &QPushButton::clicked, this, [this]() {
        emit startGameClicked();
    });
    
    m_settingsButton = new QPushButton("Settings", this);
    m_settingsButton->setFont(font);
    m_settingsButton->setGeometry(500, 560, 200, 50);
    connect(m_settingsButton, &QPushButton::clicked, this, [this]() {
        emit settingsClicked();
    });

    m_exitButton = new QPushButton("Exit", this);
    m_exitButton->setFont(font);
    m_exitButton->setGeometry(500, 625, 200, 50);
    connect(m_exitButton, &QPushButton::clicked, this, [this]() {
        emit exitClicked();
    });

}