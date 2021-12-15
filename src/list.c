#include "list.h"

bool zombie_list_empty() {
    return head == NULL; 
}

uint8_t zombie_list_size() {
    uint8_t size = 0; 
    struct zombie_list *current = head; 

    for (; current != NULL; current = current->next) {
        size++; 
    }

    return size; 
}

void zombie_list_clear() {
    struct zombie_list *current = head;
    struct zombie_list *prev = NULL; 

    while (current != NULL) {
        prev = current; 
        current = current->next;
        free(prev->z);
        free(prev); 
    }
} 


void zombie_list_pop_back() {
    

}

void zombie_list_push_back(struct zombie *z) {
    
} 