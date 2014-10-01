/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
	// TODO: Fill this in!
    
}

VectorPriorityQueue::~VectorPriorityQueue() {
	// TODO: Fill this in!
    
}

int VectorPriorityQueue::size() {
	// TODO: Fill this in!
	
	return priorityVector.size();
}

bool VectorPriorityQueue::isEmpty() {
	// TODO: Fill this in!
	
	return (priorityVector.size() == 0);
}

void VectorPriorityQueue::enqueue(string value) {
	// TODO: Fill this in!
    priorityVector.push_back(value);
}

string VectorPriorityQueue::peek() {
	// TODO: Fill this in!
    if (isEmpty()) {
        error("Queue is empty");
        return "";
    }
    else{
        string peekValue = dequeueMin();
        enqueue(peekValue);
        return peekValue;
    }
}

string VectorPriorityQueue::dequeueMin() {
	// TODO: Fill this in!
    if (isEmpty()) {
        error("No elements found in the queue.");
    }
    else{
        int position = 0;
        minVector = priorityVector[0];
        for (int i = 1 ; i < priorityVector.size(); i++) {
            if (minVector > priorityVector[i]) {
                minVector = priorityVector[i];
                position = i;
            }
        }
        priorityVector.removeAt(position);
    }
	return minVector;
}

