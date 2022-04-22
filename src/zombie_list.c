#include "zombie_list.h"

struct zombie_list get_zombie_list_head(struct zombie_list* head) {
    return *head;
}

struct zombie_list get_zombie_list_current(struct zombie_list* head) {
    return;
} 

bool zombie_list_empty(struct zombie_list* head) {
    return head == NULL; 
}

uint8_t zombie_list_size(struct zombie_list* head) {
    uint8_t size = 0; 
    struct zombie_list *current = head;

    for (; current != NULL; current = current->next) {
        size++; 
    }

    return size; 
}

void zombie_list_clear(struct zombie_list** head) {
    struct zombie_list *prev = NULL; 

    while (*head != NULL) {
        prev = (*head); 
        (*head) = (*head)->next;
        free(prev); 
    } 
}

void zombie_list_remove(struct zombie_list** head, struct zombie_list* z) {
    struct zombie_list* current = *head;
    struct zombie_list* prev = NULL; 

    while (current != NULL) {
        if (current == z) {
            if (prev != NULL) {
                prev->next = current->next;
            }
            free(current);
            return;
        }

        prev = current; 
        current = current->next; 
    }    
}

void zombie_list_pop_back(struct zombie_list* head) {
    
}

void zombie_list_push_back(struct zombie_list** head, struct zombie* z) {
    struct zombie_list* node = malloc(sizeof(struct zombie_list));
    node->z = z; 
    node->next = (*head);
    (*head) = node; 
} 

void zombie_print_list(struct zombie_list* head) {
    struct zombie_list* current = head;

    while(current != NULL) {
        KLog_U1("id ", current->z->id);
        current = current->next;
    }
}