#pragma once

#include <GameScore.h>
class Note {
public:
    enum Type { TAP, HOLD };
    
    Note(Type type, double time, int track) : m_type(type), m_time(time), m_track(track) {};
    
    Type getType() const { return m_type; }
    double getTime() const { return m_time; }
    int getTrack() const { return m_track; }
    double getHoldDuration() const { return m_holdDuration; }
    bool isActive() const { return m_isActive; }
    bool isPressed() const { return m_isPressed; }
    int getY() const { return m_y; }
    int getHeight() const { return height; }

    void setHoldDuration(double duration) { m_holdDuration = duration; }
    void setActive(bool active) { m_isActive = active; }
    void setPressed(bool pressed) { m_isPressed = pressed; }
    void setY(int y) { m_y = y; }
    void setHeight(int h) { height = h; }

private:
    Type m_type;
    double m_time;                  // hit time(s)
    int m_track;                    // Track (0-7 corresponds to ASDFJKL;)
    double m_holdDuration;          // HOLD note duration
    bool m_isActive = true;         // Can it be hit (true means yes, false means hit or timed out)
    bool m_isPressed = false;       // Is it pressed (only valid for HOLD notes)

    int m_y = 0;                    // Coordinates at the bottom of the note
    int height = 50;                // Note height
};
