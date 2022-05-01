#include <QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow *window = new QMainWindow;
    QMenuBar *tool_bar = new QMenuBar(window);
    Window *graph_area = new Window(window);
    QAction *action;

    if (graph_area->parse_command_line(argc, argv))
    {
        QMessageBox::warning(0, "Wrong input arguments!",
                             "Wrong input arguments!");
        return -1;
    }

    action = tool_bar->addAction("Change function", graph_area, SLOT(change_func()));
    action->setShortcut(QString("0"));

    action = tool_bar->addAction("Change method", graph_area, SLOT(change_method()));
    action->setShortcut(QString("1"));

    action = tool_bar->addAction("Scale +", graph_area, SLOT(scale_plus()));
    action->setShortcut(QString("2"));

    action = tool_bar->addAction("Scale -", graph_area, SLOT(scale_minus()));
    action->setShortcut(QString("3"));

    action = tool_bar->addAction("Points +", graph_area, SLOT(points_plus()));
    action->setShortcut(QString("4"));

    action = tool_bar->addAction("Points -", graph_area, SLOT(points_minus()));
    action->setShortcut(QString("5"));

    action = tool_bar->addAction("Delta +", graph_area, SLOT(delta_plus()));
    action->setShortcut(QString("6"));

    action = tool_bar->addAction("Delta -", graph_area, SLOT(delta_minus()));
    action->setShortcut(QString("7"));

    action = tool_bar->addAction("Exit", window, SLOT(close()));
    action->setShortcut(QString("Ctrl+X"));

    tool_bar->setMaximumHeight(30);

    window->setMenuBar(tool_bar);
    window->setCentralWidget(graph_area);
    window->setWindowTitle("2d");

    window->show();
    app.exec();
    delete window;
    return 0;
}
