// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>

#include "parseworker.h"
#include "commons.h"
#include "leaplistener.h"
#include "leapactionsender.h"

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    TriangleMesh* o_mesh;
    LeapActionSender leapActionSender;

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Window *ui;
    QMenu *fileMenu;
    QAction *openAct;
    ParseWorker mParseWorker;
    void createActions();
    void createMenus();

public slots:
    void open();
    void render(QSharedPointer<TriangleMesh> sp);
    void receiveLeapAction();

signals:
    void startParsing();
};

#endif // WINDOW_H
