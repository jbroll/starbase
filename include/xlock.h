
#include <pthread.h>

#ifdef _REENTRANT

#define	lock_decl(name)		pthread_mutex_t	name;
#define lock_init(lock)		pthread_mutex_init(&(lock), NULL);
#define lock_lock(lock)		pthread_mutex_lock(&(lock));
#define lock_unlock(lock)	pthread_mutex_lock(&(lock));

#else

#define	lock_decl(name)	
#define lock_init(lock)	
#define lock_lock(lock)	
#define lock_unlock(lock)

#endif

