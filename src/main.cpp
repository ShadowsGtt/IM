#include <iostream>
#include "../include/Thread.h"
#include "../include/ThreadPool.h"
#include <functional>
#include <boost/bind.hpp>

using namespace std;

MutexLock lk;

void initCallBack()
{
    cout << "thread [" << pthread_self() << "] running ! " << endl;
}

void task(int i)
{
    MutexLockGuard mg(lk);
    cout << "thread [" << pthread_self() << "] get task ! " << endl;
    cout << " number is :" << i << endl;

}

int main(void)
{

    ThreadPool tp("shadows thread pool");
    tp.setMaxQueueSize(1024);
    tp.setThreadInitCallback(boost::bind(&initCallBack));
    tp.start(5);
    for ( int i = 0 ;i < 10 ; ++i )
        tp.addTask(boost::bind(&task,i));

    sleep(1);
    return 0;
}
