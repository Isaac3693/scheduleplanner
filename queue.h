#ifndef QUEUE_FILE
#define QUEUE_FILE

#include <stdlib.h>
#include <stdio.h>

typedef struct NODE {
    int num;
    struct NODE *next;
} *node;

typedef struct QUEUE {
    node head;
} *queue;

int enqueue(node, queue);
queue new_queue(void);
node new_node(int);
int peek(queue, int *);
int dequeue(queue, int *); //must have the data type of what it is pointing to, so if it has a struct s inside the node, return a struct s
void destroy_queue(queue);
void destroy_node(node);

node new_node(int n) {
    node t = (node)malloc(sizeof(struct NODE));
    //Check if the malloc operation returns a null operator, use it to check for any failures
    if (t == NULL) return NULL;
    t->num = n;
    t->next = NULL;
    return t;
}

queue new_queue(void) {
    queue q = (queue)malloc(sizeof(struct QUEUE));
    //Once again, check for any failures and return for error handling
    if (q == NULL) return NULL;
    q->head = NULL;
    return q;
}

void destroy_node(node n) {
    if (n != NULL) free(n);
}

void destroy_queue(queue q) {
    node t = q->head;
    while (q->head != NULL) {
        q->head = q->head->next;
        free(t);
        t = q->head;
    }
    free(q);
}

int enqueue(node n, queue q) {

    //Special case in which q->head points to NULL

    if (q->head == NULL) {
        q->head = n;
        return 0;
    }

    //Iterate through the queue so long as t doesn't point to NULL, if the next element of t is NULL, assign that to n
    for (node t = q->head; t != NULL; t = t->next) if (t->next == NULL) {
        t->next = n;
        return 0;
    }

    //Fails to assign it to end of list, for whatever reason
    return 1;
}

int peek(queue q, int *n) {

    //Check if the head is null, and then if not, then assign whatever n is pointing at to the head's element.
    if (q->head == NULL) {
        fprintf(stderr, "Empty list, returning 0\n");
        return 1;
    }
    *n = q->head->num;
    return 0;

}

int dequeue(queue q, int *n) {
    if (q->head == NULL) {
        fprintf(stderr, "Empty list, returning 0\n");
        return 1;
    }
    node t = q->head;
    *n = q->head->num;
    q->head = q->head->next;
    destroy_node(t);
    return 0;
}

void print_queue(queue q) {
    printf("{");
    for (node t = q->head; t != NULL; t = t->next) {
        if (t->next != NULL) printf("%d, ", t->num);
        else printf("%d", t->num);
    }
    printf("}\n");
}

#endif