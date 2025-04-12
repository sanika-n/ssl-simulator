#include "kuruk.h"
#include <QApplication>

int main(int argc, char **argv){
    QApplication a(argc, argv);

    Kuruk shetra; ///creates object shetra of type kuruk
    shetra.show(); //show shetra

    qDebug() << "executing...";
    return a.exec();
}
