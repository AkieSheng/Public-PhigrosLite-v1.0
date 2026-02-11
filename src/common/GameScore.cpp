#include "GameScore.h"
#include <algorithm>

void GameScore::addHit(Accuracy accuracy) {
    switch (accuracy) {
        case MISS: // Miss
            addMiss();
            return;
        case GOOD: // Good
            m_combo++;
            m_maxCombo = std::max(m_maxCombo, m_combo);
            m_score += GameScore::GOOD_SCORE + m_combo;
            m_good++;
            break;
        case PERFECT: // Perfect
            m_combo++;
            m_maxCombo = std::max(m_maxCombo, m_combo);
            m_score += GameScore::PERFECT_SCORE + m_combo;
            m_perfect++;
            break;
    }
}

void GameScore::addMiss() {
    m_combo = 0;
    m_miss++;
}

void GameScore::addPress(Timing timing) {
    switch (timing) {
        case EARLY: // Release early
            m_combo = 0;
            m_miss++;
            break;
        case LATE: // Delay release
            m_combo++;
            m_maxCombo = std::max(m_maxCombo, m_combo);
            m_score += GameScore::PRESS_SCORE;
            break;
    }
}

void GameScore::init() {
    m_score = 0;
    m_combo = 0;
    m_maxCombo = 0;
    m_perfect = 0;
    m_good = 0;
    m_miss = 0;
}