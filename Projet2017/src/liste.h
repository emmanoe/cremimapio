// LISTE_H by emmanoe.delar@etu.u-bordeaux.fr

#ifndef LISTE_H
#define LISTE_H

struct eventList{
  unsigned long delay;
  unsigned long add_time;
  unsigned long launch_time;
  void* param;
  struct eventList* next;
  struct eventList* prev;
};
typedef struct eventList* list;

struct ctrlList{
    list debut;
    list fin;
    int size;
};
typedef struct ctrlList* ctrl;

/* **************************************************************** */
extern void initCtrl(ctrl c);

extern list alloc(unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param);

extern void addTop(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param);

extern void addAfter(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param,list after);

extern void addBefore(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param,list before);

/*Add a new element at the end of the list and incr c */
extern void addBottom(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param);

extern void delEmpty(ctrl c);

extern void delTop(ctrl c);

/*Add a new element according to his delay*/
extern int globalAdd(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param);

extern void* headListParam(ctrl c);

extern long headListDelay(ctrl c);

/* **************************************************************** */

#endif
