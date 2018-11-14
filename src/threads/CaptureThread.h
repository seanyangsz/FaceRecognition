#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

// Qt
#include <QtCore/QTime>
#include <QtCore/QThread>
// OpenCV
#include <opencv2/highgui/highgui.hpp>
// Local
//#include "helper/SharedImageBuffer.h"
#include "other/Config.h"
#include "other/Structures.h"
#include "DetectThread.h"

using namespace cv;

class ImageBuffer;

class CaptureThread : public QThread
{
    Q_OBJECT
    public:
        DetectThread *detectthread;

    public:
        CaptureThread(DetectThread *detectthread,
                      bool dropFrameIfBufferFull, int width, int height, int fpsLimit);
        void stop();
        bool connectToCamera();
        bool disconnectCamera();
        bool isCameraConnected();
        int getInputSourceWidth();
        int getInputSourceHeight();
        void setCaptureState(bool docapture);

    private:
        //SharedImageBuffer *sharedImageBuffer;
        VideoCapture cap;
        QTime t;
        QMutex doStopMutex;
        QMutex doCaptureMutex;
        volatile bool doCapture;
        volatile bool doStop;
        int CaptureTime;
        bool dropFrameIfBufferFull;
        int width;
        int height;
        int fpsGoal;

    protected:
        void run();
};

#endif // CAPTURETHREAD_H
