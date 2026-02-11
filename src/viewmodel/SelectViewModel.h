#pragma once

#include <QObject>
#include <QStringList>
#include "Chart.h"

class SelectViewModel : public QObject {
    Q_OBJECT
    
public:
    explicit SelectViewModel(QObject* parent = nullptr);
    ~SelectViewModel() = default;

    QStringList getChartPaths() const { return m_chartPaths; }
    QStringList getChartTitles() const { return m_chartTitles; }
    QStringList getMusicPaths() const { return m_musicPaths; }

public slots:
    void loadAvailableCharts();
    
signals:
    void sendChartPaths(QStringList chartPaths);
    void sendChartTitles(QStringList chartTitles);
    void sendMusicPaths(QStringList musicPaths);

private:
    QStringList m_chartPaths;
    QStringList m_chartTitles;
    QStringList m_musicPaths;
};