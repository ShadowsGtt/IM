#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <atomic>

class SpinLock
{

public:
    Spinlock() : flag(ATOMIC_FLAG_INIT) {}

    void lock(){
        while ( flag.test_and_set() );
    }

    void unlock(){
        flag.clear();
    }
private:
    std::atomic_flag flag;

};

#endif // end _SPINLOCK_H
