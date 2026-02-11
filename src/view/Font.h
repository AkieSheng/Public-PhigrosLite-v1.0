#pragma once

#include <QFont>

class Font {
public:
    static QFont getDefaultFont() {
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(16);
        return font;
    }

    static QFont getTitleFont() {
        QFont font = getDefaultFont();
        font.setPointSize(32);
        return font;
    }

    static QFont getGamingFont() {
        QFont font = getDefaultFont();
        font.setPointSize(10);
        return font;
    }
};