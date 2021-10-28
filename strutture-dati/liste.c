/* 
    IMPLEMENTAZIONE C LISTE CONCATENATE
 */

#include <stdio.h>
#include <stdlib.h>

/* 
    Node struct and custom type definition
 */
typedef int Item;
typedef struct node node_t, *link;

// List node structure
struct node {
    Item val;
    link next;
};

/** 
 *  * New node function
 *  @param Item Data of the new node
 *  @param link Pointer to the next node
 *  @return Pointer to the new node
 */
link newNode(Item val, link next) {
    link t;

    // Allocating memory
    t = (node_t *) malloc(sizeof(node_t));
    if (t == NULL)
        exit(2);

    // Passing data to the new node
    t->val = val;
    t->next = next;

    // Return a pointer to the new node
    return t;
    
}

/**
 *  * Insert node at head of list
 *  @param Item Data of the node
 *  @param link Head of the list
 *  @return Pointer to the new head of the list
 */
link insertNodeAtHead(Item val, link head) {

    return newNode(val, head);

}

/**
 *  * Insert node at head of list (by pointer)
 *  @param Item Data of the node
 *  @param link* Pointer to head of the list
 *  @return --
 */
void insertNodeAtHeadP(Item val, link *head) {

    *head = newNode(val, *head);

}