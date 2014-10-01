/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */
 
#include <iostream>
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
using namespace std;

/* Constants */

struct pointT{
    int row;
    int col;
};

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void boardSetup();
void pickRandomSide(Grid<char> & board);
void printBoard(Grid<char> & board);
void shuffleBoard(Grid<char> & board);
void clearBoard(Grid<char> & board);
void playGame(Grid<char> & board);
bool validWordHuman(string word, Lexicon &lex, Grid<char> & board);
bool boardHasWord(string word, Grid<char> & board);
bool findWord(Grid<char> & board, string word, int row, int col, string & soFar, Vector<pointT> &cells);
void highlightWord(Vector<pointT> & cells);
bool alreadyCubeUsed(Vector<pointT> & cells, int row, int col);
void computerTurn(Grid<char> &board, Lexicon &lex, Set<string> &wordList);
void computerFindWords(Grid<char> &board, int row, int col, string soFar, Lexicon &lex, Vector<pointT> &cellsForComputer, Set<string> &wordList);
//string validWordComp(string soFar, string word, Lexicon &lex);
void forceConfiguration(Grid<char> &board);
void fillBoard(Grid<char> &board, string letters);

/* Main program */

int main() {
    initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    welcome();
    
    cout << "Do you want instructions? ";
    string ch = getLine();
    if (ch == "y" || ch == "Y") {
        giveInstructions();
    }
    
    boardSetup();
    
    while (true) {
        cout << "Do you want to play again?" << endl;
        string playAgain = getLine();
        if(playAgain == "n" || playAgain == "N") break;
        
        else if (playAgain.empty()){
            cout << "Enter 'Y' or 'y' for yes and 'N' or 'n' for no." << endl;
        }
        else if(playAgain == "y" || playAgain == "Y"){
            initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
            boardSetup();
        }
    }
    
    return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
   cout << "Welcome!  You're about to play an intense game ";
   cout << "of mind-numbing Boggle.  The good news is that ";
   cout << "you might improve your vocabulary a bit.  The ";
   cout << "bad news is that you're probably going to lose ";
   cout << "miserably to this little dictionary-toting hunk ";
   cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
   cout << endl;
   cout << "The boggle board is a grid onto which I ";
   cout << "I will randomly distribute cubes. These ";
   cout << "6-sided cubes have letters rather than ";
   cout << "numbers on the faces, creating a grid of ";
   cout << "letters on which you try to form words. ";
   cout << "You go first, entering all the words you can ";
   cout << "find that are formed by tracing adjoining ";
   cout << "letters. Two letters adjoin if they are next ";
   cout << "to each other horizontally, vertically, or ";
   cout << "diagonally. A letter can only be used once ";
   cout << "in each word. Words must be at least four ";
   cout << "letters long and can be counted only once. ";
   cout << "You score points based on word length: a ";
   cout << "4-letter word is worth 1 point, 5-letters ";
   cout << "earn 2 points, and so on. After your puny ";
   cout << "brain is exhausted, I, the supercomputer, ";
   cout << "will find all the remaining words and double ";
   cout << "or triple your paltry score." << endl << endl;
   cout << "Hit return when you're ready...";
   getLine();
}

// [TODO: Fill in the rest of the code]
/*  Initializes the board setup.
 *  Initializes a 4*4 board currently.
 */
void boardSetup(){
    
    int dimension;
    
    cout << "Would you like to try the Big Boggle? ";
    
    string str = getLine();
    
    if (str == "y" || str == "Y")
        dimension = 5;
    
    else
        dimension = 4;
    
    cout << endl;
    
    Grid<char> board(dimension,dimension);
    drawBoard(dimension, dimension);
    
    clearBoard(board);
    
    cout << "Would you like to force the configuration? ";
    
    str = getLine();
    
    if (str == "y" || str == "Y"){
        forceConfiguration(board);
        printBoard(board);
        playGame(board);
    }
    
    else{
        pickRandomSide(board);
        shuffleBoard(board);
        printBoard(board);
        playGame(board);
    }
    
}

/*  Picks a random side of the cube.
 *
 */
void pickRandomSide(Grid<char> & board){
    
    int i = 0;

    for (int row = 0; row < board.numRows(); row++) {
        for (int col =0; col < board.numCols(); col++) {
            int randomInt = randomInteger(0, 5);
            string standardString = STANDARD_CUBES[i];
            char ch = standardString[randomInt];
            board[row][col] = ch;
            i++;
        }
    }
}

/*  Prints the label of each cube.
 *
 */
void printBoard(Grid<char> & board){
    for (int row = 0; row < board.numRows(); row++) {
        for (int col =0; col < board.numCols(); col++) {
            char ch = board[row][col];
            labelCube(row, col, ch);
        }
    }
}

/*  Used to shuffle the board so that no cube is at the same location.
 *
 */
