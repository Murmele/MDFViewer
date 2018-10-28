#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "data.h"

class QListView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool readSettings();
    bool writeSettings();
    void initMenubar();
    void initDockWidgets();
    void initContextMenuDataFileListView();
    void initContextMenuTabWidget();
    void setupCentralWidget();
    void setupPreviewChart();
    QPair<int,int> getMDFFileSettings(QVector<int> gc);
    ~MainWindow();

private slots:
    void changeDockVisibility(bool checked);
    void changedDockVisibility(bool visible);
    void importDataFile();
    void ShowContextMenuListView(const QPoint &pos);
    void ShowContextMenuTabWidget(const QPoint &pos);
    void removeDataFile();
    void readDataFile();
    void tabCloseRequested(int index);
    void newTab();
    void addDiagram();

private:
    Data m_data;
    QString m_last_data_file_path;

    // context menu
    QMenu* m_context_menu;
    QMenu* m_context_menu_tabwidget;

    // ui stuff
    Ui::MainWindow *ui;
    QDockWidget *m_ui_signals;
    QListView* m_listview_signal;

    QDockWidget* m_ui_signals_preview;
    QDockWidget* m_ui_signals_operations;

    QDockWidget* m_ui_signal_files;
    QListView* m_listview_signal_files;

    QMenu* m_menu_file;
    QAction* m_import_data;

    QMenu* m_menu_view;
    QAction* m_show_ui_signals, *m_show_ui_signals_preview, *m_show_ui_signals_operations, *m_show_ui_signal_files;

    QTabWidget* m_tabwidget_plots;

};

#endif // MAINWINDOW_H
