#include "SettingPage.h"
#include "Constants.h"
#include "Font.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

SettingPage::SettingPage(QWidget* parent)
    : QWidget(parent), m_waitingForKeyButton(nullptr) {
    
    QFont font = Font::getDefaultFont();

    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setPixmap(QPixmap(":/images/setting_background.jpg"));
    m_backgroundLabel->setScaledContents(true);
    m_backgroundLabel->resize(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);

    m_backButton = new QPushButton("Back", this);
    m_backButton->setFont(font);
    m_backButton->setGeometry(20, 20, 100, 40);
    connect(m_backButton, &QPushButton::clicked, this, [this]() {
        emit backToMenuClicked();
    });

    m_saveButton = new QPushButton("Save", this);
    m_saveButton->setFont(font);
    m_saveButton->setGeometry(GameConstants::WINDOW_WIDTH - 120, 20, 100, 40);
    connect(m_saveButton, &QPushButton::clicked, this, &SettingPage::saveAllSettings);

    // Load game configuration
    m_gameConfig = GameConfig::instance();
    qDebug() << "GameConfig instance loaded with music volume:" << m_gameConfig->musicVolume();

    // Setup all controls
    setupVolumeControls();
    setupKeyBindingControls();
    setupOffsetControls();
    
    // Update key binding display
    updateKeyBindingButtons();
}

void SettingPage::setupVolumeControls() {
    QFont font = Font::getDefaultFont();
    
    // Volume settings
    m_musicVolumeLabel = new QLabel("Music Volume:", this);
    m_musicVolumeLabel->setStyleSheet("color: white; font-size: 48px;");
    m_musicVolumeLabel->setFont(font);
    m_musicVolumeLabel->setGeometry(100, 80, 400, 60);

    m_musicVolumeSlider = new QSlider(Qt::Horizontal, this);
    m_musicVolumeSlider->setGeometry(100, 160, 800, 60);
    m_musicVolumeSlider->setRange(0, 100);
    m_musicVolumeSlider->setValue(static_cast<int>(m_gameConfig->musicVolume() * 100));
    
    m_musicVolumeValueLabel = new QLabel(QString::number(m_musicVolumeSlider->value()) + "%", this);
    m_musicVolumeValueLabel->setStyleSheet("color: white; font-size: 40px;");
    m_musicVolumeValueLabel->setFont(font);
    m_musicVolumeValueLabel->setGeometry(920, 160, 120, 60);
    
    connect(m_musicVolumeSlider, &QSlider::valueChanged, this, [this](int value) {
        m_musicVolumeValueLabel->setText(QString::number(value) + "%");
    });

    m_musicVolumeSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            border: none;
            height: 32px;
            background: #cccccc;
            border-radius: 8px;
        }
        QSlider::sub-page:horizontal {
            background: #3daee9;
            border-radius: 8px;
        }
        QSlider::add-page:horizontal {
            background: #eeeeee;
            border-radius: 8px;
        }
        QSlider::handle:horizontal {
            background: #0078d7;
            border: 4px solid white;
            width: 40px;
            height: 40px;
            margin: -6px 0;
            border-radius: 8px;
        }
        QSlider::handle:horizontal:hover {
            background: #00aaff;
        }
        QSlider::handle:horizontal:pressed {
            background: #005577;
        }
    )");
}

void SettingPage::setupKeyBindingControls() {
    QFont font = Font::getDefaultFont();
    
    // Key binding settings
    m_keyBindingLabel = new QLabel("Key Bindings:", this);
    m_keyBindingLabel->setStyleSheet("color: white; font-size: 48px;");
    m_keyBindingLabel->setFont(font);
    m_keyBindingLabel->setGeometry(100, 250, 400, 60);

    // Create 8 key binding buttons
    QStringList trackNames = {"Track 1", "Track 2", "Track 3", "Track 4", 
                             "Track 5", "Track 6", "Track 7", "Track 8"};
    
    for (int i = 0; i < 8; ++i) {
        QPushButton* button = new QPushButton(this);
        button->setFont(font);
        button->setProperty("trackIndex", i);
        
        // Set button position (4x2 layout)
        int row = i / 4;
        int col = i % 4;
        button->setGeometry(100 + col * 260, 330 + row * 120, 240, 100);
        
        button->setStyleSheet(R"(
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: 4px solid #45a049;
                border-radius: 16px;
                font-size: 32px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
            QPushButton:pressed {
                background-color: #3e8e41;
            }
        )");
        
        connect(button, &QPushButton::clicked, this, &SettingPage::onKeyBindingButtonClicked);
        m_keyBindingButtons.append(button);
    }
}

