#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

/** @class Mutex
 * 
 * Clase que enmascara el comportamiento de los pthread_mutex de POSIX.
 *  */

class Mutex {
private:
	pthread_mutex_t m;
	pthread_cond_t cv;
	Mutex (const Mutex &c){};
public:
	Mutex() {
		pthread_mutex_init(&m, 0);
		pthread_cond_init(&cv, 0);
	}
	
	~Mutex() {
		pthread_mutex_destroy(&m);
		pthread_cond_destroy(&cv);
	}
	
	void lock() {
		pthread_mutex_lock(&m);
	}
	
	void signal() {
		pthread_cond_signal(&cv);
	}
	
	void wait() {
		pthread_cond_wait(&cv, &m);
	}
	
	void unlock() {
		pthread_mutex_unlock(&m);
	}
	
	friend class Lock;
};

/** @class Lock
 * 
 * Clase que aplica un patron RAII para manejo cómodo de Mutex. */

class Lock{
private:
	Mutex &m;
	Lock(const Lock &c):m(c.m) {};
public:
	Lock(Mutex &m):m(m) {
		m.lock();
	}
	
	~Lock() {
		m.unlock();
	}
};

#endif
