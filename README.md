# IM 服务器介绍
A Server Instant Messaging System


## Thread [Thread.cpp Thread.h]  
#### 接口  
```
	/* 新建一个线程类 参数1:设置回调函数  参数2:线程名字 */
	Thread(const boost::function<void ()>&, const string& name = string());
	
	/* 线程开始运行 */
	void Thread::start()
	
	/* 等待线程运行结束 return 0 函数调用成功  */
	int Thread::join();
	
	/* 返回线程id */
	pid_t Thread::tid();
	
	/* 返回线程名字 */
	const Thread::string& name();
```



## Mutex  [Mutex.h]  
#### 接口  
```
	/* 新建MutexLock，无参构造 */
	MutexLock();
	
	/* 本线程是否拥有锁 */
	bool MutexLock::isLockedByThisThread();

	/* 是否加锁，锁没有被锁住时会程序会退出 */
	void MutexLock::assertLocked();

	/* 锁住锁 */
	void MutexLock::lock();

	/* 释放锁 */
	void MutexLock::unlock();

	/* 获得锁 */
	pthread_mutex_t* MutexLock::getPthreadMutex();

	/* 构造时会对mutex上锁,析构时会解锁 */
	MutexLockGuard(MutexLock& mutex);
	
	/* 析构时解锁mutex */
	~MutexLockGuard();
	
```  



## ThreadPool  [ThreadPool.h  ThreadPool.cpp]
#### **接口**  
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




