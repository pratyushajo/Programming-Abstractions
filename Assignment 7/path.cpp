/*
 * File: path.cpp
 * --------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file implements the path.h interface.
 */

/////////////////////////////////////////////////////////////////////
// Your task in this part of the assignment is to write the code   //
// that implements your path.h interface.  For the benefit of      //
// future programmers who will need to work on this interface,     //
// you must also write an extended comment (presumably replacing   //
// this to-do box) that documents the design decisions you made    //
// in structuring the Path class as you did.                       //
/////////////////////////////////////////////////////////////////////

// [TODO: implement the methods in your Path class]

#include "path.h"

Path::Path(){
    
}

void Path::clear(){
    path.clear();
    pathCost = 0;
}

void Path::add(Arc * arc){
    path.add(arc);
    pathCost += arc->cost;
}

string Path::toStringPath(){
    
    string toStringPath = "";
    Node * endNode;
    for (Arc * arc: path){
        toStringPath += arc->start->name;
        toStringPath += " -> ";
        endNode = arc->finish;
    }
    toStringPath += endNode->name;
    
    return toStringPath;
}

double Path::getPathCost(){
    return pathCost;
}

int Path::size(){
    return path.size();
}

void Path::remove(int position){
    Arc * arc = path[position];
    pathCost = pathCost - arc->cost;
    path.removeAt(position);
}

Node * Path::getNode(int position){
    Arc * arc = path[position];
    Node * node = arc->finish;
    return node;
}

Vector<Arc *> Path::getArcs(){
    return path;
}