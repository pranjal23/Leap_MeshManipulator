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
#include "handmotionconverter.h"

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
    void receiveHandAction(HAND_ACTION hand_action,
                           ZOOML_TYPE zoom_type,
                           ROTATE_DIRECTION rotate_direction,
                           PAN_DIRECTION pan_direction,
                           float strength);
    void connectHandMotionConverter(HandMotionConverter *handMotionConverter);

private slots:
    void on_enableLightBtn_clicked(bool checked);
    void on_multilightsBtn_clicked(bool checked);
    void on_perspectiveBtn_clicked(bool checked);
    void on_orthographicBtn_clicked(bool checked);
    void on_boundBoxBtn_clicked(bool checked);
    void on_groundBtn_clicked(bool checked);
    void on_axisBtn_clicked(bool checked);
    void on_smoothShadingRb_toggled(bool checked);
    void on_flatShadingRb_toggled(bool checked);
    void on_wireframeRb_toggled(bool checked);
    void on_pointsRb_toggled(bool checked);
    void on_colorMatBtn_clicked(bool checked);
    void on_xyRb_toggled(bool checked);
    void on_xzRb_toggled(bool checked);
    void on_yzRb_toggled(bool checked);
    void on_axisLengthSlider_sliderMoved(int value);

    void on_lightSlider_sliderMoved(int position);

signals:
    void startParsing();
};

#endif // WINDOW_H
