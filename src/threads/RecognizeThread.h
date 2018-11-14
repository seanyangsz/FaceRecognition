#ifndef RECOGNITIONTHREAD_H
#define RECOGNITIONTHREAD_H

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
#include "face_info.h"
#include "helper/threadsafequeue.h"

using namespace cv;

class RecognizeThread : public QThread
{
    Q_OBJECT

public:
    ThreadSafeQueue<FaceInfo> *face_info_queue; //thread queue

public:
    RecognizeThread();
    ~RecognizeThread();

    void stop();

private:
    QTime t;
    QMutex doStopMutex;
    //QMutex processingMutex;
    volatile bool doStop;
    int processingTime;

protected:
    void run();

signals:
    void recognizeFrame(const FaceInfo &faceinfo);
    void faceFrame(const QImage &frame, const QString &name, const QString &time);
};

#endif // RECOGNITIONTHREAD_H
