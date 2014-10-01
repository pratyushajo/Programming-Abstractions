/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the random writer problem
 * on Assignment #2.
 * [TODO: extend the documentation]
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "filelib.h"
#include "map.h"
#include "vector.h"
#include "foreach.h"
using namespace std;

void randomWriterGenerator(ifstream & infile, int & markovState);
void analyzeText(Map<string, Vector<char> > & map, int & markovState, ifstream & infile);
void printMap(Map<string, Vector<char> > & map);
void printVector(Vector<char> vector);
string getInitialSeed(Map<string, Vector<char> > map);
int getVectorSize(Vector<char> vector);
string getRandomKey(string key, string initialSeed);
char getNextRandomCharacter(string initialSeed, Map<string, Vector<char> > map);

int main() {
	ifstream infile;
    promptUserForFile(infile, "Input file: ");
    cout << "Enter the Markov state(1-10): ";
    int markovState = getInteger();
    randomWriterGenerator(infile, markovState);
    infile.close();
	return 0;
}

//Used to generate the random text based on the Markov state provided.
//Takes the filename and the Markov State as parameters and prints out the random text.
void randomWriterGenerator(ifstream & infile, int & markovState){
    Map<string, Vector<char> > markovMap;
    
    analyzeText(markovMap, markovState, infile);
    
    string randomKey = getInitialSeed(markovMap);
    cout << randomKey;
    
    //cout << "Random text: " << endl;
    
    int numOfCharsGenerated = 0;
    
    while(true){
        
        if (numOfCharsGenerated > 2000) break;
        
        char nextCharacter = getNextRandomCharacter(randomKey, markovMap);
        
        if (nextCharacter == '\0'){
            cout << endl << "Couldnt find any Markov strings. " << endl;
            break;
        }
        
        cout << nextCharacter;
        
        randomKey = randomKey.substr(1);
        randomKey += nextCharacter;
        
        numOfCharsGenerated++;
    }
    cout << endl;
    
    //printMap(markovMap);
}


//Reading the file character by character and creating key and value map.
//Creates a key depending on the Markov state entered by user.
//If key already exists, it adds the value to the vector, else creates a new key with the value.
void analyzeText(Map<string, Vector<char> > & map, int & markovState, ifstream & infile){
    int loopCount = 0;
    
    while(infile.get() != EOF){
        infile.seekg(loopCount);
        string key;
        for(int i=0; i < markovState; i++){
            char ch = infile.get();
            key += ch;
        }
    
        char nextCharacter = infile.get();
     
        loopCount++;
        //Adding to the map.
        //If the word already exists in the map, will just add the next character to the vector.
        //else, creates a new key for the word and adds  the character to the value.
        map[key].add(nextCharacter);
        
    }
    
}

//This function returns the Markov string that has occured most number of times in the text.
//It takes in the map as the parameter
string getInitialSeed(Map<string, Vector<char> > map){
    int maxSize = 0;
    string initialSeed;
    
    //iterating throught the map to find the key with most number of values associated with it.
    foreach(string key in map){
        int size = getVectorSize(map[key]);
        
        if (size > maxSize) {
            maxSize = size;
            initialSeed = key;
        }
        
        //Selecting a random key when both the keys have that same number of values associated with it.
        else if(size == maxSize){
            maxSize = size;
            initialSeed = getRandomKey(key, initialSeed);
        }
    }
    return initialSeed;
}

//Function is used to return a random key.
//Takes in two parameters, selects one in random and returns the random key.
string getRandomKey(string key, string initialSeed){
    int randomNumber = randomInteger(0, 1);
    if (randomNumber == 0) {
        return key;
    }
    else
        return initialSeed;
}


//Function is used to generate the next random character in the text.
//If the values associated with the key is greater than 1, it selects a random value and returns it.
//Else returns the first value.
//Takes in the initial seed and the map as the parameters and returns a value.
char getNextRandomCharacter(string initialSeed, Map<string, Vector<char> > map){
    
    Vector<char> vector;
    if(map.containsKey(initialSeed)){
        vector = map.get(initialSeed);
    }
    else
        return '\0';
    
    if (vector.size() > 1) {
        int maxSize = vector.size();
        
        int randomNumber = randomInteger(0, maxSize - 1);
        
        return vector[randomNumber];
    }
    
    else
        return vector[0];
    
}

//Function gives the size of the vector associated with a key.
int getVectorSize(Vector<char> vector){
    return vector.size();
}

void printMap(Map<string, Vector<char> > & map){
    foreach (string word in map){
        cout << "Key: " << word;
        cout << "   Value: ";
        printVector(map[word]);
        cout << endl;
    }
}

void printVector(Vector<char> vector){
    for (int i=0; i < vector.size(); i++) {
        cout << vector[i];
    }
}
