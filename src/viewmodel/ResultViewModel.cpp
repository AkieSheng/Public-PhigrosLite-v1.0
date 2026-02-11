#include "ResultViewModel.h"

ResultViewModel::ResultViewModel(QObject* parent) : QObject(parent)
{
    m_gameScore.init();
}

void ResultViewModel::setGameResult(const GameScore& score)
{
    m_gameScore = score;
}

void ResultViewModel::gameFinished(GameScore gameScore)
{
    setGameResult(gameScore);
}