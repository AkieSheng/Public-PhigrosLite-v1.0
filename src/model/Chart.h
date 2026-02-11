#pragma once

#include <algorithm>
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include "Note.h"

class Chart {
public:
    void loadFromFile(const QString& filePath);
    QList<Note>& getNotes() { return m_notes; }
    const QList<Note>& getNotes() const { return m_notes; }
    const QString& getMusicFile() const { return m_musicFile; }
    const QString& getTitle() const { return m_title; }
    
private:
    QList<Note> m_notes;
    QString m_musicFile;
    QString m_title;
    QString m_artist;
};