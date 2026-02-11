#include "Timer.h"

Timer::Timer(QObject* parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_pausedTime(0.0)
    , m_lastFrameTime(0.0)
    , m_isPaused(false)
{
    m_timer->setInterval(16); // 60FPS
    connect(m_timer, &QTimer::timeout, this, &Timer::onTimeout);
}

void Timer::start() {
    m_elapsedTimer.start();
    m_timer->start();
    m_pausedTime = 0.0;
    m_lastFrameTime = 0.0;
    m_isPaused = false;
}

void Timer::pause() {
    if (!m_isPaused && m_timer->isActive()) {
        m_timer->stop();
        m_isPaused = true;
    }
}

void Timer::resume() {
    if (m_isPaused) {
        double currentTime = m_elapsedTimer.elapsed() / 1000.0;
        m_pausedTime += (currentTime - m_lastFrameTime);
        
        m_timer->start();
        m_isPaused = false;
    }
}

void Timer::stop() {
    m_timer->stop();
    m_isPaused = false;
}

void Timer::reset() {
    stop();
    m_pausedTime = 0.0;
    m_lastFrameTime = 0.0;
}

double Timer::getElapsedTime() const {
    if (!m_elapsedTimer.isValid()) return 0.0;
    
    double totalTime = m_elapsedTimer.elapsed() / 1000.0;
    return totalTime - m_pausedTime;
}

double Timer::getDeltaTime() const {
    double currentTime = getElapsedTime();
    double deltaTime = currentTime - m_lastFrameTime;
    return deltaTime;
}

bool Timer::isRunning() const {
    return m_timer->isActive() && !m_isPaused;
}

void Timer::setUpdateInterval(int msec) {
    m_timer->setInterval(msec);
}

void Timer::onTimeout() {
    double currentTime = getElapsedTime();
    double deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;
    
    emit timeout();
    emit tick(deltaTime);
}