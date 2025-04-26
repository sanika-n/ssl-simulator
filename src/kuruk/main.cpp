#include "kuruk.h"
#include <QApplication>

int main(int argc, char **argv){
    QApplication a(argc, argv);

    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        qDebug() << "SDL could not initialize! SDL Error:" << SDL_GetError();
        // Handle error
    }

    // When the application closes:
    SDL_Quit();

    Kuruk shetra; ///creates object shetra of type kuruk
    shetra.show(); //show shetra

    qDebug() << "executing...";
    return a.exec();
}
