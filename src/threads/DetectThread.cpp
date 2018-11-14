#include "threads/DetectThread.h"
#include "detect.h"
#include "systimer.h"

DetectThread::DetectThread(RecognizeThread *recognition) : QThread()
{
    this->recognitonThread = recognition;

    // Initialize members
    doStop=false;

    camera_frame = new ThreadSafeQueue<Mat>(30);
}

// Destructor
DetectThread::~DetectThread()
{
    doStopMutex.lock();
    doStop = true;
    if(releaseCapture())
        qDebug() << "Released Capture";
    doStopMutex.unlock();
    wait();
}

// Release videoCapture if available
bool DetectThread::releaseCapture()
{
    return true;
}

void DetectThread::run()
{
    Detect* detect = new Detect();
    SysTimer recognize_timer(1000);

    qDebug() << "Starting detect thread...";
    while(1)
    {
        ////////////////////////// /////// 
        // Stop thread if doStop=TRUE // 
        ////////////////////////// /////// 
        doStopMutex.lock();
        if(doStop)
        {
            doStop=false;
            doStopMutex.unlock();
            break;
        }
        doStopMutex.unlock();

        FaceInfo face_info;

        // Get frame from queue, store in currentFrame, set ROI
        if (false == camera_frame->pop_front(face_info.frame, 100)) {
            //qDebug() << "pop time out";
            continue;
        }

        if (recognize_timer.timeout()) {
            detect->detect_faces(face_info.frame, face_info.faces);
            if (face_info.faces.size() > 0) {
                recognitonThread->face_info_queue->push_back(face_info, 20);
            }
        }

        emit newFrame(face_info);

        face_info.faces.clear();
        face_info.frame.release();
    }
    qDebug() << "Stopping detect thread...";
}

void DetectThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    releaseCapture();
    doStop=true;
}
