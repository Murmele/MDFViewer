#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDockWidget>
#include <QSettings>
#include <QFileDialog>
#include <QListView>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "selectmdfproperties.h"
#include "dialognewtab.h"

#include <KDChartChart>
#include <KDChartGridAttributes>
#include <KDChartFrameAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartPlotter>
#include <KDChartLegend>
#include <KDChartBackgroundAttributes>

#include <QWidget>
#include <QStandardItemModel>

#include <signalpreviewproxymodel.h>
#include <signalnamesproxymodel.h>
#include "chart.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_data(this)
{
    ui->setupUi(this);


    setupCentralWidget();
    initMenubar();
    initDockWidgets();
    initContextMenuDataFileListView();
    initContextMenuTabWidget();

    m_last_data_file_path = "";

}

void MainWindow::setupCentralWidget(){
    QVBoxLayout* m_vlayout = new QVBoxLayout(this);

    m_tabwidget_plots = new QTabWidget(this);
    m_tabwidget_plots->setTabsClosable(true);
    m_tabwidget_plots->setMovable(true);
    connect(m_tabwidget_plots, &QTabWidget::tabCloseRequested, this, &MainWindow::tabCloseRequested);

    //QWidget* w = new QWidget(this);
    //QVBoxLayout* layout = new QVBoxLayout();
    //QPushButton* button = new QPushButton("Test");
    //layout->addWidget(button);
    //w->setLayout(layout);

    //m_tabwidget_plots->addTab(w,"Test");
    //QPushButton* but2 = qobject_cast<QPushButton*>(m_tabwidget_plots->currentWidget()->layout()->itemAt(0)->widget());
    m_vlayout->addWidget(m_tabwidget_plots);
    ui->centralWidget->setLayout(m_vlayout);
}

void MainWindow::tabCloseRequested(int index){

    QWidget* a = m_tabwidget_plots->currentWidget();
    int b = 5;


    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this,tr("Close plot area"),tr("Do you want to close this plot area?"),QMessageBox::No| QMessageBox::Yes);

    if (reply == QMessageBox::Yes){
        m_tabwidget_plots->removeTab(index);
        a->close();
    }
}

void MainWindow::addDiagram(){

    SignalPreviewProxyModel* proxy = new SignalPreviewProxyModel(this);
    proxy->setSourceModel(m_data.getSignalModel());
    Chart* chart = new Chart(m_tabwidget_plots,proxy);

    m_tabwidget_plots->currentWidget()->layout()->addWidget(chart);
    chart->show();
}

void MainWindow::initContextMenuTabWidget(){

    m_context_menu_tabwidget = new QMenu();
    QAction* new_tab =new QAction(tr("New tab"),this);
    connect(new_tab, &QAction::triggered, this, &MainWindow::newTab);
    m_context_menu_tabwidget->addAction(new_tab);

    QAction* add_diagram = new QAction(tr("Add diagram"));
    connect(add_diagram, &QAction::triggered, this, &MainWindow::addDiagram);
    m_context_menu_tabwidget->addAction(add_diagram);

    m_tabwidget_plots->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tabwidget_plots, &QWidget::customContextMenuRequested,this,&MainWindow::ShowContextMenuTabWidget);
}

void MainWindow::newTab(){
    QWidget* w = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout();
    w->setLayout(layout);

    //open dialog and return return value
    DialogNewTab* dialog = new DialogNewTab(this);
    if (dialog->exec() == QDialog::Accepted){
        QString tab_name= dialog->getTabName();
        delete dialog;

        m_tabwidget_plots->addTab(w,tab_name);
    }

}

void MainWindow::ShowContextMenuTabWidget(const QPoint &pos){

    if(m_tabwidget_plots->count()<1){ // if no tab exist, not possible to create diagram
        m_context_menu_tabwidget->actions().at(1)->setVisible(false);
    }else{
        m_context_menu_tabwidget->actions().at(1)->setVisible(true);
    }


//    // works because setFocusPolicy(Qt::ClickFocus); is in the child
//    if(qobject_cast<KDChart::Chart*>(focusWidget())!=nullptr){
//        // delete diagram option
//        m_context_menu_tabwidget->actions().at(2)->setVisible(true);
//    }else{
//        m_context_menu_tabwidget->actions().at(2)->setVisible(false);
//    }

    QPoint global = m_tabwidget_plots->mapToGlobal(pos);
    m_context_menu_tabwidget->exec(global);
}

void MainWindow::initContextMenuDataFileListView(){
    m_context_menu = new QMenu();
    QAction* activate_file =new QAction(tr("Activate file"),this);
    connect(activate_file, &QAction::triggered, this, &MainWindow::readDataFile);
    m_context_menu->addAction(activate_file);
    QAction* remove_file =new QAction(tr("Remove file"),this);
    connect(remove_file, &QAction::triggered, this, &MainWindow::removeDataFile);
    m_context_menu->addAction(remove_file);

    m_listview_signal_files->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listview_signal_files, &QWidget::customContextMenuRequested,this,&MainWindow::ShowContextMenuListView);
}

