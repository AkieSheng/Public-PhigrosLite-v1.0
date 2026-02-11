#include "GameViewModel.h"
#include "Constants.h"
#include "SelectViewModel.h"
#include <QDebug>
#include <algorithm>

GameViewModel::GameViewModel(QObject* parent)
    : QObject(parent), m_isPlaying(false), m_currentTime(0.0)
{
    // Initialize game score
    m_gameScore.init();
}

void GameViewModel::startGame(int index)
{
    // Load charts
    Chart chart;
    if (index >= 0 && index < m_chartPaths.size()) {
        QString filePath = m_chartPaths.at(index);
        chart.loadFromFile(filePath);
    } else {
        qWarning() << "Invalid chart index:" << index;
        return;
    }

    m_currentChart = chart;
    m_isPlaying = true;
    m_currentTime = 0;
    m_gameScore.init();
    m_visibleNotes.clear();
    m_maxscore = calculateMaxScore(m_currentChart.getNotes());

    emit notesUpdated(m_visibleNotes);
    emit scoreChanged(m_gameScore);
    emit gameStarted(chart.getMusicFile()); // Notify the game to start

    qDebug() << "Game started successfully";
}

void GameViewModel::pauseGame()
{
    if (!m_isPlaying) return;
    m_isPlaying = false;
}

void GameViewModel::resumeGame()
{
    if (m_isPlaying) return;
    m_isPlaying = true;
}

void GameViewModel::finishGame()
{
    if (!m_isPlaying) return;
    m_isPlaying = false;
    QString grade;
    double acc = m_gameScore.getScore() / (double)m_maxscore;
    if (acc >= 0.9) {
        grade = "S";
    } else if (acc >= 0.75) {
        grade = "A";
    } else if (acc >= 0.6) {
        grade = "B";
    } else if (acc >= 0.45) {
        grade = "C";
    } else {
        grade = "D";
    }
    // Notify at the end of the game
    emit gameFinished(m_gameScore);
    emit GradeSign(grade);
    qDebug() << "Game max score:" << m_maxscore;
    qDebug() << "Game finished with score:" << m_gameScore.getScore() << "and grade:" << grade;
}

void GameViewModel::handleKeyPress(int track)
{
    if (!m_isPlaying) return;
    
    for (auto* note : m_visibleNotes) {
        if (note->getTrack() == track && note->isActive()) {
            double timeDiff = qAbs(note->getTime() - m_currentTime);
            if (note->getType() == Note::TAP) {
                if (timeDiff <= GameViewModel::PERFECT_WINDOW) {
                    note->setActive(false); // Mark as struck
                    m_gameScore.addHit(GameScore::PERFECT);
                    emit noteHit(track, GameScore::PERFECT); // Perfect
                    emit notesUpdated(m_visibleNotes);
                    emit scoreChanged(m_gameScore);
                    return;
                } else if (timeDiff <= GameViewModel::GOOD_WINDOW) {
                    note->setActive(false); // Mark as struck
                    m_gameScore.addHit(GameScore::GOOD);
                    emit noteHit(track, GameScore::GOOD); // Good
                    emit notesUpdated(m_visibleNotes);
                    emit scoreChanged(m_gameScore);
                    return;
                } else if (timeDiff <= GameViewModel::MISS_WINDOW) {
                    note->setActive(false); // Mark as struck
                    m_gameScore.addHit(GameScore::MISS);
                    emit noteHit(track, GameScore::MISS); // Miss
                    emit notesUpdated(m_visibleNotes);
                    emit scoreChanged(m_gameScore);
                    return;
                }
            }
            else if (note->getType() == Note::HOLD) {
                if (timeDiff <= GameViewModel::PERFECT_WINDOW) {
                    note->setPressed(true); // Mark as pressed
                    m_gameScore.addHit(GameScore::PERFECT);
                    emit scoreChanged(m_gameScore);
                    emit noteHit(track, GameScore::PERFECT); // Perfect
                    emit notesUpdated(m_visibleNotes);
                    emit notePressed(track, note->getHoldDuration());
                    return;
                } else if (timeDiff <= GameViewModel::GOOD_WINDOW) {
                    note->setPressed(true); // Mark as pressed
                    m_gameScore.addHit(GameScore::GOOD);
                    emit scoreChanged(m_gameScore);
                    emit noteHit(track, GameScore::GOOD); // Good
                    emit notesUpdated(m_visibleNotes);
                    emit notePressed(track, note->getHoldDuration());
                    return;
                } else if (timeDiff <= GameViewModel::MISS_WINDOW) {
                    note->setActive(false); // Mark as pressed
                    m_gameScore.addHit(GameScore::MISS);
                    emit noteHit(track, GameScore::MISS); // Miss
                    emit notesUpdated(m_visibleNotes);
                    emit scoreChanged(m_gameScore);
                    return;
                }
            }
        }
    }
}

void GameViewModel::handleKeyRelease(int track)
{
    if (!m_isPlaying) return;

    for (auto* note : m_visibleNotes) {
        if (note->isActive() && note->getTrack() == track && note->getType() == Note::HOLD && note->isPressed()) {
            if (m_currentTime < note->getTime() + note->getHoldDuration()) {
                // Release early
                note->setActive(false); // Mark as struck
                note->setPressed(false); // Mark as release
                m_gameScore.addPress(GameScore::EARLY);
                emit noteReleased(track);
                emit notesUpdated(m_visibleNotes);
                emit scoreChanged(m_gameScore);
                return;
            } else {
                // Release normally
                note->setActive(false); // Mark as struck
                note->setPressed(false); // Mark as release
                m_gameScore.addPress(GameScore::LATE);
                emit noteReleased(track);
                emit notesUpdated(m_visibleNotes);
                emit scoreChanged(m_gameScore);
                return;
            }
        }
    }
}

void GameViewModel::update(double deltaTime)
{
    if (!m_isPlaying) return;

    m_currentTime += deltaTime;
    // Update visible note list
    m_visibleNotes.clear();
    for(auto& note : m_currentChart.getNotes()) {
        double noteTime = note.getTime();
        double fallTime = 2.0;
        double timeToHit = noteTime - m_currentTime;
        int y = GameConstants::JUDGE_LINE_Y - (int)(timeToHit / fallTime * GameConstants::JUDGE_LINE_Y);
        if (note.getType() == Note::TAP ) {
            // TAP note
            if (y <= GameConstants::WINDOW_HEIGHT && y >= 0) {
                note.setY(y);
                note.setHeight(GameConstants::NOTE_HEIGHT);
                m_visibleNotes.push_back(&note);
            }
        } else if (note.getType() == Note::HOLD ) {
            // HOLD note
            double holdDuration = note.getHoldDuration();
            int holdHeight = (int)(holdDuration / fallTime * GameConstants::JUDGE_LINE_Y);
            if (y - holdHeight <= GameConstants::JUDGE_LINE_Y && y >= 0) 
            {
                note.setY(y);
                note.setHeight(holdHeight);
                m_visibleNotes.push_back(&note);
            }
        }
    }
    emit notesUpdated(m_visibleNotes);
    //qDebug() << "GameViewModel updated. Current time:" << m_currentTime << "\n";
}

int GameViewModel::calculateMaxScore(const QList<Note> notes)
{
    GameScore maxScore;
    for (const auto& note : notes) {
        if (note.getType() == Note::TAP) {
            maxScore.addHit(GameScore::PERFECT);
        } else if (note.getType() == Note::HOLD) {
            maxScore.addHit(GameScore::PERFECT);
            maxScore.addPress(GameScore::LATE);
        }
    }
    return maxScore.getScore();
}