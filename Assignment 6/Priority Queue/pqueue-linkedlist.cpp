/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	// TODO: Fill this in!
    head = tail = NULL;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	// TODO: Fill this in!
    //delete all cells
    while (head != NULL) {
        cellT *cellToBeDeleted = head;
        head = head->next;
        delete cellToBeDeleted;
    }
}

int LinkedListPriorityQueue::size() {
	// TODO: Fill this in!
	int size = 0;
    if (isEmpty()) {
        return size;
    }
    else{
        cellT *nextCell = new cellT;
        nextCell = head;
        while (nextCell != NULL) {
            nextCell = nextCell->next;
            size++;
        }
        return size;
    }
}

bool LinkedListPriorityQueue::isEmpty() {
	// TODO: Fill this in!
	
	return (head == NULL);
}

void LinkedListPriorityQueue::enqueue(string value) {
	// TODO: Fill this in!
    cellT *newCell = new cellT;
    newCell->value = value;
    newCell->next = NULL;
    
    if (isEmpty()) {
        head = tail = newCell;
        return;
    }
    else{
        //Attaching as the first node.
        if (newCell->value <= head->value) {
            newCell->next = head;
            head = tail = newCell;
            return;
        }
        
        else{
            //Starting from the first node, checking the position to insert the new node
            tail = head;
            while (tail->next != NULL) {
                //If the next value is greater the value of the new cell, we dont advance the tail position.
                if (tail->next->value > newCell->value) break;
                //Else, advance tail.
                else if(newCell->value > tail->value) {
                    tail = tail->next;
                }
            }
            //Attach the new node.
            newCell->next = tail->next;
            tail->next = newCell;
            return;
        }
    }
}

string LinkedListPriorityQueue::peek() {
	// TODO: Fill this in!
	if (isEmpty()){
        error("Queue is empty");
        return "";
    }
    else{
        string value = head->value;
        return value;
    }
}

string LinkedListPriorityQueue::dequeueMin() {
	// TODO: Fill this in!
	if (isEmpty()) {
        error("Queue is empty");
        return "";
    }
    else{
        string value = head->value;
        cellT *old = head;
        head = head->next;
        delete old;
        return value;
    }
}

