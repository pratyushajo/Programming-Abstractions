/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */

#include <iostream.h>
#include "pqueue-heap.h"
#include "error.h"
using namespace std;

const int START_SIZE = 4;

HeapPriorityQueue::HeapPriorityQueue() {
	// TODO: Fill this in!
    arr = new string[START_SIZE];
    numAllocated = START_SIZE;
    numUsed = 0;
    arr[0] = "";
}

HeapPriorityQueue::~HeapPriorityQueue() {
	// TODO: Fill this in!
    delete[] arr;
}

int HeapPriorityQueue::size() {
	// TODO: Fill this in!
	
	return numUsed;
}

bool HeapPriorityQueue::isEmpty() {
	// TODO: Fill this in!
	
	return (numUsed == 0);
}

void HeapPriorityQueue::enqueue(string value) {
	// TODO: Fill this in!
    if (isEmpty()) {
        arr[1] = value;
        numUsed++;
    }
    else{
        int lastVacantSpot = size()+1;
        if (lastVacantSpot == numAllocated)
            doubleCapacity();
        arr[lastVacantSpot] = value;
        numUsed++;
        if(size() > 1)
            bubbleUp(lastVacantSpot, lastVacantSpot/2);
    }
}


//Increasing the capacity of the array if the number of elements increases than the maximum size
//Creates a new array with double the capacity of the existing array, copies the elements onto the
//new array and deletes the old one.
void HeapPriorityQueue::doubleCapacity(){
    string *bigger = new string[numAllocated * 2];
    for (int i = 0; i <= numUsed; i++) {
        bigger[i] = arr[i];
    }
    delete[] arr;
    arr = bigger;
    numAllocated *= 2;
}

void HeapPriorityQueue::bubbleUp(int newElemPos, int parentNodePos){
    while(arr[newElemPos] < arr[parentNodePos] && parentNodePos >= 1){
            string tempNode = arr[newElemPos];
            arr[newElemPos] = arr[parentNodePos];
            arr[parentNodePos] = tempNode;
            newElemPos = parentNodePos;
            parentNodePos = newElemPos/2;
    }
}

string HeapPriorityQueue::peek() {
	// TODO: Fill this in!
    if (isEmpty()) {
        error("Heap is empty");
        return "";
    }

	else return arr[1];
}

string HeapPriorityQueue::dequeueMin() {
	// TODO: Fill this in!
	if (isEmpty()) {
        error("Heap is empty");
        return "";
    }
    else{
        if (size() == 1) {
            string topElem = arr[1];
            arr[1] = "";
            numUsed = 0;
            return topElem;
        }
        else{
            int n = numUsed;
            //Bubble down the parent node.
            string tempNode = arr[n];
            arr[n] = arr[1];
            arr[1] = tempNode;
            string topElem = arr[n];
            arr[n] = "";
            numUsed = numUsed - 1;
            //Reshuffling the tree to bring the smallest string as the parent node.
            bubbleDown();
            return topElem;
        }
    }
}

void HeapPriorityQueue::bubbleDown(){
    /*int parentNode = 1;
    int leftChildNode = 2 * parentNode;
    int rightChildNode = 2 * parentNode + 1;
    while (arr[parentNode] > arr[leftChildNode] || arr[parentNode] > arr[rightChildNode]) {
        
        if(arr[leftChildNode] < arr[rightChildNode] && arr[parentNode] > arr[leftChildNode]
           && leftChildNode <= size()){
            string tempNode = arr[leftChildNode];
            arr[leftChildNode] = arr[parentNode];
            arr[parentNode]  = tempNode;
            parentNode = leftChildNode;
        }
        else if (arr[parentNode] > arr[rightChildNode] && rightChildNode <= size()){
            string tempNode = arr[rightChildNode];
            arr[rightChildNode] = arr[parentNode];
            arr[parentNode]  = tempNode;
            parentNode = rightChildNode;
        }
        leftChildNode = 2 * parentNode;
        rightChildNode = 2 * parentNode + 1;
    }*/
    
    int n = numUsed;
    while (true) {
        if (n == 1) break;
        if (arr[1] > arr[n]) {
            string tempNode = arr[n];
            arr[n] = arr[1];
            arr[1] = tempNode;
        }
        n = n-1;
    }
}


