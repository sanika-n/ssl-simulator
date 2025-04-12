/**
 * The simulator is divided into various components.
 * If you are versed with mythology you may be able to
 * guess each components purpose.
 *
 * KuruK: The MainWindow of the application.
 * parent of all components. All components
 * are constructed here.
 *
 */

#ifndef KURUK_H
#define KURUK_H

#include <QMainWindow>
#include "ui_kuruk.h"
#include "vyasa/vyasa.h"
#include "shunya/shunya.h"
#include "drona/drona.h"
#include "yodha/yodha.h"
#include "vishnu/vishnu.h"

/**
 *
 * @class Kuruk
 * @brief The main window class for the SSL simulator.
 *
 * Acts as the central controller for the application.
 * Responsible for constructing and initializing major simulation components such as
 * Vyasa, Shunya, Drona and Vishnu. It also manages shared game elements like players and ball.
 * */

class Kuruk : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Constructor for Kuruk.
     * @param parent THe parent widget (usually nullptr for main window).
     */
    explicit Kuruk(QWidget *parent = nullptr);

    /**
     * @brief Pointer to Vyasa component
     * Used to receive game state updates
     * */
    Vyasa *vyasa;

    /**
     * @brief Destructor for Kuruk.
     * Deallocates dynamically allocated resources
     * */
    ~Kuruk();

private:
    //! Qt UI object
    Ui::kuruk *ui;

    //! Pointer to Shunya
    Shunya *shunya;

    //! Pointer to Drona
    Drona *drona;

    //! Pointer to Vishnu
    Vishnu *vishnu;

    //! Shared pointer to the list of blue team bots
    std::shared_ptr<std::vector<BlueBot>> pandav;

    //! Shared pointer to the list of yellow team bots
    std::shared_ptr<std::vector<YellowBot>> kaurav;

    //! Shared pointer to the ball objects
    std::shared_ptr<Ball> ball;
signals:
    // void configureBots();
};

#endif // KURUK_H
