#pragma once

#include <QObject>

class MainWindow;
class SelectViewModel;
class GameViewModel;
class ResultViewModel;

class Application : public QObject {
    Q_OBJECT
    
public:
    explicit Application(QObject* parent = nullptr);
    ~Application();
    
    bool initialize();
    void show();
    void quit();
    
private slots:
    void onConfigChanged();
    
private:
    MainWindow* m_mainWindow;
    SelectViewModel* m_selectViewModel;
    GameViewModel* m_gameViewModel;
    ResultViewModel* m_resultViewModel;
    
    bool setupResourcePaths();
    bool loadConfiguration();
    void setupViewModels();
    void connectSignals();
};