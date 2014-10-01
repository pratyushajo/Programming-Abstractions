/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */

#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	// TODO: Fill this in!
    head = NULL;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	// TODO: Fill this in!
    while (head != NULL) {
        cellT *cellToBeDeleted = head;
        head = head->next;
        delete cellToBeDeleted;
    }
}

int DoublyLinkedListPriorityQueue::size() {
	// TODO: Fill this in!
	int size = 0;
    if (isEmpty()) {
        return size;
    }
    else{
        cellT *currElem = head;
        while (currElem != NULL) {
            currElem = currElem->next;
            size += 1;
        }
        return size;
    }
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	// TODO: Fill this in!
	
	return (head == NULL);
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	// TODO: Fill this in!
    cellT *newCellToBeAdded = new cellT;
    newCellToBeAdded->value = value;
    newCellToBeAdded->next = NULL;
    newCellToBeAdded->prev = NULL;
    
    if (isEmpty()) {
        head = newCellToBeAdded;
    }
    
    else{
        newCellToBeAdded->next = head;
        head->prev = newCellToBeAdded;
        head = newCellToBeAdded;
    }
}

string DoublyLinkedListPriorityQueue::peek() {
	// TODO: Fill this in!
	if (isEmpty()) {
        error("Queue is empty");
        return "";
    }
	else return (findMin());
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	// TODO: Fill this in!
    if (isEmpty()) {
        error("Queue is empty");
        return "";
    }
    
    else{
        currCell = head;
        
        string minValue = findMin();
        //Finding the node holding the minimum value.
        while (currCell->value != minValue) {
            currCell = currCell->next;
        }
        
        //If there is only one node in the list.
        if(currCell->next == NULL && currCell->prev == NULL){
            head = NULL;
            delete currCell;
        }
        //If we are deleting the first node
        else if (currCell->prev  == NULL) {
            currCell->next->prev = NULL;
            head = currCell->next;
            delete currCell;
        }
        //If we are deleting the last node
        else if(currCell->next == NULL){
            currCell->prev->next = NULL;
            delete currCell;
        }
        //Deleting a node from the middle of the list.
        else{
            //Splicing the node.
            currCell->next->prev = currCell->prev;
            currCell->prev->next = currCell->next;
            delete currCell;
        }
    
        return minValue;
    }
}

string DoublyLinkedListPriorityQueue::findMin(){
    
    string minValue = head->value;
    
    cellT *minCell = head;
    while (minCell != NULL) {
        if (minCell->value < minValue) {
            minValue = minCell->value;
        }
        minCell = minCell->next;
    }
    return minValue;
}

