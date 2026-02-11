#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QSlider>
#include <QSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "GameConfig.h"

class SettingPage : public QWidget {
    Q_OBJECT

public:
    explicit SettingPage(QWidget* parent = nullptr);

signals:
    void backToMenuClicked();
    void saveSettingsClicked();

private slots:
    void onKeyBindingButtonClicked();
    void onOffsetChanged(int value);

private:
    QPushButton* m_backButton;
    QPushButton* m_saveButton;
    QLabel* m_backgroundLabel;

    // Volume Setting
    QLabel* m_musicVolumeLabel;
    QSlider* m_musicVolumeSlider;
    QLabel* m_musicVolumeValueLabel;

    // Key Binding Settings
    QLabel* m_keyBindingLabel;
    QList<QPushButton*> m_keyBindingButtons;
    QPushButton* m_waitingForKeyButton;

    // Offset Settings
    QLabel* m_offsetLabel;
    QSpinBox* m_offsetSpinBox;
    QLabel* m_offsetValueLabel;

    GameConfig* m_gameConfig;
    
    void setupVolumeControls();
    void setupKeyBindingControls();
    void setupOffsetControls();
    void updateKeyBindingButtons();
    void saveAllSettings();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};