/*
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
 * @brief The Kuruk class creates a main window,
 */

class Kuruk : public QMainWindow
{
    Q_OBJECT
public:
    explicit Kuruk(QWidget *parent = nullptr);
    Vyasa *vyasa; ///pointer to vyasa object
    ~Kuruk();///kuruk destructor

private:
    Ui::kuruk *ui;
    Shunya *shunya; ///shunya object pointer
    Drona *drona; ///drona object pointer
    Vishnu *vishnu; ///vishnu object pointer
    std::shared_ptr<std::vector<BlueBot>> pandav; ///list of blue bots
    std::shared_ptr<std::vector<YellowBot>> kaurav; ///list of yellow bots
    std::shared_ptr<Ball> ball; //ball
signals:
    // void configureBots();
};

#endif // KURUK_H
