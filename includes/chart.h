#ifndef CHART_H
#define CHART_H

#include <KDChartChart>
#include <QWidget>

class QMenu;
class QSortFilterProxyModel;
class ChartSettingsDialog;
class ChartSettings;


class Chart: public KDChart::Chart
{
    Q_OBJECT
public:
    Chart(QWidget* parent, QSortFilterProxyModel *proxy);
    void ShowContextMenu(const QPoint &pos);
    void initContextMenu();
    void deleteDiagram();
    void chartSettings();
private:
    QMenu* m_context_menu;
    ChartSettings* m_settings;
};

#endif // CHART_H
