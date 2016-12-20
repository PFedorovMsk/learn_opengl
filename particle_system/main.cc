#include "smoke_scene.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SmokeScene s;
    s.show();

    return a.exec();
}
