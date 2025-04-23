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
#include <QWidget>
#include <QDockWidget>
#include <QLabel>
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
    explicit Kuruk(QWidget *parent = nullptr);
    Vyasa *vyasa;
    static QLabel* robotIdLabel;
    static QLabel* robotPositionLabel;
    static QLabel* robotOrientationLabel;
    QWidget* sidebar;
    QDockWidget* sidebarDockWidget;
    ~Kuruk();

private:
    Ui::kuruk *ui;
    Shunya *shunya;
    Drona *drona;
    Vishnu *vishnu;
    std::shared_ptr<std::vector<BlueBot>> pandav;
    std::shared_ptr<std::vector<YellowBot>> kaurav;
    std::shared_ptr<Ball> ball;
    
public slots:
    void updateSidebar(int id, QPointF position, float orientation);
signals:
    // void configureBots();
};

#endif // KURUK_H
