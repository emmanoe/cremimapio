#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "liste.h"

void initCtrl(ctrl c){
    c->debut=NULL;
    c->fin=NULL;
    c->size=0;
}

list alloc(long  delay, void* param){
    
    list new = (list)malloc(sizeof(struct eventList));
    new->delay=delay;
    new->param=param;
    
    return new;
}

void addTop(ctrl c,long delay, void* param){
    list new = alloc(delay,param);
    
    if(c->size == 0){
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

void addAfter(ctrl c,long delay, void* param,list after){
    
    list new = alloc(delay,param);
    if(c->size == 0){
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

void addBefore(ctrl c,long delay, void* param,list before){
    list new = alloc(delay,param);
    if(c->size == 0){
        c->fin = new;
        c->debut = new;
        new->prev = NULL;
        new->next = NULL;
    }else{
        
        new-> next = before;
        new->prev = before->prev;
        if(before->prev!=NULL){
            before->prev->next=new;
        }else{
            c->debut = new;
        }
        before->prev = new;
        
    }
    
    c->size++;
}

void addBottom(ctrl c,long delay, void* param){
    list new = alloc(delay,param);
    if(c->size == 0){
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

void globalAdd(ctrl c,long delay, void* param){
    
    list tmp = c->debut;
    
    while(tmp != NULL && delay > tmp->delay ){
        tmp = tmp->next;
    }
    
    if(tmp == NULL && c->size == 0){
        addTop(c,delay,param);
    }else if(tmp != NULL ){
        if(delay <= tmp->delay && tmp == c->debut){
            addTop(c,delay,param);
        }else{
            addBefore(c,delay,param,tmp);
        }
    }else{
        addBottom(c,delay,param);
    }
}

void* headList(ctrl c){
    
    void *param = c->debut->param;
    delTop(c);
    return param;
}

