#include "method.h"
#include "window.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    window scene;

    if (scene.parse_command_line(argc, argv))
    {
        QMessageBox::warning(0, "Неправильные аргументы", "Неправильные аргументы");
        return -1;
    }

    scene.setWindowTitle("3d");
    scene.resize(500, 500);
    scene.showMaximized();
    return app.exec();
}
