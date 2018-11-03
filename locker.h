#ifndef __LOCKER_H__
#define __LOCKER_H__

#include <exception>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>

//封装信号量
class sem
{
public:
    sem()
    {
        if(sem_init(&m_sem, 0 ,0) != 0)
        {
            throw std::exception();
        }

    }

    ~sem()
    {
        sem_destroy( &m_sem );
    }
    //等待信号量
    bool wait()
    {
        return sem_wait( &m_sem ) == 0;
    }
    //增加信号量
    bool post()
    {
        return sem_post( &m_sem ) == 0;
    }
private:
    sem_t m_sem;
};

//封装互斥锁
class locker
{
public:
    //初始化互斥锁
    locker()
    {
        if(pthread_mutex_init(&m_mutex, NULL) != 0)
        {
            throw std::exception();
        }
    }

    ~locker()
    {
        pthread_mutex_destroy( &m_mutex );
    }

    //获取锁或者说上锁
    bool lock()
    {
        printf("任务队列上锁\n");
        return pthread_mutex_lock( &m_mutex ) == 0;
    }

    //解锁或者说释放
    bool unlock()
    {
        printf("任务队列解锁\n");
        return pthread_mutex_unlock(&m_mutex) == 0;
    }

private:
    pthread_mutex_t m_mutex;

};

//封装条件变量
class cond
{
public:
    cond()
    {
        if(pthread_mutex_init( &m_mutex, NULL ) != 0)
        {
            throw std::exception();
        }

        if(pthread_cond_init( &m_cond, NULL) != 0)
        {
            pthread_mutex_destroy( &m_mutex );
            throw std::exception();
        }
    }

    ~cond()
    {
        pthread_mutex_destroy( &m_mutex );
        pthread_cond_destroy(&m_cond);
    }

    //等待条件变量
    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock( &m_mutex );
        ret = pthread_cond_wait( &m_cond, &m_mutex );
        pthread_mutex_unlock( &m_mutex );
        return ret == 0;
    }

    //换新等待条件变量的线程
    bool signal()
    {
        return pthread_cond_signal( &m_cond ) == 0;
    }

private:
    pthread_cond_t m_cond;
    pthread_mutex_t m_mutex;
};

#endif

