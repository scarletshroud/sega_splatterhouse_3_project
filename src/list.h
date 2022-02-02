#ifndef _LIST_H_
#define _LIST_H_

#include "zombie.h"

struct zombie_list {
    struct zombie* z;
    struct node* next;          
};

struct zombie_list get_zombie_list_head();
struct zombie_list get_zombie_list_current();  

void zombie_list_pop_back();
void zombie_list_push_back(struct zombie_list** head, const struct zombie* const z); 
 
void zombie_list_clear();
void zombie_list_remove(struct zombie_list* head, struct zombie_list* z); 

bool zombie_list_empty();
uint8_t zombie_list_size();

#endif