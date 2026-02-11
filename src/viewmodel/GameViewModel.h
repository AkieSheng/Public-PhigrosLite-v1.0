#pragma once

#include <QObject>
#include <QStringList>
#include <QList>
#include "Chart.h"
#include "GameScore.h"

class GameViewModel : public QObject {
    Q_OBJECT
    
public:
    explicit GameViewModel(QObject* parent = nullptr);
    ~GameViewModel() = default;

public slots:
    // Interact with SelectViewModel
    void getChartList(const QStringList& chartPaths) {m_chartPaths = chartPaths;} // Load chart list
    // Interact with view
    void startGame(int index);  // Select the index-th chart and start the game
    void pauseGame();   // Pause the game
    void resumeGame();  // Resume the game
    void finishGame();  // End the game
    void handleKeyPress(int track); // Press the track-th track
    void handleKeyRelease(int track); // Release the track-th track
    void update(double deltaTime); // Update game state, deltaTime is the time increment

signals:
    void notesUpdated(QList<Note*> notes); // Note update notification
    void scoreChanged(GameScore m_gameScore); // Score change notification
    void noteHit(int track, GameScore::Accuracy accuracy); // Note hit notification
    void notePressed(int track, double duration); // HOLD note pressed notification
    void noteReleased(int track); // HOLD note released notification
    void gameFinished(const GameScore& finalScore); // Game finished notification
    void gameStarted(const QString& musicPath); // Game started notification
    void GradeSign(const QString& grade); // Score grade notification

private:
    Chart m_currentChart;
    GameScore m_gameScore;
    bool m_isPlaying = false; // Whether the game is in progress
    double m_currentTime = 0.0; // Current game time
    QStringList m_chartPaths;
    QList<Note*> m_visibleNotes; // List of visible notes
    int m_maxscore = 0; // Maximum score

    int calculateMaxScore(const QList<Note> notes);

    constexpr static double MISS_WINDOW = 0.20; // Timeout window, in seconds
    constexpr static double GOOD_WINDOW = 0.15; // Good window, in seconds
    constexpr static double PERFECT_WINDOW = 0.10; // Perfect window, in seconds

};