void shuffleBoard(Grid<char> & board){
    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            int randomRow = randomInteger(row, board.numRows()-1);
            int randomCol = randomInteger(col, board.numCols()-1);
            char randomLabel = board[row][col];
            board[row][col] = board[randomRow][randomCol] ;
            board[randomRow][randomCol] = randomLabel;
        }
    }
}

/*  Clears the grid for every new round of the game.
 *
 */
void clearBoard(Grid<char> & board){
    for (int i=0; i<board.numRows(); i++) {
        for (int j=0; j<board.numCols(); j++) {
            board[i][j] = false;
        }
    }
}

/*  Starts the play of the game by asking the user to start entering words.
 *  Uses a "Set" to store the words already entered. Once an empty line is entered the user's turn ends and
 *  the computer lists all the words found.
 *  The function checks if the word is found in the lexicon, meets the minimum length requirement
 *  or words that have already been entered. It displays the respective message if any of the condition
 *  is not met.
 */
void playGame(Grid<char> & board){
    Player currentPlayer = HUMAN;
    Lexicon lex("EnglishWords.dat");   //Lexicon containing the words.
    Set<string> wordList;
    
    cout << "Ok, take all the time you want and find all the words you can!  Signal that you're finished by entering an empty line." << endl << endl;
    
    if (wordList.size() > 0) {
        wordList.clear();
    }
    
    //User turn
    while (true) {

        cout << "Enter a word: " ;
        string word = getLine();
        
        word = toUpperCase(word);
        
        if (word.empty()) {
            currentPlayer = COMPUTER;
            break;
        }
    
        if(word.length() <= 3){
            cout << "I'm sorry, but we have our standards." << endl;
            cout << "That word doesn't meet the minimum word length." << endl;
        }

        else if(validWordHuman(word, lex, board)){
            
            if (wordList.contains(word)) {
                cout << "You already entered this word." << endl;
            }
            else{
                wordList.add(word);
                recordWordForPlayer(word, currentPlayer);
            }
        }
        
        else
            cout << "Thats not a valid word!" << endl;
    }
    // The user turn ends here and the computer begins listing the words.
    computerTurn(board, lex, wordList);
}

/*  This function is used to check if the word is found in the lexicon.
 *  Returns true if the word is found in the lexicon, else false
 */
bool validWordHuman(string word, Lexicon &lex, Grid<char> &board){
    
    //Checks if word is present in the lexicon and also checks if the word can be formed on the board.
    if (lex.contains(word) && boardHasWord(word, board)) {
            return true;
    }
    
    else return false;
    
}

/*  Takes in the word entered by user and checks if it satifies the conditions of adjoinment cubes.
 *
 */
bool boardHasWord(string word, Grid<char> & board){
    
    string soFar = "";
    
        for (int row=0; row < board.numRows(); row++) {
            for (int col =0; col < board.numCols(); col++) {
                
                //The first letter of the word is found on the board
                if (board[row][col] == word[0]) {
                    
                    //Adding the first letter to sofar
                    soFar = word[0];
                    
                    Vector<pointT> cells;
                    //Adding the cube as already used.
                    pointT pt;
                    pt.row = row;
                    pt.col = col;
                    cells.add(pt);
                    
                    //If the word satisfies the condition of adjoinment, highlight the word.
                    if (findWord(board, word, row, col, soFar, cells)) {
                        highlightWord(cells);
                        return true;
                    }
                    //If not found, remove the first character from sofar and remove the cube as used.
                    else{
                        soFar = "";
                        cells.removeAt(cells.size()-1);
                    }
                }
            }
        }
    return false;
}
/*  The function takes in the word and the starting cube and checks for the remaining part of the word
 *  in the adjoining cubes.
 *  The function checks for the conditions:
 *      - the adjoining cube shouldnt already be used.
 *      - skips the current cube
 *      - checks if the adjoining cubes are within the bounds of the board.
 *
 */
bool findWord(Grid<char> & board, string word, int row, int col, string & soFar, Vector<pointT> &cells){
    
    //Checking if a part of the string is matching. If not, the function returns.
    if (word.substr(0,soFar.length()) != soFar) {
        return false;
    }
    
    
    if (soFar.length() > word.length()) {
        return false;
    }
    
    //If the word is found.
    if(soFar == word){
        return true;
    }
    else{
        for(int i = -1; i <= 1; i++){
            for (int j = -1; j <= 1; j++) {
                if (!(alreadyCubeUsed(cells, row+i, col+j)) && !(i == 0 && j == 0)
                    && board.inBounds(row+i, col+j)) {
                                //If the conditions are satified, adds the adjoining cube to sofar.
                                soFar += board[row+i][col+j];
                                //Adds the cube as already used.
                                pointT pt;
                                pt.row = row+i;
                                pt.col = col+j;
                                cells.add(pt);
                        //Recursive call to find the remaining part of the word.
                        if(findWord(board, word, row+i, col+j, soFar, cells)){
                            return true;
                        }
                        //if the adjoining cube returns false, remove it from soFar and used cubes list.
                        else{
                            if(soFar.length() > 0){
                                soFar = soFar.substr(0, soFar.length()-1);
                                cells.removeAt(cells.size()-1);
                            }
                        }
                }
            }
        }
        
    }
    return false;
}

