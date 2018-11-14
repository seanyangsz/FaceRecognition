#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <QDateTime>
#include <stdio.h>
#include <time.h>

long long get_runtime_in_msecs();

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue(int nMaxSize);
    bool push_back(const T& data);
    T pop_front();

    bool push_back(const T& data, const int milisec);
    bool pop_front(T &data, const int milisec);

private:
    int m_nMaxSize;
    QList<T> m_oList;
    QMutex m_oMutex;
    QWaitCondition m_oWaitCondition;
};

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue(int nMaxSize)
    : m_oList(),
    m_oMutex(),
    m_oWaitCondition()
{
    this->m_nMaxSize = nMaxSize;
}

/* 首先判断队列是否为空，如果队列为空时，当加入新的数据时，调用条件等待的wakeAll唤醒
 * 其他阻塞的读线程，如果有多个读线程，只能唤醒一个线程，唤醒线程的顺序一般先唤醒高优先级的,
 * 如果优先级相同，则唤醒等待时间长的那个
 */
template<typename T>
bool ThreadSafeQueue<T>::push_back(const T &data)
{
    m_oMutex.lock();
    if (m_oList.count() == m_nMaxSize) {
        m_oMutex.unlock();
        return false;
    }

    bool bEmpty = m_oList.empty();
    m_oList.push_back(data);
    m_oMutex.unlock();

    if (bEmpty)
        m_oWaitCondition.wakeAll();

    return true;
}

/* 当队列为空时等待，QWaitCondition还会释放QMutex(内部会调用unlock)
 * 其他调用此方法的线程也阻塞在等待条件这里
 */
template<typename T>
T ThreadSafeQueue<T>::pop_front()
{
    m_oMutex.lock();

    /* 在这里应该使用while而不应该使用if, 因为操作系统有时会出现虚假唤醒 */
    while (m_oList.empty())
    {
        m_oWaitCondition.wait(&m_oMutex);
    }
    T temp = m_oList.front();
    m_oList.pop_front();

    m_oMutex.unlock();

    return temp;
}

template<typename T>
bool ThreadSafeQueue<T>::push_back(const T &data, const int milisec)
{
    long long nTick1 = get_runtime_in_msecs();
    QMutexLocker mutexLocker(&m_oMutex);
    long long nTick2 = get_runtime_in_msecs();
    if (nTick2 - nTick1 < milisec) {
        bool bEmpty = m_oList.empty();

        m_oList.push_back(data);

        if (bEmpty)
            m_oWaitCondition.wakeAll();

        return true;
    }

    return false;
}

template<typename T>
bool ThreadSafeQueue<T>::pop_front(T &data, const int milisec)
{
    long long nTick1 = get_runtime_in_msecs();
    QMutexLocker mutexLocker(&m_oMutex);

    long long nTick2 = get_runtime_in_msecs();
    if (nTick2 - nTick1 < milisec)
    {
        bool bResult = true;
        while (m_oList.empty() && bResult)
        {
            /* QWaitCondition如果是被唤醒，则返回true
             * 如果是超时(参数2)，则返回false
             */
            bResult = m_oWaitCondition.wait(&m_oMutex, milisec - (nTick2 - nTick1));
        }
        if (bResult) {
            data = m_oList.front();
            m_oList.pop_front();
            return true;
        }
    }

    return false;
}

#endif // THREADSAFEQUEUE_H
