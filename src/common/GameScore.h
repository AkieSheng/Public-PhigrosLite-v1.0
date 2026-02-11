#pragma once

class GameScore {
public:
    constexpr static int GOOD_SCORE = 50;
    constexpr static int PERFECT_SCORE = 100;
    constexpr static int PRESS_SCORE = 150;

    enum Accuracy {MISS, GOOD, PERFECT};
    enum Timing {EARLY, LATE};
    void addHit(Accuracy accuracy);
    void addMiss();
    void init();
    void addPress(Timing timing);

    int getScore() const { return m_score; }
    int getCombo() const { return m_combo; }
    int getMaxCombo() const { return m_maxCombo; }
    int getPerfectCount() const { return m_perfect; }
    int getGoodCount() const { return m_good; }
    int getMissCount() const { return m_miss; }
    
private:
    int m_score = 0;
    int m_combo = 0;
    int m_maxCombo = 0;
    int m_perfect = 0;
    int m_good = 0;
    int m_miss = 0;
};