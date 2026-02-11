#pragma once

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class Timer : public QObject {
    Q_OBJECT
    
public:
    explicit Timer(QObject* parent = nullptr);

    void start();
    void pause();
    void resume();
    void stop();
    void reset();
    
    double getElapsedTime() const;      // Elapsed time (s)
    double getDeltaTime() const;        // Frame interval time (s)
    bool isRunning() const;
    
    // 更新频率
    void setUpdateInterval(int msec);
    
signals:
    void timeout();                     // Trigger signal of timer
    void tick(double deltaTime);        // Update signal for each frame
    
private slots:
    void onTimeout();
    
private:
    QTimer* m_timer;
    QElapsedTimer m_elapsedTimer;
    double m_pausedTime;
    double m_lastFrameTime;
    bool m_isPaused;
};