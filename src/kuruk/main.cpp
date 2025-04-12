#include "kuruk.h"
#include <QApplication>

/**
 * @file main.cpp
 * @brief Entry point
 *
 * Initialises the Qt application and creates the main window ('Kuruk)
 * */


/**
 * @brief main Main function of simulator
 *
 * This function initialises the Qt application and creates the main window ('Kuruk)
 * @param argc Argument count
 * @param argv Argument vector
 * @return Application exit status
 */

int main(int argc, char **argv){
    QApplication a(argc, argv);

    //! Create an instance of Kuruk, the main window
    Kuruk shetra;
    shetra.show();

    qDebug() << "executing...";
    return a.exec();
}
