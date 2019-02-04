
#ifndef _THREAD_H
#define _THREAD_H

//#include "../include/Atomic.h"
#include "../include/CountDownLatch.h"
#include "../include/Types.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
#include <pthread.h>
#include <string>
#include "./Timestamp.h"

using namespace std;

class Timestamp;

class Thread : boost::noncopyable
{
 public:
  typedef boost::function<void ()> ThreadFunc;

  explicit Thread(const ThreadFunc&, const string& name = string()); 
  ~Thread();

  void start();
  int join(); // return pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return tid_; }
  const string& name() const { return name_; }

  //static int numCreated() { return numCreated_.get(); }

 private:
  //void setDefaultName();

  bool       started_;
  bool       joined_;
  pthread_t  pthreadId_;
  pid_t      tid_;
  ThreadFunc func_;
  string     name_;
  CountDownLatch latch_;

  //static AtomicInt32 numCreated_;
};

#endif