void MainWindow::ShowContextMenuListView(const QPoint &pos){

    if(m_data.getFilePaths()->rowCount(QModelIndex()) < 1){
        return;
    }

    if(m_listview_signal_files->selectionModel()->selectedRows().length()<1){
        return;
    }

    QPoint global = m_listview_signal_files->mapToGlobal(pos);
    m_context_menu->exec(global);
}

void MainWindow::readDataFile(){
    int row = m_listview_signal_files->selectionModel()->selectedRows()[0].row();

    m_data.readDataFile(row);
}

void MainWindow::removeDataFile(){
    int row = m_listview_signal_files->selectionModel()->selectedIndexes()[0].row();

    m_data.removeDataFile(row);
}

// so only one function is responsible for all docks, instead of a function per dock
void MainWindow::changeDockVisibility(bool checked){
    Q_UNUSED(checked);
    m_ui_signals->setVisible(m_show_ui_signals->isChecked());
    m_ui_signals_operations->setVisible(m_show_ui_signals_operations->isChecked());
    m_ui_signals_preview->setVisible(m_show_ui_signals_preview->isChecked());
    m_ui_signal_files->setVisible(m_show_ui_signal_files->isChecked());
}

void MainWindow::changedDockVisibility(bool visible){
    Q_UNUSED(visible);
    m_show_ui_signals->setChecked(m_ui_signals->isVisible());
    m_show_ui_signals_operations->setChecked(m_ui_signals_operations->isVisible());
    m_show_ui_signals_preview->setChecked(m_ui_signals_preview->isVisible());
    m_show_ui_signal_files->setChecked(m_ui_signal_files->isVisible());
}

void MainWindow::initMenubar(){

    // Begin Menu "File"
    m_menu_view = ui->menuBar->addMenu(tr("File"));
    m_import_data = new QAction(tr("Import data file"),this);
    m_menu_view->addAction(m_import_data);
    connect(m_import_data, &QAction::triggered, this, &MainWindow::importDataFile);
    // End Menu "File"

    // Begin Menu "View"
    m_menu_view = ui->menuBar->addMenu(tr("View"));

    m_show_ui_signals = new QAction(tr("Show signals"),this);
    m_show_ui_signals->setCheckable(true);
    m_show_ui_signals->setChecked(true);
    m_menu_view->addAction(m_show_ui_signals);
    connect(m_show_ui_signals, &QAction::toggled, this, &MainWindow::changeDockVisibility);

    m_show_ui_signals_operations = new QAction(tr("Show signal operations"),this);
    m_show_ui_signals_operations->setCheckable(true);
    m_show_ui_signals_operations->setChecked(true);
    m_menu_view->addAction(m_show_ui_signals_operations);
    connect(m_show_ui_signals_operations, &QAction::toggled, this, &MainWindow::changeDockVisibility);

    m_show_ui_signals_preview = new QAction(tr("Show preview"),this);
    m_show_ui_signals_preview->setCheckable(true);
    m_show_ui_signals_preview->setChecked(true);
    m_menu_view->addAction(m_show_ui_signals_preview);
    connect(m_show_ui_signals_preview, &QAction::toggled, this, &MainWindow::changeDockVisibility);

    m_show_ui_signal_files = new QAction(tr("Show signal files"),this);
    m_show_ui_signal_files->setCheckable(true);
    m_show_ui_signal_files->setChecked(true);
    m_menu_view->addAction(m_show_ui_signal_files);
    connect(m_show_ui_signal_files, &QAction::toggled, this, &MainWindow::changeDockVisibility);
    // End Menu "View"


}

