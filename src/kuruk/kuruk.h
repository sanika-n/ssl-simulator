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

class Kuruk : public QMainWindow
{
    Q_OBJECT
public:
    bool just_teleported = false; //to add ticks to delay drona sending vel commands after shunya teleports bots
    explicit Kuruk(QWidget *parent = nullptr);
    Vyasa *vyasa;
    ~Kuruk();

private:
    Ui::kuruk *ui;
    Shunya *shunya;
    Drona *drona;
    Vishnu *vishnu;
    std::shared_ptr<std::vector<BlueBot>> pandav;
    std::shared_ptr<std::vector<YellowBot>> kaurav;
    std::shared_ptr<Ball> ball;
signals:
    // void configureBots();
};

#endif // KURUK_H
