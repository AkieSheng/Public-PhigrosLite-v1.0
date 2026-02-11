#pragma once

#include <QPointF>
#include <QElapsedTimer>
#include <QColor>

class HitEffect {
public:
    HitEffect(const QPointF& pos, QColor color = Qt::white, float maxradius = 50.0f, float opacity = 1.0f, int lifetime = 300)
        : position(pos), color(color), maxradius(maxradius), opacity(opacity), lifetime(lifetime) {
        timer.start();
        radius = 0;
    }

    void update() {
        if (timer.elapsed() > lifetime) {
            opacity = 0; // Effect has expired
        } else {
            opacity = 1.0 - static_cast<float>(timer.elapsed()) / lifetime;
            radius = maxradius * (1.0 - static_cast<float>(timer.elapsed()) / lifetime);
        }
    }

    bool isExpired() const {
        return timer.elapsed() > lifetime;
    }

    void setLifetime(int newLifetime) {
        lifetime = newLifetime;
        timer.restart();
    }

    QPointF getPosition() const { return position; }
    float getRadius() const { return radius; }
    float getOpacity() const { return opacity; }
    QColor getColor() const { return color; }

private:
    QPointF position;
    float maxradius;
    float radius;
    float opacity;
    int lifetime;
    QElapsedTimer timer;
    QColor color;
};
