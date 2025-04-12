#include "kuruk.h"
#include "ui_kuruk.h"

/**
 * @class Kuruk
 * @brief Main window class for the SSL simulator UI.
 *
 * This class sets up the main application window,
 * initializes the components such as Vyasa, Shunya, Drona, and Vishnu
 * and connects them through Qt signal-slot mechanism
 *
*/
Kuruk::Kuruk(QWidget *parent)
    : QMainWindow{parent},
    ui(new Ui::kuruk),
    vyasa(new Vyasa(this)),
    shunya(new Shunya(this)),
    drona(new Drona(this)),
    vishnu(new Vishnu(this))
{
    ui->setupUi(this);

    //! Shared pointers to lists of bots
    pandav = std::make_shared<std::vector<BlueBot>>();
    kaurav = std::make_shared<std::vector<YellowBot>>();

    //! Shared pointer to the ball
    ball = std::make_shared<Ball>(Qt::black, 5);

    //! Giving ownership of players and ball to kshetra and drona
    ui->kshetra->setPlayers(pandav, kaurav);
    ui->kshetra->setBall(ball);
    drona->setPlayers(pandav, kaurav);
    drona->setBall(ball);

    //! Connect Vyasa's state updates to Kshetra for visualization
    connect(vyasa, &Vyasa::recievedState, ui->kshetra, &Kshetra::handleState);
    //! Uncomment to visualize Voronoi graph from Drona in Kshetra
    //! connect(drona, &Drona::draw_graph, ui->kshetra, &Kshetra::handleGraph);


    /** Connect Vyasa to Drona AFTER Kshetra, since Kshetra updates
     *  the bot positions, and the new bot positions are used by Drona
     *
    */
    connect(vyasa, &Vyasa::recievedState, drona, &Drona::handleState);

    //! Connect UI actions to Shunya and Kshetra slots
    connect(ui->actionreset, &QAction::triggered, shunya, &Shunya::setup);
    connect(ui->actionHotMap, &QAction::triggered, ui->kshetra, &Kshetra::viewHotMap);
    connect(ui->actionAttack, &QAction::triggered, shunya, &Shunya::attack_setup);
    connect(ui->actionDefense, &QAction::triggered, shunya, &Shunya::defense_setup);
}

/**
 * @brief Destructor for Kuruk.
 *
 * Deallocate all dynamically allocated memory and do cleanup for
 * UI and strategy modules and clears pointers.
 *
 * */


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
