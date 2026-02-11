#include "GameConfig.h"
#include "Constants.h"
#include <QStandardPaths>

GameConfig* GameConfig::s_instance = nullptr;

GameConfig* GameConfig::instance() {
    if (!s_instance) {
        s_instance = new GameConfig();
    }
    return s_instance;
}

GameConfig::GameConfig(QObject* parent)
    : QObject(parent)
    , m_musicVolume(0.8f)
    , m_offsetMs(0)
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    m_settings = new QSettings(configPath + "/phigros_game.ini", QSettings::IniFormat, this);
    
    setupDefaults();
    loadConfig();
}

void GameConfig::setupDefaults() {
    m_keyBindings = GAME_KEYS;
}

void GameConfig::loadConfig() {
    m_musicVolume = m_settings->value("audio/musicVolume", 0.8f).toFloat();
    m_offsetMs = m_settings->value("game/offsetMs", 0).toInt();
    
    // Load button
    m_settings->beginReadArray("controls/keys");
    for (int i = 0; i < 8; ++i) {
        m_settings->setArrayIndex(i);
        int keyValue = m_settings->value("key", GAME_KEYS[i]).toInt();
        if (i < m_keyBindings.size()) {
            m_keyBindings[i] = static_cast<Qt::Key>(keyValue);
        }
    }
    m_settings->endArray();
}

void GameConfig::saveConfig() {
    m_settings->setValue("audio/musicVolume", m_musicVolume);
    m_settings->setValue("game/offsetMs", m_offsetMs);
    
    // Save button
    m_settings->beginWriteArray("controls/keys");
    for (int i = 0; i < m_keyBindings.size(); ++i) {
        m_settings->setArrayIndex(i);
        m_settings->setValue("key", static_cast<int>(m_keyBindings[i]));
    }
    m_settings->endArray();
    
    m_settings->sync();
    emit configChanged();
}

void GameConfig::setMusicVolume(float volume) {
    volume = qBound(0.0f, volume, 1.0f);
    if (m_musicVolume != volume) {
        m_musicVolume = volume;
        emit musicVolumeChanged(volume);
    }
}

void GameConfig::setKeyBinding(int track, Qt::Key key) {
    if (track >= 0 && track < m_keyBindings.size()) {
        m_keyBindings[track] = key;
        emit keyBindingsChanged();
    }
}

Qt::Key GameConfig::getKeyForTrack(int track) const {
    if (track >= 0 && track < m_keyBindings.size()) {
        return m_keyBindings[track];
    }
    return Qt::Key_unknown;
}

int GameConfig::getTrackForKey(Qt::Key key) const {
    return m_keyBindings.indexOf(key);
}

void GameConfig::setOffsetMs(int offset) {
    m_offsetMs = offset;
}

void GameConfig::resetToDefaults() {
    setupDefaults();
    m_musicVolume = 0.8f;
    m_offsetMs = 0;
    
    emit musicVolumeChanged(m_musicVolume);
    emit keyBindingsChanged();
    emit configChanged();
}

const QList<Qt::Key> GameConfig::GAME_KEYS = {
    Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_F,
    Qt::Key_J, Qt::Key_K, Qt::Key_L, Qt::Key_Semicolon
};