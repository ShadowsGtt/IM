# IM 服务器介绍
A Server Instant Messaging System



## Mutex  [Mutex.h]  
#### 接口  
```
	/* 新建MutexLock，无参构造 */
	MutexLock();
	
	/* 本线程是否拥有锁 */
	bool isLockedByThisThread();

	/* 是否加锁，锁没有被锁住时会程序会退出 */
	void assertLocked();

	/* 锁住锁 */
	void lock();

	/* 释放锁 */
	void unlock()

	/* 获得锁 */
	pthread_mutex_t* getPthreadMutex()

	/* 构造时会对mutex上锁,析构时会解锁 */
	MutexLockGuard(MutexLock& mutex)
	
	
```  
### Example  
```
#include <iostream>
#include "../include/Mutex.h"

using namespace std;


int main(void)
{
    /* 新建一个互斥锁 */
    MutexLock mtx;

    /* 上锁 */
    mtx.lock();

    /* 未上锁时，程序会退出 */
    mtx.assertLocked();

    /* 解锁 */
    mtx.unlock();

    /* 获得原生锁 */
    pthread_mutex_t *mt = mtx.getPthreadMutex();

    /* 锁是否被本线程锁住 */
    if(mtx.isLockedByThisThread())
        cout << "is locked by this thread" << endl;
    else
        cout << "is not locked by this thread" << endl;

    {
        /* 对mtx加锁 */
        MutexLockGuard mx(mtx);
        if(mtx.isLockedByThisThread())
            cout << "MutexLockGuard locked" << endl;
        else
            cout << "MutexLockGuard unlocked" << endl;
    }
    if(mtx.isLockedByThisThread())
        cout << "MutexLockGuard locked" << endl;
    else
        cout << "MutexLockGuard unlocked" << endl;

    return 0;
}
```  





## ThreadPool  [ThreadPool.h  ThreadPool.cpp]
#### **接口*  
```
	/* 新建线程池,并为其命名 */
	ThreadPool(const string& nameArg = string("ThreadPool"));	

    /* 设置任务队列大小 */
    void setMaxQueueSize(int maxSize);
    
    /* 设置线程池中线程启动时运行的函数 */
    void setThreadInitCallback(const Task& cb);

    /* 设置线程池大小并启动 */
    void start(int numThreads);

    /* 线程池停止 */
    void stop();

    /* 返回线程池名字 */
    const string& name();
    
    /* 返回任务队列中任务个数 */
    size_t queueSize();
```
#### **Example

```
#include <iostream>
#include "../include/Thread.h"
#include "../include/ThreadPool.h"
#include <boost/bind.hpp>

using namespace std;

MutexLock lk;

void initCallBack()
{
    cout << "thread [" << pthread_self() << "] running ! " << endl;
}

void task(int i)
{
    /* 这里加锁是为了让stdout有序输出  */
    MutexLockGuard mg(lk);
    cout << "thread [" << pthread_self() << "] get task ! " << endl;
    cout << " number is :" << i << endl;

}

int main(void)
{

    /* 创建线程池并命名 */
    ThreadPool tp("shadows thread pool");

    /* 设置线程池任务队列大小 */
    tp.setMaxQueueSize(1024);

    /* 设置线程池中的线程启动时运行的初始化函数 */
    tp.setThreadInitCallback(boost::bind(&initCallBack));

    /* 设置线程池中线程数目并启动线程池 */
    tp.start(5);

    /* 打印线程池名字 */
    cout << "name of thread pool :" << tp.name() << endl;

    /* 打印任务队列中任务个数 */
    cout << "count of task :" << tp.queueSize() << endl;

    /* 向线程池中添加任务 */
    for ( int i = 0 ;i < 10 ; ++i )
        tp.addTask(boost::bind(&task,i));

    /* 线程池停止 */
    tp.stop();

    return 0;
}

```
