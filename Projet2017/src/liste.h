// LISTE_H by emmanoe.delar@etu.u-bordeaux.fr

#ifndef LISTE_H
#define LISTE_H
typedef struct eventList* list;
typedef struct ctrlList* ctrl;

/* **************************************************************** */
extern void initCtrl(ctrl c);

extern list alloc(int delay, void* param);

extern void addTop(ctrl c,int delay, void* param);

extern void addAfter(ctrl c,int delay, void* param,list after);

extern void addBefore(ctrl c,int delay, void* param,list before);

/*Add a new element at the end of the list and incr c */
extern void addBottom(ctrl c,int delay, void* param);

extern void delEmpty(ctrl c);

extern void delTop(ctrl c);

/*Add a new element according to his delay*/
extern void globalAdd(ctrl c,int delay, void* param);

extern void* headList(ctrl c);

/* **************************************************************** */

#endif