void MainWindow::setupPreviewChart(){
    KDChart::Chart* chart= new KDChart::Chart(m_ui_signals_preview);

    KDChart::BackgroundAttributes chart_bg;
    chart_bg.setBrush(Qt::white);
    chart_bg.setVisible(true);
    chart->setBackgroundAttributes(chart_bg);

    // coordinate plane setup
//    QStandardItemModel* model = new QStandardItemModel(this);

//    model->setRowCount(100);
//    model->setColumnCount(6); // 3 data sets
//    for (int i=0; i<100; ++i) {
//        // dateset 1
//        qreal t = i;
//        qreal v = qreal(i)*i;
//        QModelIndex index = model->index( i, 0 );
//        model->setData( index, QVariant( t ) );
//        index = model->index( i, 1 );
//        model->setData( index, QVariant( v ) );

//        // dateset 2
//        t = i*2;
//        v = qreal(i)*i-i;
//        index = model->index( i, 2 );
//        model->setData( index, QVariant( t ) );
//        index = model->index( i, 3 );
//        model->setData( index, QVariant( v ) );

//        // dateset 3
//        t = qreal(i)/2;
//        v = qreal(i)*i+i;
//        index = model->index( i, 4 );
//        model->setData( index, QVariant( t ) );
//        index = model->index( i, 5 );
//        model->setData( index, QVariant( v ) );
//    }

//    model->setHeaderData( 0, Qt::Horizontal, "Dataset 1" );
//    model->setHeaderData( 2, Qt::Horizontal, "Dataset 2" );
//    model->setHeaderData( 4, Qt::Horizontal, "Dataset 3" );

    SignalPreviewProxyModel* proxy = new SignalPreviewProxyModel(this);
    proxy->setSourceModel(m_data.getSignalModel());


    KDChart::AbstractCoordinatePlane * plane1 = chart->coordinatePlane();
    plane1->setRubberBandZoomingEnabled(false);

    // create cartesian diagrams
    KDChart::Plotter * plotter = new KDChart::Plotter;
    plotter->setHidden( 0, true );
    plotter->setAntiAliasing(false);
    plotter->setModel( proxy ); //model
    plane1->replaceDiagram( plotter );

    // axis
    KDChart::CartesianAxis * xAxis = new KDChart::CartesianAxis( plotter );
    xAxis->setPosition( KDChart::CartesianAxis::Bottom );
    xAxis->setTitleText("X-Title");

    KDChart::TextAttributes att = xAxis->titleTextAttributes();
    QFont f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize( KDChart::Measure(16) );
    xAxis->setTitleTextAttributes(att);

    KDChart::CartesianAxis * y1Axis = new KDChart::CartesianAxis( plotter );
    y1Axis->setPosition( KDChart::CartesianAxis::Left );
    y1Axis->setTitleText("Y-Title");

    att = y1Axis->titleTextAttributes();
    f = att.font();
    f.setBold(true);
    att.setFont(f);
    att.setAutoShrink(true);
    att.setFontSize( KDChart::Measure(16) );
    y1Axis->setTitleTextAttributes(att);


    // add the axis to the plotter
    plotter->addAxis( xAxis );
    plotter->addAxis( y1Axis );





    m_ui_signals_preview->setWidget(chart);
    chart->show();
}

void MainWindow::initDockWidgets(){
    m_ui_signals = new QDockWidget(tr("Signals"),this);
    m_listview_signal = new QListView(this);
    SignalNamesProxyModel* proxy_signal_names = new SignalNamesProxyModel(this);
    proxy_signal_names->setSourceModel(m_data.getSignalModel());
    m_listview_signal->setModel(proxy_signal_names); //m_data.getSignalNames());
    m_listview_signal->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui_signals->setWidget(m_listview_signal);

    m_ui_signals_operations = new QDockWidget(tr("Signal Operations"),this);

    m_ui_signals_preview = new QDockWidget(tr("Preview"),this);
    setupPreviewChart();

    m_ui_signal_files = new QDockWidget(tr("Signal Files"),this);
    m_listview_signal_files = new QListView(this);
    m_listview_signal_files->setModel(m_data.getFilePaths());
    m_ui_signal_files->setWidget(m_listview_signal_files);

    connect(m_ui_signals, &QDockWidget::visibilityChanged, this,&MainWindow::changedDockVisibility);
    connect(m_ui_signals_operations, &QDockWidget::visibilityChanged, this,&MainWindow::changedDockVisibility);
    connect(m_ui_signals_preview, &QDockWidget::visibilityChanged, this,&MainWindow::changedDockVisibility);
    connect(m_ui_signal_files, &QDockWidget::visibilityChanged, this,&MainWindow::changedDockVisibility);


    if(!readSettings()){
        // set dockwidgets position initially
        addDockWidget(Qt::LeftDockWidgetArea,m_ui_signal_files);
        addDockWidget(Qt::LeftDockWidgetArea,m_ui_signals);
        addDockWidget(Qt::LeftDockWidgetArea,m_ui_signals_preview);
        addDockWidget(Qt::RightDockWidgetArea,m_ui_signals_operations);

        m_ui_signal_files->setVisible(true);
        m_ui_signals->setVisible(true);
        m_ui_signals_preview->setVisible(true);
        m_ui_signals_operations->setVisible(true);

    }
}

bool MainWindow::readSettings(){
    int value = 0;

    QSettings settings("MyCompany", "TestQDockWidgets");
    settings.beginGroup("mainWindow");
    value = restoreGeometry(settings.value("geometry").toByteArray());
    value += restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();

    if(value == 0){
        return 0;
    }
    return 0; // musst be set to 1!
}

void MainWindow::importDataFile(){
   QString path = QFileDialog::getOpenFileName(this,tr("Open Data File"),m_last_data_file_path,tr("Data File (*.mf4)"));

   if(path.compare("") > 0){
        m_last_data_file_path = path;
        m_data.addDataFile(m_last_data_file_path);
   }
}

QPair<int,int> MainWindow::getMDFFileSettings(QVector<int> gc){
    //open dialog and return return value
    SelectMDFProperties* dialog = new SelectMDFProperties(gc,this);
    if (dialog->exec() == QDialog::Accepted){
        QPair<int,int> properties = dialog->getProperties();
        delete dialog;
        return properties;
    }

    delete dialog;
    return QPair<int,int>(-1,-1);
}

MainWindow::~MainWindow()
{
    QSettings settings("MyCompany", "TestQDockWidgets");
    settings.beginGroup("mainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();
    delete ui;
}
