#include "SelectPage.h"
#include "Constants.h"
#include "Font.h"

SelectPage::SelectPage(QWidget* parent)
    : QWidget(parent) {

    QFont font = Font::getDefaultFont();

    m_backgroundLabel = new QLabel(this);
    m_backgroundLabel->setPixmap(QPixmap(":/images/select_background.jpg"));
    m_backgroundLabel->setScaledContents(true);
    m_backgroundLabel->resize(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT);
    
    m_backButton = new QPushButton("Back", this);
    m_backButton->setFont(font);
    m_backButton->setGeometry(20, 20, 100, 40);
    connect(m_backButton, &QPushButton::clicked, this, [this]() {
        emit backToMenuClicked();
    });

    m_chartListWidget = new QListWidget(this);
    m_chartListWidget->setFont(font);
    m_chartListWidget->setGeometry(300, 100, 600, 600);
    m_chartListWidget->setStyleSheet(R"(
        QListWidget {
            background-color: rgba(255, 255, 255, 180);  
            border: 2px solid #888;
            border-radius: 10px;
            padding: 5px;
        }
        QListWidget::item {
            padding: 10px;
            margin: 5px;
            border-radius: 5px;
            background: transparent;
            border-bottom: 2px solid gray;
        }
        QListWidget::item:selected {
            background-color: rgba(255, 255, 255, 180);
            color: black;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 5px 0;
        }
        QScrollBar::handle:vertical {
            background: #aaa;
            border-radius: 4px;
        }
    )");

    connect(m_chartListWidget, &QListWidget::itemClicked, this, &SelectPage::onChartSelected);

}

void SelectPage::onChartSelected(QListWidgetItem* item) {
    int index = m_chartListWidget->row(item);
    emit chartSelected(index);
}

void SelectPage::onGetChartTitles(const QStringList& titles) {
    m_chartListWidget->clear();
    for (const QString& title : titles) {
        m_chartListWidget->addItem(new QListWidgetItem(title));
    }
}