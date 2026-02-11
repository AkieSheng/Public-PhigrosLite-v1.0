#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

class SelectPage : public QWidget {
    Q_OBJECT
public:
    explicit SelectPage(QWidget* parent = nullptr);

signals:
    void chartSelected(int index);
    void backToMenuClicked();

private slots:
    void onChartSelected(QListWidgetItem* item);
    void onGetChartTitles(const QStringList& titles);

private:
    QPushButton* m_backButton;
    QLabel* m_backgroundLabel;
    QListWidget* m_chartListWidget;
};
