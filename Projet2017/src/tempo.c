#include "SDL.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

#include "timer.h"
#include "liste.h"

ctrl tempoList;

static pthread_mutex_t capsule = PTHREAD_MUTEX_INITIALIZER;

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

void handlerALRM(int sig){

  puts("ok");
  
  pthread_mutex_lock (&capsule);
  int delay;
  do {
    sdl_push_event(headListParam(tempoList));
    delay = headListDelay(tempoList);
    delTop(tempoList);
  }while(tempoList->size > 0 && headListDelay(tempoList) - delay < 10000);

  if(tempoList->size > 0 ){

    struct itimerval it_val;
    delay = tempoList->debut->delay - get_time();
    it_val.it_value.tv_sec =     delay/1000;
    it_val.it_value.tv_usec =    (delay*1000) % 1000000;
    it_val.it_interval.tv_sec =  0;
    it_val.it_interval.tv_usec =  0;
    printf("new timer ? %lu\n",it_val.it_value.tv_sec);
    setitimer(ITIMER_REAL,&it_val,NULL);
  }
  pthread_mutex_unlock (&capsule);
  

}

void* run_th1(void* theSignal)
{
  struct sigaction gestionSignalALRM;
  gestionSignalALRM.sa_handler=&handlerALRM;
  gestionSignalALRM.sa_flags=0;
  sigfillset(&gestionSignalALRM.sa_mask);
  sigdelset(&gestionSignalALRM.sa_mask,SIGALRM);
  sigaction(SIGALRM,&gestionSignalALRM,NULL);
    
  while(1){
    sigsuspend(&gestionSignalALRM.sa_mask);
  }
    
}

// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{
  pthread_t th1;
    
  // Mask SIGALRM signal
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGALRM);
  sigaddset(&mask, SIGUSR1);
  sigprocmask(SIG_SETMASK, &mask, NULL);
  /* now mask == {SIGALRM}*/
    
  tempoList = (ctrl)malloc(sizeof(struct ctrlList));
    
  initCtrl(tempoList);
    
  // Create thread
  if(pthread_create(&th1, NULL, run_th1, NULL) == -1) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }
    
  return 1;
}

timer_id_t timer_set (Uint32 delay, void *param)
{
  struct itimerval it_val;
  
  it_val.it_value.tv_sec =     delay/1000;
  it_val.it_value.tv_usec =    (delay*1000) % 1000000;
  it_val.it_interval.tv_sec =  0;
  it_val.it_interval.tv_usec =  0;
  
  pthread_mutex_lock (&capsule);
  if(globalAdd(tempoList,get_time(),get_time()+delay, param)){
    puts("change 1");
    setitimer(ITIMER_REAL,&it_val,NULL);
  }
  pthread_mutex_unlock (&capsule);
  

  return 0 ;
}

int timer_cancel (timer_id_t timer_id)
{
  // TODO
    
  return 0; // failure
}

#endif
