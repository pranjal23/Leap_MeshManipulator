#include <QtWidgets>
#include "window.h"
#include "ui_window.h"
#include "trianglemesh.h"

#include "viewportwidget.h"
#include "mfileparser.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    o_mesh = NULL;

    connect(this,SIGNAL(startParsing()),&mParseWorker,SLOT(parse()));

    connect(&mParseWorker,
            SIGNAL(parseComplete(QSharedPointer<TriangleMesh>)),
            this,
            SLOT(render(QSharedPointer<TriangleMesh>)));

    connect(&leapActionSender,
            SIGNAL(sendLeapAction()),
            this,
            SLOT(receiveLeapAction()));

    createActions();
    createMenus();
}

Window::~Window()
{
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::New);
    openAct->setStatusTip(tr("Open a new 3D Mesh"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
}

void Window::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
}

//bool use_multi_threading = true;
TriangleMesh* out_mesh = NULL;
QLabel *lbl  = NULL;
void Window::open(){
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open 3D Mesh"),
                QDir::currentPath(),
                tr("3D Mesh (*.m)") );
    if( !filename.isEmpty() )
    {
        lbl = new QLabel;
        lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        QMovie *movie = new QMovie(":/images/loading.gif");
        lbl->setMovie(movie);
        lbl->setAttribute(Qt::WA_TranslucentBackground);
        lbl->setFixedSize(350,290);
        lbl->setWindowModality(Qt::ApplicationModal);
        lbl->setWindowFlags(Qt::FramelessWindowHint);
        lbl->show();
        movie->start();

        mParseWorker.setFileName(filename);
        emit startParsing();

    }
}

void Window::render(QSharedPointer<TriangleMesh> sp){
    TriangleMesh* sInMesh = sp.data();
    if(sInMesh!=NULL){

        ui->viewPortWidget->triangleMesh = sInMesh;
        ui->viewPortWidget->updateGL();
    }
    else
    {
        qDebug() << "Pointer location NULL";
    }
    lbl->close();
}

int m_Mode = 0;
void Window::receiveLeapAction()
{
    qDebug() << "Received Empty Leap Action in Window...";
    qDebug() << "Number of items in vector: " << QString::number(leapActionSender.actionVector->size());

    LeapAction leapAction = leapActionSender.actionVector->back();
    const int command = leapAction.getActionName();
    qDebug() << "Name: " << QString::number(command);

    if(command==0 && m_Mode == 0)
    {
        //ZOOM
        qDebug() << "IN ZOOM ";
        float direction = leapAction.getAxisOfMotion()->z;

        if( direction < 0.0)
        {
            ui->viewPortWidget->changeCameraZoom(-0.5f);
        }
        else if(direction > 0.0)
        {
            ui->viewPortWidget->changeCameraZoom(0.5f);
        }
    }
    else if(command==1 && m_Mode == 1)
    {
        //PAN
        qDebug() << "IN PAN ";
        float directionX = leapAction.getAxisOfMotion()->x;

        if( directionX < 0.0)
        {
            ui->viewPortWidget->changeCameraPositionOnXAxis(-0.5f);
        }
        else if(directionX > 0.0)
        {
            ui->viewPortWidget->changeCameraPositionOnXAxis(0.5f);
        }

        float directionY = leapAction.getAxisOfMotion()->y;

        if( directionY < 0.0)
        {
            ui->viewPortWidget->changeCameraPositionOnYAxis(-0.5f);
        }
        else if(directionY > 0.0)
        {
            ui->viewPortWidget->changeCameraPositionOnYAxis(0.5f);
        }
    }
    else if(command==2 && m_Mode == 2)
    {
        //ROTATE
        qDebug() << "IN ROTATE ";
    }
    else if(command==3)
    {
        //CHANGE MODE
        qDebug() << "IN MODE ";

        if(m_Mode<2)
            m_Mode++;
        else
            m_Mode=0;

        if(m_Mode==0)
            ui->actionNameTextEdit->setText("ZOOM");
        else if(m_Mode==1)
            ui->actionNameTextEdit->setText("PAN");
        else if(m_Mode==2)
            ui->actionNameTextEdit->setText("ROTATE");
    }
    else
    {
        //UNKNOWN MODE
        qDebug() << "IN UNKNOWN MODE ";
    }

    leapActionSender.actionVector->pop_back();
}

