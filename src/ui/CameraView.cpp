#include "ui/CameraView.h"
#include "ui_CameraView.h"
#include "addpersondialog.h"
#include "face_info.h"

CameraView::CameraView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraView),
    codec(-1)
{
    // Setup UI
    ui->setupUi(this);

    itembackground = false;

    // Initialize internal flag
    isCameraConnected=false;

    connect(ui->stopRecognizeBtn, SIGNAL(released()), this, SLOT(clickRecognizeBtn()));
    connect(ui->addPersonBtn, SIGNAL(released()), this, SLOT(clickAddPersonBtn()));

    connectToCamera(true, 640, 480, -1);

    // Register type
    qRegisterMetaType<struct FaceInfo>("FaceInfo");
}

CameraView::~CameraView()
{
    if (isCameraConnected) {
        if(captureThread->isRunning())
            stopCaptureThread();
        if(detectThread->isRunning())
            stopDetectThread();
        if(recognizeThread->isRunning())
            stopRecognizeThread();

        // Disconnect camera
        if(captureThread->disconnectCamera())
            qDebug() << "Camera successfully disconnected.";
        else
            qDebug() << "WARNING: Camera already disconnected.";
    }
    // Delete UI
    delete detectThread;
    delete captureThread;
    delete recognizeThread;
    delete ui;
}

bool CameraView::connectToCamera(bool dropFrameIfBufferFull, int width, int height, int fps)
{
    ui->detectFrame->setText(tr("Connecting to camera..."));
    // Create recognize thread
    recognizeThread = new RecognizeThread();
    // Create detect thread
    detectThread = new DetectThread(recognizeThread);
    // Create capture thread
    captureThread = new CaptureThread(detectThread, dropFrameIfBufferFull, width, height, fps);

    // Attempt to connect to camera
    if(captureThread->connectToCamera())
    {
        // Set internal flag and return
        isCameraConnected=true;

        // Setup signal/slot connections
        connect(detectThread, SIGNAL(newFrame(FaceInfo)), this, SLOT(updateFrame(FaceInfo)));
        connect(recognizeThread, SIGNAL(recognizeFrame(FaceInfo)), this, SLOT(updateRecognizeFrame(FaceInfo)));
        connect(recognizeThread, SIGNAL(faceFrame(QImage, QString, QString)), this, SLOT(updateFaceFrame(QImage, QString, QString)));

        recognizeThread->start();
        detectThread->start();
        captureThread->start();

        return true;
    }
    // Failed to connect to camera
    else
        return false;
}

void CameraView::stopCaptureThread()
{
    qDebug() << "About to stop capture thread...";
    captureThread->stop();
    captureThread->wait();
    qDebug() << "Capture thread successfully stopped.";
}

void CameraView::stopDetectThread()
{
    qDebug() << "About to stop detect thread...";
    detectThread->stop();
    detectThread->wait();
    qDebug() << "Detect thread successfully stopped.";
}

void CameraView::stopRecognizeThread()
{
    qDebug() << "About to stop recognize thread...";
    recognizeThread->stop();
    recognizeThread->wait();
    qDebug() << "Recognize thread successfully stopped.";
}

void CameraView::updateFrame(const FaceInfo &faceinfo)
{
    QImage img = MatToQImage(faceinfo.frame);
#if 0
    QPainter painter(&img);
    painter.setPen(QPen(Qt::green, 5));

    for (unsigned int i = 0; i < faceinfo.faces.size(); i++) {
        face_box box = faceinfo.faces[i];
        painter.drawRect(box.x0, box.y0, box.x1 - box.x0, box.y1 - box.y0);
    }
#endif
    // Display frame
    ui->detectFrame->setPixmap(QPixmap::fromImage(img).scaled(ui->detectFrame->width(), ui->detectFrame->height(),Qt::KeepAspectRatio));
}

void CameraView::updateRecognizeFrame(const FaceInfo &faceinfo)
{
    QImage img = MatToQImage(faceinfo.frame);
    QPainter painter(&img);
    QFont font;

    font.setPointSize(24);
    painter.setFont(font);
    painter.setPen(QPen(Qt::green, 2));

    for (unsigned int i = 0; i < faceinfo.faces.size(); i++) {
        face_box box = faceinfo.faces[i];
        painter.drawRect(QRectF(QPointF(box.x0, box.y0), QPointF(box.x1, box.y1)));
        painter.drawText(box.x0, box.y0 - 5 < 0 ? 0 : box.y0 - 5, QString::fromStdString(box.label));
    }

    // Display frame
    ui->recognitionFrame->setPixmap(QPixmap::fromImage(img).scaled(ui->detectFrame->width(), ui->detectFrame->height(),Qt::KeepAspectRatio));
}

void CameraView::updateFaceFrame(const QImage &frame, const QString &name, const QString &time)
{
    RecognitionItem *item = new RecognitionItem(this, true);
    QListWidgetItem *listItem = new QListWidgetItem;

    listItem->setSizeHint(QSize(item->sizeHint().width(), item->sizeHint().height()));
    if (itembackground == true) {
        listItem->setBackgroundColor(0xFFCCEEDD);
        itembackground = false;
    } else {
        listItem->setBackgroundColor(0xFFCCDDEE);
        itembackground = true;
    }

    item->fill(frame, name, time);

    ui->listWidget->addItem(listItem);
    ui->listWidget->setItemWidget(listItem, item);
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);

   if(ui->listWidget->count() >= 50) {
       ui->listWidget->clear();
   }
}

void CameraView::clickRecognizeBtn() {
    if(ui->stopRecognizeBtn->isChecked()) {
        ui->stopRecognizeBtn->setText(tr("开始识别"));
        captureThread->setCaptureState(false);
    } else {
        ui->stopRecognizeBtn->setText(tr("停止识别"));
        captureThread->setCaptureState(true);
    }
}

void CameraView::clickAddPersonBtn() {
    // Show dialog
    AddPersonDialog *addPersonDialog = new AddPersonDialog(this);

    if(addPersonDialog->exec() == QDialog::Accepted)
    {

    }
}
