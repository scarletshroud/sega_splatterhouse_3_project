#include "list.h"

static struct zombie_list* head; 
static struct zombie_list* current;

struct zombie_list get_zombie_list_head() {
    return *head;
}

struct zombie_list get_zombie_list_current() {
    return *current;
} 

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

void zombie_list_remove(struct zombie_list* head, struct zombie_list* z) {
    struct zombie_list* current = head;
    struct zombie_list* prev = NULL; 

    while (current != NULL) {
        if (current == z) {
            KLog_U1("yep: ", 2);
            if (prev != NULL) {
                prev->next = current->next;
            }
            free(current); 
        }
        prev = current; 
        current = current->next; 
    }    
}

void zombie_list_pop_back() {
    
}

void zombie_list_push_back(struct zombie_list** head, const struct zombie* const z) {
    struct zombie_list* node = (struct zombie_list*)malloc(sizeof(struct zombie_list));
    node->z = z; 
    node->next = (*head);
    (*head) = node; 
} 