#include "threads/CaptureThread.h"
#include "systimer.h"

CaptureThread::CaptureThread(DetectThread *detectthread,
                             bool dropFrameIfBufferFull, int width, int height, int fpsLimit)
    : QThread()
{
    // Save passed parameters
    this->dropFrameIfBufferFull=dropFrameIfBufferFull;
    this->width = width;
    this->height = height;
    this->fpsGoal = fpsLimit;
    // Initialize variables(s)
    doStop=false;
    doCapture = true;

    this->detectthread = detectthread;
}

void CaptureThread::run()
{
    SysTimer capture_timer(100);

    qDebug() << "Starting capture thread...";
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
        ////////////////////////// ////////
        ////////////////////////// ////////

        // Save capture time
        doCaptureMutex.lock();
        if (doCapture) {
            doCaptureMutex.unlock();

            // Capture frame (if available)
            if (!cap.grab()) {
                this->msleep(30);
                continue;
            }

            Mat grabbedFrame;
            // Retrieve frame
            cap.retrieve(grabbedFrame);

            if (capture_timer.timeout()) {
                // push frame to queue
                detectthread->camera_frame->push_back(grabbedFrame, 20);
            }

            grabbedFrame.release();
        } else {
            doCaptureMutex.unlock();
            this->msleep(30);
        }
    }
    qDebug() << "Stopping capture thread...";
}

bool CaptureThread::connectToCamera()
{
    // Open camera
    bool camOpenResult = cap.open(0);
    // Set resolution
    if(width != -1)
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    if(height != -1)
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    // Set maximum frames per second
    if(fpsGoal != -1)
        cap.set(CV_CAP_PROP_FPS, fpsGoal);
    // Return result
    return camOpenResult;
}

bool CaptureThread::disconnectCamera()
{
    // Camera is connected
    if(cap.isOpened())
    {
        // Disconnect camera
        cap.release();
        return true;
    }
    // Camera is NOT connected
    else
        return false;
}

void CaptureThread::stop()
{
    QMutexLocker locker(&doStopMutex);
    doStop=true;
}

bool CaptureThread::isCameraConnected()
{
    return cap.isOpened();
}

int CaptureThread::getInputSourceWidth()
{
    return cap.get(CV_CAP_PROP_FRAME_WIDTH);
}

int CaptureThread::getInputSourceHeight()
{
    return cap.get(CV_CAP_PROP_FRAME_HEIGHT);
}

void CaptureThread::setCaptureState(bool docapture)
{
    doCaptureMutex.lock();
    doCapture = docapture;
    doCaptureMutex.unlock();
}
