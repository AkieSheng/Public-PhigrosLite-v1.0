#pragma once

#include <QObject>
#include "GameScore.h"

class ResultViewModel : public QObject {
    Q_OBJECT
    
public:
    explicit ResultViewModel(QObject* parent = nullptr);
    
    void setGameResult(const GameScore& score);
    
    int getScore() const { return m_gameScore.getScore(); }
    int getMaxCombo() const { return m_gameScore.getMaxCombo(); }
    int getPerfectCount() const { return m_gameScore.getPerfectCount(); }
    int getGoodCount() const { return m_gameScore.getGoodCount(); }
    int getMissCount() const { return m_gameScore.getMissCount(); }
    
public slots:
    void gameFinished(GameScore gameScore);

private:
    GameScore m_gameScore;
};