/*  Once the word is found on the board, this function temporarily highlights the word.
 *
 */
void highlightWord(Vector<pointT> & cells){
    for (int i=0; i<cells.size(); i++) {
        highlightCube(cells[i].row, cells[i].col, true);
    }
    pause(500);
    for (int j=0; j<cells.size(); j++) {
        highlightCube(cells[j].row, cells[j].col, false);
    }
}

/*  This function takes in the position of a cube and checks if it has already been used in forming 
 *  the word.
 *
 */
bool alreadyCubeUsed(Vector<pointT> & cells, int row, int col){
    for (int i=0; i < cells.size(); i++) {
        if (cells[i].row == row && cells[i].col == col) {
            return true;
        }
    }
    return false;
}

/*  The computer takes in the lexicon and the list of words that have been found by the user.
 *  It skips listing the words already listed by the user.
 */
void computerTurn(Grid<char> &board, Lexicon &lex, Set<string> &wordList){
    
    string soFar = "";
    Vector<pointT> cellsForComputer;
    
    for (int row =0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            //Starts with the cube at location (0,0)
            soFar = board[row][col];
            //Adds the cube as a used cube
            pointT pt;
            pt.row = row;
            pt.col = col;
            cellsForComputer.add(pt);
        
            computerFindWords(board, row, col, soFar, lex, cellsForComputer, wordList);
            //If no words were formed with the starting cube, making the cube as unused.
            cellsForComputer.removeAt(cellsForComputer.size()-1);
        }
    }
}

/*  The function takes in the starting location to look for the word and checks for all possible words that
 *  can be formed using its adjoining cubes.
 *
 */
void computerFindWords(Grid<char> &board, int row, int col, string soFar, Lexicon &lex, Vector<pointT> &cellsForComputer, Set<string> &wordList){
    
    // Check if soFar is a valid boggle word and add it to the wordlist if it is not already been added.
    if (soFar.length() > 3 && lex.contains(soFar)) {
        if (!(wordList.contains(soFar))) {
            wordList.add(soFar);
            recordWordForPlayer(soFar, COMPUTER);
        }
    }
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(alreadyCubeUsed(cellsForComputer, row+i, col+j)) && !(i == 0 && j == 0)
                    && board.inBounds(row+i, col+j)) {
                //If the prefix exists in the lexicon, continues further.
                if (lex.containsPrefix(soFar + board[row+i][col+j])){
                    soFar += board[row+i][col+j];
                    //Adding the cube as already used.
                    pointT pt;
                    pt.row = row+i;
                    pt.col = col+j;
                    cellsForComputer.add(pt);
                    
                    //Recursive call, called when soFar is found as a prefix in the lexicon.
                    computerFindWords(board, row+i, col+j, soFar, lex, cellsForComputer, wordList);
                    
                    // Found all the words on the baord that start with soFar + board[row+i][col+j]
                    // Now remove board[row+i][col+j] from the updated so far and continue searching
                    soFar = soFar.substr(0, soFar.length()-1);
                    cellsForComputer.removeAt(cellsForComputer.size()-1);
                        
                }
            }
        }
    }
    
    return;
}

/*  This function allows the user to customize the cubes
 *  Asks the user to enter a string that has enough characters to fill the board.
 *  If a string longer than expected is entered, the extra characters at the end are just discarded.
 */
void forceConfiguration(Grid<char> &board){
    
    string enteredString = "";
    int boardSize = board.numRows() * board.numCols();
    
    cout << "Enter a " << boardSize << "-character string to identify which letters you want on the cubes." << endl;
    
    cout << "The first " << board.numRows() << " letters are the cubes on the top row from left to right, the next " << board.numRows() << " letters are the second row, and so on." << endl;
    cout << "Enter an empty line after entering the string." << endl << "Enter the string: " ;
 
    enteredString = getLine();
    
    //Asking the user to enter a string as long as the number of cubes.
    while (enteredString.length() < boardSize) {
        cout << "Incorrect number of characters entered. Please enter " << boardSize << " characters long string" << endl;
        enteredString = getLine();
    }
    
    enteredString = toUpperCase(enteredString);
    
    fillBoard(board, enteredString);
}

/*  Fills the board with the characters entered by the user
 *
 */
void fillBoard(Grid<char> &board, string letters){
    
    int i = 0;
    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            board[row][col] = letters[i];
            i++;
        }
    }
}
