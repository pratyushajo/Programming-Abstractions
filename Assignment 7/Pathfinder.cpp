/*
 * File: Pathfinder.cpp
 * --------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Pathfinder application
 * on Assignment #6.
 */
 
// [TODO: complete this implementation]

#include <iostream>
#include <fstream>
#include <string>
#include "filelib.h"
#include "console.h"
#include "graphtypes.h"
#include "gpathfinder.h"
#include "graph.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "strlib.h"
#include "Vector.h"
#include "pqueue.h"
#include "path.h"
using namespace std;

/* Function prototypes */

void welcome();
void quitAction();
void readFile(SimpleGraph * graph);
void readAllNodeInformation(ifstream & infile, SimpleGraph * graph);
void readOneNode(string line, SimpleGraph * graph);
void readAllArcInformation(ifstream & infile, SimpleGraph * graph);
void readOneArc(string line, SimpleGraph * graph);
void clearAction(SimpleGraph* & graph);
void dijkstraAction(SimpleGraph* & graph);
void minSpanningTree(SimpleGraph* & graph);
bool checkValidNode(GPoint pt, SimpleGraph * graph, Node* & point);
Path findShortestPath(Node * start, Node * finish);
double getPathCost(Vector<Arc *> & path);
void highlightPath(Path path, SimpleGraph * graph);
void dimAllArcs(SimpleGraph * graph);
void toStringPath(Vector<Arc *> path);
void MapAction(SimpleGraph* & graph);
void initializeGraph(SimpleGraph* & graph);
void getNodes(Node * start);

/* Main program */

int main() {
   initPathfinderGraphics();

    SimpleGraph * graph = new SimpleGraph;
    
    addButton("Map", MapAction, graph);
    addButton("Djikstra", dijkstraAction, graph);
    addButton("Kruskal", minSpanningTree, graph);
    addButton("Quit", quitAction);
    
    welcome();
    readFile(graph);
    
   pathfinderEventLoop();
    
   return 0;
}

//Giving a cheery welcome message
void welcome(){
    cout << "This masterful piece of work is a graph extravaganza!" << endl;
    cout << "The main attractions include a lovely visual presentation" << endl;
    cout << "of the graph along with an implementation of Dijkstra's" << endl;
    cout << "shortest path algorithm and Kruskal's computation of " << endl;
    cout << "a minimal spanning tree.  Enjoy!" << endl;
}

/* Sample callback function */
void quitAction() {
   exit(0);
}

void MapAction(SimpleGraph* & graph){
    clearAction(graph);
    readFile(graph);
}

void clearAction(SimpleGraph* & graph){
    graph->nodes.clear();
    graph->arcs.clear();
    graph->nodeMap.clear();
    drawPathfinderMap("");
    repaintPathfinderDisplay();
}

// Reading the file and storing the node and arc information and populating
// into the respective data structures.
void readFile(SimpleGraph * graph){
    ifstream infile;
    
    //Asking user to enter the file to be read.
    promptUserForFile(infile, "Input file: ");
    
    //Storing the name of the image file (first line in the text file) into fileName.
    string fileName;
    getline(infile, fileName);
    drawPathfinderMap(fileName);
    
    //Initialising the scanner to read the file.
    // Reading NODES
    // All the nodes start from here
    readAllNodeInformation(infile, graph);
    
    // Reading ARCS
    readAllArcInformation(infile, graph);
    
    infile.close();
}

//This function takes in all the node information.
void readAllNodeInformation(ifstream & infile, SimpleGraph * graph){
    
    string line;
    
    //returns the line containing "NODES" which describes the start of node information
    getline(infile, line);
    
    //Infile is the pointer to the start of the line which describes all the nodes
    while (line != "ARCS") {
      // Read One Node, allocate memory for it and populate the appropriate fileds
        getline(infile,line);
        readOneNode(line, graph);
    }
        
}

