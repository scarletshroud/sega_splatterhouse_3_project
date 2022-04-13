#ifndef _LIST_H_
#define _LIST_H_

#include "zombie.h"

struct zombie_list {
    struct zombie* z;
    struct zombie_list* next;          
};

struct zombie_list get_zombie_list_head(struct zombie_list* head);
struct zombie_list get_zombie_list_current(struct zombie_list* head);  

void zombie_list_pop_back(struct zombie_list* head);
void zombie_list_push_back(struct zombie_list** head, struct zombie* z); 
 
void zombie_list_clear(struct zombie_list** head);
void zombie_list_remove(struct zombie_list** head, struct zombie_list* z); 

bool zombie_list_empty(struct zombie_list* head);
uint8_t zombie_list_size(struct zombie_list* head);

void zombie_print_list(struct zombie_list* head) {
    struct zombie_list* current = head;

    while(current != NULL) {
        KLog_U1("id ", current->z->id);
        current = current->next;
    }
}

#endif