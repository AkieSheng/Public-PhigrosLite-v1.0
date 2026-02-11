#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "GameScore.h"

class ResultPage : public QWidget {
    Q_OBJECT
public:
    explicit ResultPage(QWidget* parent = nullptr);

public slots:
    void updateScore(const GameScore& score);
    void updateGrade(const QString& grade);

signals:
    void backToMenuClicked();
    void retryClicked();

private:

    QPushButton* m_backButton;
    QPushButton* m_retryButton;
    QLabel* m_ScoreLabel;
    QLabel* m_GradeLabel;
    QLabel* m_BackgroundLabel;
};
