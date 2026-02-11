#include "SelectViewModel.h"
#include "Constants.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

SelectViewModel::SelectViewModel(QObject* parent)
    : QObject(parent)
{
    loadAvailableCharts();
}

void SelectViewModel::loadAvailableCharts()
{
    qWarning() << "Loading available charts...";
    m_chartPaths.clear();
    m_chartTitles.clear();
    
    QDir chartsDir(GameConstants::CHARTS_DIR);
    if (!chartsDir.exists()) {
        qWarning() << "Charts directory not found!";
        return;
    }
    
    QStringList filters;
    filters << "*.json";
    chartsDir.setNameFilters(filters);
    
    QFileInfoList chartFiles = chartsDir.entryInfoList();
    for (const QFileInfo& fileInfo : chartFiles) {
        QString filePath = fileInfo.absoluteFilePath();
        
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open chart file:" << filePath;
            continue;
        }
        QByteArray data = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse false:" << error.errorString() << "file:" << filePath;
            continue;
        }
        
        QJsonObject root = doc.object();
        QString title = root["title"].toString();
        QString musicPath = root["music"].toString();
        if (title.isEmpty()) {
            title = fileInfo.baseName();
        }
        
        m_chartPaths.append(filePath);
        m_chartTitles.append(title);
    }

    emit sendChartPaths(m_chartPaths);
    emit sendChartTitles(m_chartTitles);
    emit sendMusicPaths(m_musicPaths);
}