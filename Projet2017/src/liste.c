#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "liste.h"

void initCtrl(ctrl c){
    c->debut=NULL;
    c->fin=NULL;
    c->size=0;
}

long headListDelay(ctrl c){
    return c->debut->delay;
}

void* headListParam(ctrl c){
    return c->debut->param;
}


int get_ctrl_size(ctrl c){
    if(c != NULL){
        return c->size;        
    }
    exit(EXIT_FAILURE);
}

list get_ctrl_debut(ctrl c){
    if(c != NULL){
        return c->debut;        
    }
    exit(EXIT_FAILURE);
}

list get_ctrl_fin(ctrl c){
    if(c != NULL){
        return c->fin;        
    }
    exit(EXIT_FAILURE);
}

unsigned long get_list_delay(list l){
    if(l != NULL){
        return l->delay;        
    }
    exit(EXIT_FAILURE);
}

unsigned long get_list_added_time(list l){
    if(l != NULL){
        return l->add_time;        
    }
    exit(EXIT_FAILURE);
}

unsigned long get_list_launch_time(list l){
    if(l != NULL){
        return l->launch_time;        
    }
    exit(EXIT_FAILURE);
}

void* get_param(list l){
    if(l != NULL){
        return l->param;        
    }
    exit(EXIT_FAILURE);
}

list get_next(list l){
    if(l != NULL){
        return l->next;        
    }
    exit(EXIT_FAILURE);
}

list get_prev(list l){
    if(l != NULL){
        return l->prev;        
    }
    exit(EXIT_FAILURE);
}

list alloc(unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param){
    
    list new = (list)malloc(sizeof(struct eventList));
    new->delay=delay;
    new->param=param;
    new->add_time=add_time;
    new->launch_time = launch_time;
    
    return new;
}

void addTop(ctrl c,unsigned long add_time,unsigned long launch_time, unsigned long delay, void* param){
  list new = alloc(add_time,launch_time,delay,param);
    
    if(get_ctrl_size(c)== 0){
        c->fin = new;
        new->next = NULL;
        
    }else{
        new->next = c->debut;
        c->debut->prev=new;
    }
    c->debut = new;
    new->prev = NULL;
    
    c->size++;
}

void addAfter(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param,list after){
    
  list new = alloc(add_time,launch_time,delay,param);
    if(get_ctrl_size(c)== 0){
        c->fin = new;
        c->debut = new;
        new->prev = NULL;
        new->next = NULL;
    }else{
        new-> next = after->next;
        new->prev = after;
        if(after->next != NULL){
            after->next->prev = new;
        }else{
            c->fin = new;
        }
        after->next = new;
    }
    
    c->size++;
}

void addBefore(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param,list before){
  list new = alloc(add_time,launch_time,delay,param);
    if(get_ctrl_size(c)== 0){
        c->fin = new;
        c->debut = new;
        new->prev = NULL;
        new->next = NULL;
    }else{
        
        new-> next = before;
        new->prev = before->prev;
        if(get_prev(before)!=NULL){
            before->prev->next=new;
        }else{
            c->debut = new;
        }
        before->prev = new;
        
    }
    
    c->size++;
}

void addBottom(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param){
  list new = alloc(add_time,launch_time,delay,param);
    if(get_ctrl_size(c)== 0){
        c->fin = new;
        c->debut = new;
        new->prev = NULL;
    }else{
        new->prev = c->fin;
        c->fin->next = new;
        c->fin=new;
    }
    new->next = NULL;
    c->size++;
}

void delEmpty(ctrl c){
    
    c->debut = NULL;
    c->fin = NULL;
    c->size--;
}

void delTop(ctrl c){

  if( c != NULL ){
    list del = c->debut;
    if(c->size > 1){
      c->debut = del->next;
      c->debut->prev = NULL;
      c->size--;
    }else{
      delEmpty(c);
    }
    free(del);
  }
  
}

int globalAdd(ctrl c,unsigned long add_time,unsigned long launch_time,unsigned long delay, void* param){
    
    list tmp = get_ctrl_debut(c);
    int action = 0;
    while(tmp != NULL && launch_time > get_list_launch_time(tmp)){
        tmp = get_next(tmp);
    }
    
    if(tmp == NULL && get_ctrl_size(c) == 0){
        addTop(c,add_time,launch_time,delay,param);
        return 1;
    }else if(tmp != NULL ){
        if(launch_time <= get_list_launch_time(tmp) && tmp == get_ctrl_debut(c)){
            addTop(c,add_time,launch_time,delay,param);
            return 1;
        }else{
            addAfter(c,add_time,launch_time,delay,param,tmp);
        }
    }else{
        addBottom(c,add_time,launch_time,delay,param);
    }
    return action;
}
