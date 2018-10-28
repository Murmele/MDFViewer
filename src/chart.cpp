#include "chart.h"
#include <KDChartGridAttributes>
#include <KDChartFrameAttributes>
#include <KDChartTextAttributes>
#include <KDChartDataValueAttributes>
#include <KDChartMarkerAttributes>
#include <KDChartPlotter>
#include <KDChartLegend>
#include <KDChartBackgroundAttributes>
#include <QSortFilterProxyModel>

#include <QMenu>
#include "chartsettingsdialog.h"
#include "chartsettings.h"

Chart::Chart(QWidget *parent,QSortFilterProxyModel* proxy): KDChart::Chart(parent)
{

    m_settings = new ChartSettings();

    setFocusPolicy(Qt::ClickFocus);

    KDChart::BackgroundAttributes chart_bg;
    chart_bg.setBrush(Qt::white);
    chart_bg.setVisible(true);
    setBackgroundAttributes(chart_bg);

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

    KDChart::AbstractCoordinatePlane * plane1 = coordinatePlane();
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


    initContextMenu();
}

void Chart::ShowContextMenu(const QPoint &pos){
    QPoint global = this->mapToGlobal(pos);
    m_context_menu->exec(global);
}

void Chart::initContextMenu(){

    m_context_menu = new QMenu();

    QAction* delete_diagram = new QAction(tr("Delete diagram"));
    connect(delete_diagram, &QAction::triggered, this, &Chart::deleteDiagram);
    m_context_menu->addAction(delete_diagram);

    QAction* diagram_settings = new QAction(tr("Chart settings"));
    connect(diagram_settings, &QAction::triggered, this, &Chart::chartSettings);
    m_context_menu->addAction(diagram_settings);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,this,&Chart::ShowContextMenu);
}

void Chart::deleteDiagram(){
    deleteLater();
}

void Chart::chartSettings(){
    ChartSettingsDialog settings_dialog(m_settings,this);

    settings_dialog.exec();
}
