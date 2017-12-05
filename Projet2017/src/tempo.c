#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "timer.h"

// Return number of elapsed µsec since... a long time ago
static unsigned long get_time (void)
{
  struct timeval tv;

  gettimeofday (&tv ,NULL);

  // Only count seconds since beginning of 2016 (not jan 1st, 1970)
  tv.tv_sec -= 3600UL * 24 * 365 * 46;
  
  return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#ifdef PADAWAN


void handler(int sig){
  printf ("sdl_push_event(%p) appelée au temps %ld\n",NULL, get_time ());//A CHANGER
}

void* run_th1(void* theSignal)
{ 

  struct sigaction gestionSignal;
  
  gestionSignal.sa_handler=&handler;
  gestionSignal.sa_flags=0;
  
  sigfillset(&gestionSignal.sa_mask);
  sigdelset(&gestionSignal.sa_mask,SIGALRM);
  sigaction(SIGALRM,&gestionSignal,NULL);
  
  while(1){
    sigsuspend(&gestionSignal.sa_mask);
  }
 
}

// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{
  pthread_t th1;
  printf(" PID : %d\n",getpid());

  sigset_t mask;
     sigemptyset(&mask);
  sigaddset(&mask, SIGALRM);
  sigprocmask(SIG_SETMASK, &mask, NULL);
  
  if(pthread_create(&th1, NULL, run_th1, NULL) == -1) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }

  return 0; // Implementation not ready
}

timer_id_t timer_set (Uint32 delay, void *param)
{
  
  struct itimerval it_val;

  it_val.it_value.tv_sec =     delay/1000;
  it_val.it_value.tv_usec =    (delay*1000) % 1000000;	
  it_val.it_interval = it_val.it_value;
  
  return (timer_id_t) setitimer(ITIMER_REAL,&it_val,NULL);
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO

  return 0; // failure
}

#endif
