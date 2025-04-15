#include "kuruk.h"
#include "ui_kuruk.h"

Kuruk::Kuruk(QWidget *parent)
    : QMainWindow{parent},
    ui(new Ui::kuruk),
    vyasa(new Vyasa(this)),
    shunya(new Shunya(this)),
    drona(new Drona(this)),
    vishnu(new Vishnu(this))
{
    ui->setupUi(this);
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
    connect(drona, &Drona::draw_graph, ui->kshetra, &Kshetra::handleGraph);
    // make sure vyasa is connected to drona AFTER kshetra, since kshetra updates
    // the bot position, and the new bot positions are used by drona
    // forums online say not to rely on the ordering of slots but, idgaf
    connect(vyasa, &Vyasa::recievedState, drona, &Drona::handleState);
    connect(ui->actionreset, &QAction::triggered, shunya, &Shunya::setup);
    connect(ui->actionHotMap, &QAction::triggered, ui->kshetra, &Kshetra::viewHotMap);
    connect(ui->actionAttack, &QAction::triggered, shunya, &Shunya::attack_setup);
    connect(ui->actionDefense, &QAction::triggered, shunya, &Shunya::defense_setup);
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
