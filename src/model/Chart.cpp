#include "Chart.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void Chart::loadFromFile(const QString& filePath) {
    // 读入曲谱
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen()) {
        qWarning() << "无法打开谱面文件:" << filePath;
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    // Parse JSON data
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    QJsonObject root = doc.object();
    m_title = root["title"].toString();
    m_artist = root["artist"].toString();
    m_musicFile = root["musicFile"].toString();
    // Analyze musical notes
    QJsonArray notesArray = root["notes"].toArray();
    for (const QJsonValue& noteValue : notesArray) {
        QJsonObject noteObj = noteValue.toObject();
        Note::Type type = (noteObj["type"].toString() == "HOLD") ? Note::HOLD : Note::TAP;
        double time = noteObj["time"].toDouble();
        int track = noteObj["track"].toInt();
        Note note(type, time, track);
        if (type == Note::HOLD) {
            double duration = noteObj["duration"].toDouble();
            note.setHoldDuration(duration);
        }
        m_notes.append(note);
    }
    // Sort notes by time
    std::sort(m_notes.begin(), m_notes.end(), 
        [](const Note& a, const Note& b) { return a.getTime() < b.getTime(); });
}