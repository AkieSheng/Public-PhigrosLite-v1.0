#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>

class StartPage : public QWidget {
    Q_OBJECT
    
public:
    explicit StartPage(QWidget* parent = nullptr);
    
signals:
    void startGameClicked();
    void settingsClicked();
    void exitClicked();
    
private:
    QPushButton* m_startButton;
    QPushButton* m_settingsButton;
    QPushButton* m_exitButton;
    QLabel* m_backgroundLabel;
    
};