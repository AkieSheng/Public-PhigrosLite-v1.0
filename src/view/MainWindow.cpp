#include "MainWindow.h"
#include "StartPage.h"
#include "SelectPage.h"
#include "GamePage.h"
#include "ResultPage.h"
#include "Constants.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_startPage(new StartPage()),
      m_settingPage(new SettingPage()),
      m_selectPage(new SelectPage()),
      m_gamePage(new GamePage()),
      m_resultPage(new ResultPage()),
      m_stackedPages(new QStackedWidget(this)) {
    
    setWindowTitle("Simple Phigros");
    this->resize(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    
    m_stackedPages->addWidget(m_startPage);
    m_stackedPages->addWidget(m_settingPage);
    m_stackedPages->addWidget(m_selectPage);
    m_stackedPages->addWidget(m_gamePage);
    m_stackedPages->addWidget(m_resultPage);
    m_stackedPages->setCurrentWidget(m_startPage);

    setCentralWidget(m_stackedPages);
    
    connect(m_startPage, &StartPage::startGameClicked, this, &MainWindow::showSelectPage);
    connect(m_startPage, &StartPage::settingsClicked, this, &MainWindow::showSettingPage);

    connect(m_settingPage, &SettingPage::backToMenuClicked, this, &MainWindow::showStartPage);
    connect(m_settingPage, &SettingPage::saveSettingsClicked, this, &MainWindow::showStartPage);

    connect(m_selectPage, &SelectPage::chartSelected, this, &MainWindow::showGamePage);
    connect(m_selectPage, &SelectPage::backToMenuClicked, this, &MainWindow::showStartPage);

    connect(m_gamePage, &GamePage::gameFinished, this, [this]() {
        showResultPage();
    });
    connect(m_gamePage, &GamePage::gameFinished, m_resultPage, &ResultPage::updateScore);
    connect(m_gamePage, &GamePage::backToMainMenu, this, &MainWindow::showStartPage);

    connect(m_resultPage, &ResultPage::backToMenuClicked, this, &MainWindow::showStartPage);
    connect(m_resultPage, &ResultPage::retryClicked, this, &MainWindow::showSelectPage);
}

void MainWindow::showStartPage() {
    m_stackedPages->setCurrentWidget(m_startPage);
}

void MainWindow::showSettingPage() {
    m_stackedPages->setCurrentWidget(m_settingPage);
}

void MainWindow::showSelectPage() {
    m_stackedPages->setCurrentWidget(m_selectPage);
}

void MainWindow::showGamePage() {
    m_stackedPages->setCurrentWidget(m_gamePage);
}

void MainWindow::showResultPage() {
    m_stackedPages->setCurrentWidget(m_resultPage);
}


