#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

struct List_ctrl{
	Liste_t debut;
	Liste_t fin;
	int nb;
};

struct Liste_t{
	int delay;
	void* param;
	struct Liste_t* next;
};

List_ctrl
liste_ctrl_alloc(void){
	List_ctrl l;
	assert(l = malloc(sizeof(struct List_ctrl)));
	l->debut = NULL;
	l->fin = NULL;
	return l;
}

Liste_t
liste_alloc(void){
	Liste_t l;
	assert(l = malloc(sizeof(struct Liste_t)));
	l->next = NULL;
	return l;
}

void
liste_ctrl_free_elem(List_ctrl elem){
	if(elem != NULL)
		free(elem);
}

void
liste_free_elem(Liste_t elem){
	if(elem != NULL)
		free(elem);
}

Liste_t
liste_prepend(Liste_t L, int delay, void* param){
	Liste_t new = liste_alloc();
	assert(new);
	new->delay = delay;
	new->param = param;
	new->next = L;
	return new;
}

Liste_t 
liste_delete_first(Liste_t liste){
	if(liste != NULL){
		Liste_t tmp = liste;
		liste = liste->next;
		liste_free_elem(tmp);
	}
	return liste;
}

Liste_t 
liste_insert_after(Liste_t l, Liste_t p, int delay, void* param, Liste_t next){
	if(p!=NULL){
		Liste_t new = liste_alloc();
		new->delay = delay;
		new->param = param;
		new->next = p->next;
		p->next = new;
	}
	return l;
}

Liste_t 
liste_delete_after(Liste_t l, Liste_t p){
	if(p!=NULL){
		if(p->next!=NULL){
			Liste_t tmp = p->next;
			p->next=p->next->next;
			liste_free_elem(tmp);
		}
	}
	return l;
}

Liste_t 
liste_next(Liste_t l){
	if(l->next != NULL){
		return l->next;
	}
	return NULL;
}

void * liste_param(Liste_t l){
	assert(l);
	return(l->param);
}

int liste_delay(Liste_t l){
	assert(l);
	return(l->delay);
}