#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "linked_list.h"
#include "iterator.h"

#define NOT_SUPPORTED()  \
do  {  \
    fprintf(stderr, "The simple linked list does not support %s\n",__func__); \
    assert(false); \
    } while (0)\

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_func) {
    ioopm_list_t *new_list = calloc(1, sizeof(ioopm_list_t));
    new_list->ioopm_eq_function = eq_func;
    return new_list;
    
}

void ioopm_linked_list_destroy(ioopm_list_t *list) {
    link_t *current = list->head;

    while (current) {
        link_t *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}

link_t *link_create(elem_t value, link_t *next) {
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
} 

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value) {
    if (list->size == 0) {
        list->last = link_create(value, NULL);
        list->head = list->last;
    } else {
        link_t *prev_last_link = list->last;
        link_t *new_last_link = link_create(value, NULL);
        prev_last_link->next = new_last_link;
        list->last = new_last_link;
    }
    list->size ++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value) {
    if (list->size == 0) {
        list->head = link_create(value, NULL);
        list->last = list->head;
    } else {
        link_t *current_head = list->head;
        list->head = link_create(value, current_head);
    }
    list->size++;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element) {
    link_t *current = list->head;
    bool value_exist = false;
    while (value_exist == false && current != NULL) {
        if (list->ioopm_eq_function(current->value, element)) {
            value_exist = true;
            return value_exist;
        }
        current = current->next;
    }
    return value_exist;
}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value) {
    if (index == list->size || index > list->size) {
        ioopm_linked_list_append(list, value);
    }
    else if (index == 0) {
        link_t *old_first_element = list->head;
        list->head = link_create(value, old_first_element);
        list->size++;
    } else {
        link_t *current = list->head;
        for (size_t i = 1; i != index; i++) {
            current = current->next;
        }
        link_t *tmp = current->next;
        current->next = link_create(value, tmp);
        list->size++;
    }
}

static link_t *prev_link_in_list(ioopm_list_t *lst, size_t index) {
    link_t *current = lst->head;
    for (size_t i = 0; i < index - 1; i++) {
        current = current->next;
    }  
    return current;
}
 

elem_t ioopm_linked_list_remove(ioopm_list_t *lst, size_t index) {
    assert((index >= 0 && index <= lst->size) && ("Du kan endast ta bort inom listan"));
    if (index == 0) {
        link_t *tmp = lst->head;
        elem_t value_to_del = tmp->value;
        lst->head = lst->head->next;
        free(tmp);
        lst->size--;
        return value_to_del;
    } else if (index == lst->size - 1) {
        link_t *current = prev_link_in_list(lst, index);
        link_t *tmp = lst->last;
        elem_t value_to_del = tmp->value;
        lst->last = current;
        free(tmp);
        current->next = NULL;
        lst->size--;
        return value_to_del;
    } else {
        link_t *current = prev_link_in_list(lst, index);
        link_t *tmp = current->next;
        elem_t value_to_del = tmp->value;
        current->next = tmp->next;
        free(tmp);
        lst->size--;
        return value_to_del;
    }   

}

elem_t ioopm_linked_list_get(ioopm_list_t *lst, size_t index) {
    assert((index >= 0 && index <= lst->size) && ("Du kan endast hämta value inom listan"));
    link_t *current = lst->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current->value;
}

size_t ioopm_linked_list_size(ioopm_list_t *list) {
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list) {
    return list->size == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list) {
    link_t *head = list->head;
    link_t *next = head->next;
    free(head);
    list->size--;
    while (next != NULL) {
        head = next;
        next = head->next;
        free(head);
        list->size--;
    }
    list->head = NULL;
    list->last = NULL;
}

bool is_even(elem_t key_ignored, elem_t value, void *extra_ignored) {
    return value.number % 2 == 0;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, void *extra) {
    bool predicate = false;
    link_t *current = list->head;
    elem_t not_used = {.number = 0};
    while (predicate == false && current != NULL) {
        if(prop(not_used, current->value, extra)) {
            predicate = true;
            return predicate;
        }
        current = current->next;
    }
    return predicate;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, void *extra) {
    bool predicate = true;
    link_t *current = list->head;
    elem_t not_used = {.number = 0};
    while(predicate && current != NULL) {
        if(prop(not_used, current->value, extra) == false) {
            predicate = false;
            return predicate;
        }
        current = current->next;
    }
    return predicate;
}

void replace(elem_t key_ignored, elem_t *value, void *extra) {
  elem_t *ptr_to_void = extra;
  *value = *ptr_to_void;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function fun, void *extra) {
    link_t *current = list->head;
    elem_t not_used = {.number = 0};
    while (current != NULL) {
        fun(not_used, &current->value, extra);
        current = current->next;
    }
}

ioopm_list_iterator_t *ioopm_create_list_iterator(ioopm_list_t *lst) {
    ioopm_list_iterator_t *iterator = calloc(1, sizeof(ioopm_list_iterator_t));
    iterator->current = lst->head;
    iterator->list = lst;
    return iterator;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter) {
    return iter->current->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter) {
    if (iter->current->next != NULL) {
        iter->current = iter->current->next;
        return iter->current->value;
    } else {
        return iter->current->value;
    }
    
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter) {
    return iter->current->value;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter) {
    iter->current = iter->list->head;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter) {
    free(iter);
}