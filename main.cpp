#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include <string.h>
#include "Leap.h"

#include "window.h"
#include "leaplistener.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("MeshViewer-Qt");


    // Create a LeapMotion listener and controller
    LeapMotionListener listener;
    Controller controller;

    // Have the LeapMotion listener receive events from the controller
    controller.addListener(listener);

    //HandMotionConverter
    listener.setHandMotionConverter(&window.leapActionSender);


    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    return app.exec();
}
