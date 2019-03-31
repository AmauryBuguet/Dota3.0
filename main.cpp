#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Main_Window w;
    w.setAttribute(Qt::WA_DeleteOnClose);
    w.show();

    return app.exec();
}