//This function takes in one line of node information, tokenizes it and stores in respective memory location
void readOneNode(string line, SimpleGraph * graph){
   // Read this one line, tokenize it, alllocate memory and store the values in appropriate fields
    TokenScanner scanner(line);
    scanner.scanNumbers();
    scanner.ignoreWhitespace();
    
    Node * newNode = new Node;
    newNode->name = scanner.nextToken();
    double x,y;
    //Converting the next tokens into integer type.
    //atoi gives the type casting from string to integer, c_str() converts the string into the C type string
    x = atoi(scanner.nextToken().c_str());
    y = atoi(scanner.nextToken().c_str());
    
    //Storing the node location.
    GPoint point(x, y);
    newNode->loc = point;
    
    graph->nodes.add(newNode);
    graph->nodeMap.put(newNode->name, newNode);
    //Drawing the node on the graphics window.
    drawPathfinderNode(point, NODE_COLOR, newNode->name);
}

//This function takes in all the arc information
void readAllArcInformation(ifstream & infile, SimpleGraph * graph){
    string line;
    
    while (getline(infile,line)) {
        readOneArc(line, graph);
    }
}

//This function reads one line, tokenizes it and stores in the respective memory location
void readOneArc(string line, SimpleGraph * graph){
    
    // Read this one line, tokenize it, alllocate memory and store the values in appropriate fields
    TokenScanner scanner(line);
    scanner.scanNumbers();
    scanner.ignoreWhitespace();
    
    string startNodeName = scanner.nextToken();
    string endNodeName = scanner.nextToken();
    
    //Finding the nodes in the map.
    Node * startNode = graph->nodeMap.get(startNodeName);
    Node * endNode = graph->nodeMap.get(endNodeName);
    
    //Creating an arc
    Arc * newArc = new Arc;
    newArc->start = startNode;
    newArc->finish = endNode;
    newArc->cost = atoi(scanner.nextToken().c_str());
    startNode->arcs.add(newArc);
    
    //Creating a bi-directional arc
    Arc * returnArc = new Arc;
    returnArc->start = endNode;
    returnArc->finish = startNode;
    returnArc->cost = newArc->cost;
    endNode->arcs.add(returnArc);
    
    //Adding the arcs to the graph.
    graph->arcs.add(newArc);
    graph->arcs.add(returnArc);
    //Drawing the arc on the graphics window.
    drawPathfinderArc(startNode->loc, endNode->loc, ARC_COLOR);
}

//Asking the user to enter the start and end locations which is passed onto the Dijkstra's algorithm
// to find the shortest path between the two nodes.
void dijkstraAction(SimpleGraph* & graph){
    
    initializeGraph(graph);
    
    GPoint pt;
    Node * startLoc;
    Node * endLoc;
    bool found = false;
    
    //Finding the start node
    while (!found) {

        //Asking the user to click on the start location
        cout << "Click on starting location... ";
        //Waiting for user click
        pt = getMouseClick();
        found = checkValidNode(pt, graph, startLoc);
        
        //If clicked on any point on graphics window apart from the place nodes.
        if (!found) {
            cout << "Invalid click. Please try again." << endl;
        }
    }
    
    cout << endl;
    found = false;
    
    //Finding the end node.
    while (!found) {
        
        cout << "Click on the ending location...";
        pt = getMouseClick();
        found = checkValidNode(pt, graph, endLoc);
        
        if (!found) {
            cout << "Invalid click. Please try again." << endl;
        }
    }
    cout << endl;
    
    Path path = findShortestPath(startLoc, endLoc);
    
    highlightPath(path, graph);
}

//Initializes the graph to original look.
void initializeGraph(SimpleGraph* & graph){
    repaintPathfinderDisplay();
    foreach(Arc * arc in graph->arcs){
        drawPathfinderArc(arc->start->loc, arc->finish->loc, ARC_COLOR);
    }
    
    foreach(Node * node in graph->nodes){
        drawPathfinderNode(node->loc, NODE_COLOR);
        
    }
}

