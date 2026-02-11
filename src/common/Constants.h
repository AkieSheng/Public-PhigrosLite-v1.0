#pragma once

#include <QList>
#include <QString>
#include <Qt>

class GameConstants {
public:
    static constexpr int WINDOW_WIDTH = 1200;
    static constexpr int WINDOW_HEIGHT = 800;
    static constexpr int TARGET_FPS = 60;

    static constexpr int JUDGE_LINE_Y = 600;
    static constexpr int TRACK_WIDTH = 120;
    static constexpr int TRACK_START_X = 120;
    static constexpr int NOTE_HEIGHT = 10;
    static constexpr int NOTE_SPEED = 300;
    
    // File Path
    static const QString CHARTS_DIR;
    static const QString MUSIC_DIR;
    static const QString CONFIG_FILE;
};


