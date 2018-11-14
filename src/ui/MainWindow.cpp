#include "ui/MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);

    // Connect other signals/slots
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(setFullScreen(bool)));

    connectToCamera();
}

MainWindow::~MainWindow()
{
    disconnectCamera();
    delete ui;
}

void MainWindow::connectToCamera()
{
    // Create CameraView
    cameraView = new CameraView(ui->centralWidget);
    ui->frameLayout->addWidget(cameraView,0,0);
}

void MainWindow::disconnectCamera()
{
    delete cameraView;
}

void MainWindow::showAboutDialog()
{
    QString aboutMessage;
    QTextStream ts(&aboutMessage);
    ts << "<h3>Face Detect and Recognition</h3>"<<
          "<p>This is an OpenCV and Qt based face detect and recognition.</p>"<<
          "<p>Copyright &copy; 2018 Yang Fenghu</p>";

    QMessageBox::about(this, tr("About"), aboutMessage);
}

void MainWindow::setFullScreen(bool input)
{
    if(input)
        this->showFullScreen();
    else
        this->showNormal();
}