void SettingPage::setupOffsetControls() {
    QFont font = Font::getDefaultFont();
    
    // Offset settings
    m_offsetLabel = new QLabel("Audio Offset:", this);
    m_offsetLabel->setStyleSheet("color: white; font-size: 48px;");
    m_offsetLabel->setFont(font);
    m_offsetLabel->setGeometry(100, 580, 400, 60);

    m_offsetSpinBox = new QSpinBox(this);
    m_offsetSpinBox->setGeometry(100, 660, 300, 80);
    m_offsetSpinBox->setRange(-200, 200);
    m_offsetSpinBox->setValue(m_gameConfig->getOffsetMs());
    m_offsetSpinBox->setSuffix(" ms");
    m_offsetSpinBox->setFont(font);
    m_offsetSpinBox->setStyleSheet(R"(
        QSpinBox {
            background-color: white;
            border: 4px solid #cccccc;
            border-radius: 8px;
            padding: 10px;
            font-size: 32px;
        }
        QSpinBox:focus {
            border: 4px solid #3daee9;
        }
    )");
    
    m_offsetValueLabel = new QLabel("(Earlier <-> Later)", this);
    m_offsetValueLabel->setStyleSheet("color: #cccccc; font-size: 32px;");
    m_offsetValueLabel->setFont(font);
    m_offsetValueLabel->setGeometry(440, 660, 400, 80);
    
    connect(m_offsetSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), 
            this, &SettingPage::onOffsetChanged);
}

void SettingPage::updateKeyBindingButtons() {
    QList<Qt::Key> keyBindings = m_gameConfig->getKeyBindings();
    
    for (int i = 0; i < m_keyBindingButtons.size() && i < keyBindings.size(); ++i) {
        QString keyName = QKeySequence(keyBindings[i]).toString();
        m_keyBindingButtons[i]->setText(QString("Track %1\n%2").arg(i + 1).arg(keyName));
    }
}

void SettingPage::onKeyBindingButtonClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        m_waitingForKeyButton = button;
        button->setText("Press Key...");
        button->setStyleSheet(R"(
            QPushButton {
                background-color: #FF9800;
                color: white;
                border: 4px solid #F57C00;
                border-radius: 16px;
                font-size: 32px;
                font-weight: bold;
            }
        )");
        
        // Set focus to receive keyboard events
        this->setFocus();
    }
}

void SettingPage::onOffsetChanged(int value) {
    m_gameConfig->setOffsetMs(value);
}

void SettingPage::keyPressEvent(QKeyEvent* event) {
    if (m_waitingForKeyButton) {
        Qt::Key key = static_cast<Qt::Key>(event->key());
        int trackIndex = m_waitingForKeyButton->property("trackIndex").toInt();
        
        // Check for key conflicts with other tracks
        bool conflictFound = false;
        for (int i = 0; i < 8; ++i) {
            if (i != trackIndex && m_gameConfig->getKeyForTrack(i) == key) {
                conflictFound = true;
                break;
            }
        }
        
        if (conflictFound) {
            QMessageBox::warning(this, "Key Conflict", 
                               QString("Key '%1' is already used by another track!")
                               .arg(QKeySequence(key).toString()));
        } else {
            m_gameConfig->setKeyBinding(trackIndex, key);
        }
        
        // Reset button state
        m_waitingForKeyButton->setStyleSheet(R"(
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border: 4px solid #45a049;
                border-radius: 16px;
                font-size: 32px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
            QPushButton:pressed {
                background-color: #3e8e41;
            }
        )");
        
        m_waitingForKeyButton = nullptr;
        updateKeyBindingButtons();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void SettingPage::saveAllSettings() {
    // Save all settings
    m_gameConfig->setMusicVolume(m_musicVolumeSlider->value() / 100.0f);
    m_gameConfig->setOffsetMs(m_offsetSpinBox->value());
    m_gameConfig->saveConfig();
    
    emit saveSettingsClicked();
}