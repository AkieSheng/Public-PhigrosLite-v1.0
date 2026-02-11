#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include "StartPage.h"
#include "SelectPage.h"
#include "GamePage.h"
#include "ResultPage.h"
#include "SettingPage.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    // Add getter functions
    StartPage* getStartPage() const { return m_startPage; }
    SelectPage* getSelectPage() const { return m_selectPage; }
    GamePage* getGamePage() const { return m_gamePage; }
    ResultPage* getResultPage() const { return m_resultPage; }

    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void showStartPage();
    void showSelectPage();
    void showGamePage();
    void showResultPage();
    void showSettingPage();
    
private:
    QStackedWidget* m_stackedPages;
    StartPage* m_startPage;
    SettingPage* m_settingPage;
    SelectPage* m_selectPage;
    GamePage* m_gamePage;
    ResultPage* m_resultPage;
};