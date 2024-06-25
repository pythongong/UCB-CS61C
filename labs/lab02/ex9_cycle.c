#include <stddef.h>
#include "ex9_cycle.h"

int ll_has_cycle(node *head) {
    node *slow_pointer;
    node *fast_pointer;
    slow_pointer = head;
    fast_pointer = head;
    while (slow_pointer != NULL && fast_pointer != NULL) {
        slow_pointer = slow_pointer->next;
        if (fast_pointer->next == NULL) {
            return 0;
        }
        fast_pointer = fast_pointer->next->next;
        if (slow_pointer == fast_pointer)
        {
            return 1;
        }
        
    }
    return 0;
    
}
