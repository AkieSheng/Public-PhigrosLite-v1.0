#pragma once

#include <QObject>
#include <QSettings>
#include <QList>
#include <Qt>

class GameConfig : public QObject {
    Q_OBJECT
    Q_PROPERTY(float musicVolume READ musicVolume WRITE setMusicVolume NOTIFY musicVolumeChanged)
    
public:
    static GameConfig* instance();
    static const QList<Qt::Key> GAME_KEYS;

    // Audio setup
    float musicVolume() const { return m_musicVolume; }
    void setMusicVolume(float volume);
    
    // Key settings
    QList<Qt::Key> getKeyBindings() const { return m_keyBindings; }
    void setKeyBinding(int track, Qt::Key key);
    Qt::Key getKeyForTrack(int track) const;
    int getTrackForKey(Qt::Key key) const;
    
    // Game settings
    int getOffsetMs() const { return m_offsetMs; }
    void setOffsetMs(int offset);

    // Configuration file
    void loadConfig();
    void saveConfig();
    void resetToDefaults();
    
signals:
    void musicVolumeChanged(float volume);
    void keyBindingsChanged();
    void configChanged();
    
private:
    explicit GameConfig(QObject* parent = nullptr);
    static GameConfig* s_instance;
    
    QSettings* m_settings;
    
    // Configuration Data
    float m_musicVolume;
    QList<Qt::Key> m_keyBindings;
    int m_offsetMs;
    
    void setupDefaults();
};

