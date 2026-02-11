#include "Application.h"
#include "MainWindow.h"
#include "SelectViewModel.h"
#include "GameViewModel.h"
#include "ResultViewModel.h"
#include <QDebug>
#include <QDir>

Application::Application(QObject* parent)
    : QObject(parent)
    , m_mainWindow(nullptr)
    , m_selectViewModel(nullptr)
    , m_gameViewModel(nullptr)
    , m_resultViewModel(nullptr)
{
}

Application::~Application()
{
    delete m_mainWindow;
}

bool Application::initialize() {
    if (!setupResourcePaths()) {
        return false;
    }

    m_mainWindow = new MainWindow();
    
    setupViewModels();
    connectSignals();
    
    return true;
}

void Application::show() {
    if (m_mainWindow) {
        m_mainWindow->show();
    }
}

void Application::quit() {
    if (m_mainWindow) {
        m_mainWindow->close();
    }
}

bool Application::setupResourcePaths() {
    QDir resourcesDir("resources");
    if (!resourcesDir.exists()) {
        qWarning() << "Resources directory not found! Creating directories...";
        return false;
    }
    return true;
}

bool Application::loadConfiguration() {
    qDebug() << "Configuration loaded (simplified)";
    return true;
}

void Application::setupViewModels() {
    if (!m_mainWindow) return;

    m_selectViewModel = new SelectViewModel(this);
    m_gameViewModel = new GameViewModel(this);
    m_resultViewModel = new ResultViewModel(this);

}

void Application::connectSignals() {
    if (!m_mainWindow) return;

    StartPage* startPage = m_mainWindow->getStartPage();
    SelectPage* selectPage = m_mainWindow->getSelectPage();
    GamePage* gamePage = m_mainWindow->getGamePage();
    ResultPage* resultPage = m_mainWindow->getResultPage();
    
    // StartPage Connection
    connect(startPage, &StartPage::exitClicked, this, &Application::quit);
    connect(startPage, &StartPage::startGameClicked, m_selectViewModel, &SelectViewModel::loadAvailableCharts);

    // SelectPage Connection
    QStringList chartTitles = m_selectViewModel->getChartTitles();
    QMetaObject::invokeMethod(selectPage, "onGetChartTitles", 
                             Q_ARG(QStringList, chartTitles));
    
    connect(selectPage, &SelectPage::chartSelected, m_gameViewModel, &GameViewModel::startGame);
    connect(m_selectViewModel, &SelectViewModel::sendChartPaths, m_gameViewModel, &GameViewModel::getChartList);

    // GamePage Connection
    connect(m_gameViewModel, &GameViewModel::gameStarted, gamePage, &GamePage::onGameStart);
    connect(gamePage, &GamePage::keyPressed, m_gameViewModel, &GameViewModel::handleKeyPress);
    connect(gamePage, &GamePage::keyReleased, m_gameViewModel, &GameViewModel::handleKeyRelease);
    connect(m_gameViewModel, &GameViewModel::noteHit, gamePage, &GamePage::onNoteHit);
    connect(m_gameViewModel, &GameViewModel::notePressed, gamePage, &GamePage::onNotePressed);
    connect(m_gameViewModel, &GameViewModel::noteReleased, gamePage, &GamePage::onNoteReleased);
    connect(gamePage, &GamePage::timeUpdated, m_gameViewModel, &GameViewModel::update);
    connect(gamePage, &GamePage::pauseGame, m_gameViewModel, &GameViewModel::pauseGame);
    connect(gamePage, &GamePage::resumeGame, m_gameViewModel, &GameViewModel::resumeGame);
    // Update note's data
    connect(m_gameViewModel, &GameViewModel::notesUpdated, gamePage, &GamePage::onUpdateActiveNotes);
    connect(m_gameViewModel, &GameViewModel::scoreChanged, gamePage, &GamePage::onScoreChanged);
    
    connect(m_gameViewModel, &GameViewModel::gameFinished, m_resultViewModel, &ResultViewModel::gameFinished);

    connect(m_gameViewModel, &GameViewModel::gameFinished, gamePage, &GamePage::gameFinished);
    connect(m_gameViewModel, &GameViewModel::gameFinished, resultPage, &ResultPage::updateScore);
    connect(m_gameViewModel, &GameViewModel::GradeSign, resultPage, &ResultPage::updateGrade);
    connect(gamePage, &GamePage::finishGame, m_gameViewModel, &GameViewModel::finishGame);
    
    // ResultPage Connection
    connect(resultPage, &ResultPage::retryClicked, m_mainWindow, &MainWindow::showSelectPage);
    
    qDebug() << "All signals connected successfully";
}

void Application::onConfigChanged() {
    qDebug() << "Configuration changed";
}