// LISTE_H by emmanoe.delar@etu.u-bordeaux.fr

#ifndef LISTE_H
#define LISTE_H
typedef struct Liste_t *Liste_t;
typedef struct List_ctrl *List_ctrl; 

/* **************************************************************** */
extern List_ctrl liste_ctrl_alloc(void);

extern Liste_t liste_alloc(void);

extern void liste_ctrl_free_elem(List_ctrl elem);

extern void liste_free_elem(Liste_t elem);

extern Liste_t liste_prepend(Liste_t L, int delay, void* param);

extern Liste_t liste_delete_first(Liste_t liste);

extern Liste_t liste_insert_after(Liste_t l, Liste_t p, int delay, void* param, Liste_t next);

extern Liste_t liste_delete_after(Liste_t l, Liste_t p);

extern Liste_t liste_next(Liste_t l);

extern void * liste_param(Liste_t l);

extern int liste_delay(Liste_t l);

/* **************************************************************** */

#endif