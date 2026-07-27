#include "example.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    example window;
    window.show();
    return app.exec();
}
