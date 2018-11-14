#include "threads/RecognizeThread.h"
#include "recognition.h"

RecognizeThread::RecognizeThread() : QThread()
{
    // Initialize members
    doStop=false;

    face_info_queue = new ThreadSafeQueue<FaceInfo>(30);
}

// Destructor
RecognizeThread::~RecognizeThread()
{
    doStopMutex.lock();
    doStop = true;

    doStopMutex.unlock();
    wait();
}

void RecognizeThread::run()
{
    Recognition* recog = new Recognition();

    qDebug() << "Starting recognize thread...";
    while(1)
    {
        doStopMutex.lock();
        if(doStop)
        {
            doStop=false;
            doStopMutex.unlock();
            break;
        }
        doStopMutex.unlock();

        FaceInfo face_info;
        if (false == face_info_queue->pop_front(face_info, 1000)) {
            //qDebug() << "pop time out";
            continue;
        }

        recog->face_recognition(face_info.frame, face_info.faces);

        for (unsigned int i = 0; i < face_info.faces.size(); i++) {
            face_box& box = face_info.faces[i];

            /* get face area */
            float startX = box.x0 < 0 ? 0 : box.x0;
            float startY = box.y0 < 0 ? 0 : box.y0;
            float width = box.x1 >= face_info.frame.cols ? face_info.frame.cols - startX : box.x1 - startX;
            float high  = box.y1 >= face_info.frame.rows ? face_info.frame.rows - startY : box.y1 - startY;

            Mat face = face_info.frame(Rect2f(startX, startY, width, high));

            emit faceFrame(MatToQImage(face), QString::fromStdString(box.label), QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
            emit recognizeFrame(face_info);
        }

        face_info.frame.release();
        face_info.faces.clear();
    }
    qDebug() << "Stopping recognize thread...";
}

void RecognizeThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    doStop=true;
}
