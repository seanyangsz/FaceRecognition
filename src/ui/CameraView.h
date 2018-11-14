#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

// Qt
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
// Local
#include "threads/CaptureThread.h"
#include "threads/DetectThread.h"
#include "other/Structures.h"
#include "helper/SharedImageBuffer.h"
#include "ui/FrameLabel.h"
#include "recognitionitem.h"

namespace Ui {
    class CameraView;
}

class CameraView : public QWidget
{
    Q_OBJECT

    public:
        explicit CameraView(QWidget *parent);
        ~CameraView();
        bool connectToCamera(bool dropFrame, int width, int height, int fps);
        void disconnectToCamera();

    private:
        Ui::CameraView *ui;
        DetectThread *detectThread;
        CaptureThread *captureThread;
        RecognizeThread *recognizeThread;
        //ImageProcessingFlags imageProcessingFlags;
        void stopCaptureThread();
        void stopDetectThread();
        void stopRecognizeThread();
        bool isCameraConnected;
        int codec;
        bool itembackground;

    private slots:
        void updateFrame(const FaceInfo &faceinfo);
        void updateRecognizeFrame(const FaceInfo &faceinfo);
        void updateFaceFrame(const QImage &frame, const QString &name, const QString &time);

        void clickRecognizeBtn();
        void clickAddPersonBtn();
};

#endif // CAMERAVIEW_H