bool checkValidNode(GPoint pt, SimpleGraph * graph, Node* & point){
    
    //Checking through the graph to find if it corresponds to a valid location.
    for (Node * node : graph->nodes) {
        GPoint nodeLoc = node->loc;
        double x = nodeLoc.getX();
        double y = nodeLoc.getY();
        
        if (pt.getX() < x + NODE_RADIUS && pt.getX() > x - NODE_RADIUS) {
            if (pt.getY() < y + NODE_RADIUS && pt.getY() > y - NODE_RADIUS) {
                point = node;
                //Highlighting the node.
                drawPathfinderNode(nodeLoc, HIGHLIGHT_COLOR);
                
                return true;
            }
        }
    }
    
    return false;
    
}

//Implementing Dijkstra's Algorithm to find the shortest path between two nodes.
Path findShortestPath(Node * start, Node * finish){
    
    Path path;
    path.clear();
    PriorityQueue<Path> queue;
    Map<string, double> fixed;
    
    cout << endl << "Finding shortest path using Dijkstra... " << endl;
    
    while (start != finish) {
        if (!fixed.containsKey(start->name)) {
            
            cout << "Fix the distance to " << start->name << " at " << path.getPathCost() << endl;
            
            fixed.put(start->name, path.getPathCost());
            
            cout << "Process arcs out of " << start->name << " (";
            getNodes(start) ;
            cout << ")" << endl;
            
            for (Arc * arc : start->arcs){
                    if (!fixed.containsKey(arc->finish->name)) {
                        path.add(arc);
                        
                        cout << "Enqueue the path: " << path.toStringPath();
                        cout << " (" << path.getPathCost() << ")"<< endl;
                        
                        queue.enqueue(path, path.getPathCost());
                        path.remove(path.size() - 1);
                    }
                    else{
                        cout << "Ignore " << arc->finish->name << " because its distance is known." << endl;
                    }
            }
        }
        if (queue.isEmpty()) {
            path.clear();
            return path;
        }
        
        path = queue.dequeue();
        
        cout << "Dequeue the shortest path: " << path.toStringPath();
        cout<< " (" << path.getPathCost() << ")"<< endl;

        start = path.getNode(path.size()-1);
    }
    return path;
}

void getNodes(Node * start){
    
    Node * endNode;
    string nodes = "";
    
    for (Arc *arc : start->arcs){
        nodes += arc->finish->name;
        nodes += ", ";
        endNode = arc->finish;
    }
    cout << nodes;
}
/*double getPathCost(Vector<Arc *> & path){
    double cost = 0;
    foreach(Arc * arc in path){
        cost += arc->cost;
    }
    return cost;
}

void toStringPath(Vector<Arc *> path){
    
    //string toStringPath = "";
    Node * endNode;
    for (Arc * arc: path){
        cout << arc->start->name + " -> ";
        endNode = arc->finish;
    }
    cout << endNode->name;
}*/

//Used to highlist the shortest path found using the Djikstra algorithm
void highlightPath(Path path, SimpleGraph * graph){
    
    dimAllArcs(graph);
    Node * start;
    Node * end;
    
    foreach(Arc * pathArc in path.getArcs()){
        
        start = pathArc->start;
        end = pathArc->finish;
        drawPathfinderNode(start->loc, HIGHLIGHT_COLOR);
        drawPathfinderNode(end->loc, HIGHLIGHT_COLOR);
        drawPathfinderArc(start->loc, end->loc, HIGHLIGHT_COLOR);
        
    }
}

//Dimming the remaing arcs and nodes that are not part of the shortest path.
void dimAllArcs(SimpleGraph * graph){
    
    foreach(Arc * arc in graph->arcs){
        drawPathfinderArc(arc->start->loc, arc->finish->loc, DIM_COLOR);
    }
    
    foreach(Node * node in graph->nodes){
        drawPathfinderNode(node->loc, DIM_COLOR);
        
    }
}

//Implementing the Kruskal's algorithm to find the minimum spanning tree.
void minSpanningTree(SimpleGraph* & graph){
    
}