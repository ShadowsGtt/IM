# IM 服务器介绍
A Server Instant Messaging System

## InetAddress [InetAddress.cpp InetAddress.h]  
#### 描述  
```
	对IPV4地址结构struct sockaddr_in的封装
```
#### 接口  
```
	/* 构造函数  传入ip 和 port */
	InetAddress(string ip, uint16_t port);
	
	/* 构造函数  传入struct sockaddr_in结构体 */
	InetAddress(const struct sockaddr_in& addr);

	/* 返回地址族 用到的都是IPV4 AF_INET */
  	sa_family_t family();

	 /* 以string形式返回IP地址  */
  	/* eg: 0.0.0.0 */
  	string toIp() const;

  	/* 以string形式返回IP 及 port */
  	/* eg: 0.0.0.0:8888 */
  	string toIpPort();

  	/* 返回port */
  	uint16_t toPort();


  	/* 返回网络端(大端)IP */
  	uint32_t ipNetEndian();
  
  	/* 返回网络端(大端)端口 */
  	uint16_t portNetEndian();

	/* 以struct sockaddr形式返回地址 */
  	const struct sockaddr* getSockAddr();

   	/* 设置sockaddr_in地址 */
   	void setSockAddrInet(const struct sockaddr_in& addr);
```



## Socket [Socket.cpp Socket.h]  
#### 描述  
```
	对socket fd的封装
```
#### 接口  
```
	/* 构造函数  传入socket fd */
	Socket(int sockfd);

	/* 返回socket fd */
	int fd();
	
	/* 获得TCP详细信息  结果存入传入的tcp_info结构地址中  */
	/* 成功返回true 失败返回false */
	bool getTcpInfo(struct tcp_info*);

	/* 获得TCP详细信息  结果存入buf地址中 */
	bool getTcpInfoString(char* buf, int len);

	/* 绑定ip和port给socketfd */
	void bindAddress(const InetAddress& localaddr);

	/* 开始监听 */
	void listen();

	/* 关闭写端 */
	void shutdownWrite();

	/* 是否设置TCP 选项 TCP_NODELAY */
	/* 设置TCP_NODELAY其实就是禁用了Nagle算法，允许小包的发送 */
	/* 如果对延时要求高，则开启TCP_NODELAY */
	void setTcpNoDelay(bool on);

	/* 是否设置地址复用 SO_REUSEADDR选项  */
	/* 设置SO_REUSEADDR为了重启监听服务 */
	/* 端口被释放之后socketfd处于TIME_WAIT状态 正常情况2min內是不可以重新绑定的 */
	/* 而导致bind函数出错 */
	void setReuseAddr(bool on);

	/* 是否设置端口复用 SO_REUSEPORT选项 */
	/* 这个选项允许将相同的ip和port绑定到多个socket上 */
    /* 这些socket可以分布在相同主机的同一个线程、多个线程、乃至多个进程中去 */
	/* 内核会自动把这个端口的请求自动分派到各个socket上面去 */
	/* 而且这个过程没有用户惊群、互斥等问题 */
	void setReusePort(bool on);

	/* 设置保活 */
	void setKeepAlive(bool on);
	
```



## Thread [Thread.cpp Thread.h]  
#### 描述  
```
	对线程的封装
```
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
#### 描述  
```
	对互斥锁的封装
```
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
#### 描述  
```
	静态线程池的实现
```
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




