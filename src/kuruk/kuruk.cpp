#include "kuruk.h"
#include "ui_kuruk.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QDockWidget> 
#define LOG qDebug() << "[kuruk] : "

QLabel* Kuruk::robotIdLabel = nullptr;
QLabel* Kuruk::robotPositionLabel = nullptr;
QLabel* Kuruk::robotOrientationLabel = nullptr;

Kuruk::Kuruk(QWidget *parent)
    : QMainWindow{parent},
    ui(new Ui::kuruk),
    vyasa(new Vyasa(this)),
    shunya(new Shunya(this)),
    drona(new Drona(this)),
    vishnu(new Vishnu(this))
{
    ui->setupUi(this);
    sidebar = new QWidget(this);
    sidebarDockWidget = new QDockWidget("Robot Info", this);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);

    Kuruk::robotIdLabel = new QLabel("ID: ", this);
    Kuruk::robotPositionLabel = new QLabel("Position: ", this);
    Kuruk::robotOrientationLabel = new QLabel("Orientation: ", this);

    sidebarLayout->addWidget(Kuruk::robotIdLabel);
    sidebarLayout->addWidget(Kuruk::robotPositionLabel);
    sidebarLayout->addWidget(Kuruk::robotOrientationLabel);
    sidebarLayout->addStretch();
    sidebar->setLayout(sidebarLayout);

    sidebarDockWidget->setWidget(sidebar);  // Set the sidebar widget to the dock widget
    sidebarDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);  // Allow docking on left or right
    sidebarDockWidget->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);  // Add features like closable and movable
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDockWidget);
    
    pandav = std::make_shared<std::vector<BlueBot>>();
    kaurav = std::make_shared<std::vector<YellowBot>>();
    ball = std::make_shared<Ball>(Qt::black, 5);

    // giving ownership of players and ball to kshetra and drona
    ui->kshetra->setPlayers(pandav, kaurav);
    ui->kshetra->setBall(ball);
    ui->kshetra->setFixedSize(1280, 720);  // hardcoded override
    drona->setPlayers(pandav, kaurav);
    drona->setBall(ball);

    connect(vyasa, &Vyasa::recievedState, ui->kshetra, &Kshetra::handleState);
    // voronoi graph
    // connect(drona, &Drona::draw_graph, ui->kshetra, &Kshetra::handleGraph);
    // make sure vyasa is connected to drona AFTER kshetra, since kshetra updates
    // the bot position, and the new bot positions are used by drona
    // forums online say not to rely on the ordering of slots but, idgaf
    connect(vyasa, &Vyasa::recievedState, drona, &Drona::handleState);
    connect(ui->actionreset, &QAction::triggered, shunya, &Shunya::setup);
    connect(ui->actionHotMap, &QAction::triggered, ui->kshetra, &Kshetra::viewHotMap);
    connect(ui->actionAttack, &QAction::triggered, shunya, &Shunya::attack_setup);
    connect(ui->actionDefense, &QAction::triggered, shunya, &Shunya::defense_setup);
    connect(ui->kshetra, &Kshetra::robotSelected, this, &Kuruk::updateSidebar);
}


Kuruk::~Kuruk(){
    delete ui;
    delete vyasa;
    delete shunya;
    delete drona;
    ui = nullptr;
    vyasa = nullptr;
    shunya = nullptr;
    drona = nullptr;
}

void Kuruk::updateSidebar(int id, QPointF position, float orientation) {

    // Assuming your sidebar is in a QWidget or custom widget:
    qDebug() << "Slot hit: Robot ID:" << id << "at" << position << "orientation" << orientation;
    Kuruk::robotIdLabel->setText(QString("ID: %1").arg(id));              // Update the robot ID
    Kuruk::robotPositionLabel->setText(QString("Position: (%1, %2)").arg(position.x()).arg(position.y())); // Update position
    Kuruk::robotOrientationLabel->setText(QString("Orientation: %1").arg(orientation));  // Update orientation
}
