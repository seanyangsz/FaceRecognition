#ifndef DETECTTHREAD_H
#define DETECTTHREAD_H

// Qt
#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtCore/QQueue>
#include "QDebug"
// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
// Local
#include "other/Structures.h"
#include "other/Config.h"
#include "other/Buffer.h"
#include "helper/MatToQImage.h"
#include "helper/SharedImageBuffer.h"
#include "RecognizeThread.h"

using namespace cv;

class DetectThread : public QThread
{
    Q_OBJECT
public:
    RecognizeThread *recognitonThread;
    ThreadSafeQueue<Mat> *camera_frame; //thread queue

public:
    DetectThread(RecognizeThread *recognition);
    ~DetectThread();
    bool releaseCapture();
    void stop();


private:
    QMutex doStopMutex;
    volatile bool doStop;

protected:
    void run();


signals:
    void newFrame(const FaceInfo &faceinfo);
};

#endif // DETECTTHREAD_H
