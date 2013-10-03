// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
using namespace std;

bool checkFileName(string fileName);
string getFileName();
void initializeGrid(string fileName, Grid<string>& environment);
void evolve(Grid<string>& habitat, Grid<string>& newGeneration);
void printGrid(Grid<string> &grid);

int main() {
    setConsoleSize(700, 400);

    string fileName = getFileName();

    Grid<string> habitat;
    initializeGrid(fileName, habitat);

    Grid<string> newGeneration(habitat.numRows(), habitat.numCols());

    string command;

    while(1) {
        cout << "a)nimate, t)ick, q)uit? ";
        cin >> command;
        if(command == "a") {
            int frame = 1;
            cout << "How many frames? ";
            cin >> frame;
            while(frame-- > 0) {
                evolve(habitat, newGeneration);
                pause(100);
                printGrid(habitat);
            }
        } else if (command == "t") {
            evolve(habitat, newGeneration);
            printGrid(habitat);
        } else if (command == "q") {
            break;
        }
    }

    cout << "Have a nice Life!" << endl;

    return 0;
}

string getFileName() {
    string fileName;

    cout << "Welcome to the CS 106B Game of Life," << endl
         << "a simulation of the lifecycle of a bacteria colony." << endl
         << "Cells (X) live and die by the following rules:" << endl
         << "- A cell with 1 or fewer neighbors dies." << endl
         << "- Locations with 2 neighbors remain stable." << endl
         << "- Locations with 3 neighbors will create life." << endl
         << "- A cell with 4 or more neighbors dies." << endl << endl;

    while(!checkFileName(fileName)) {
        cout << "Grid input file name? ";
        cin >> fileName;
    }

    return fileName;
}

bool checkFileName(string fileName) {
    bool result = true;

    if(fileName == "") {
        result = false;
        // cout << "You must enter a valid file name." << endl;
    }

    return result;
}

void initializeGrid(string fileName, Grid<string>& habitat) {
    ifstream stream;
    // open file
    if( openFile(stream, fileName) ) {
        string line;
        // get rows
        getline(stream, line);
        int rows = stringToInteger(line);
        // get columns
        getline(stream, line);
        int columns = stringToInteger(line);
        // resize grid
        habitat.resize(rows, columns);
        // initialize the grid from file
        int currentRow = 0;
        while(currentRow < rows && getline(stream, line) != NULL) {
            for ( int i = 0; i < columns; i++ ) {
                habitat.set(currentRow, i, line.substr(i, 1));
            }
            currentRow++;
        }
    } else {
        cout << "open file failed!" << endl;
    }

    printGrid(habitat);
}

void evolve(Grid<string>& habitat, Grid<string>& newGeneration) {
    Grid<string> tmp;
    for(int row=0; row < habitat.numRows(); row++) {
        for(int column=0; column < habitat.numCols(); column++) {
            int lifeCount = 0;
            for(int tmpRow = row -1; tmpRow <= row+1; tmpRow++) {
                for(int tmpCol = column -1; tmpCol <= column+1; tmpCol++) {
                    if( ( tmpRow != row || tmpCol != column ) && habitat.inBounds(tmpRow, tmpCol) ) {
                        if( habitat.get(tmpRow, tmpCol) == "X") {
                            lifeCount++;
                        }
                    }
                }
            }
            switch (lifeCount) {
                case 2:
                    newGeneration.set(row, column, habitat.get(row, column));
                    break;
                case 3:
                    newGeneration.set(row, column, "X");
                    break;
                default:
                    newGeneration.set(row, column, "-");
                    break;
            }
        }
    }
    tmp = habitat;
    habitat = newGeneration;
    newGeneration = tmp;
}

void printGrid(Grid<string> &grid) {
    for(int i = 0; i < grid.numRows(); i++) {
        for(int j = 0; j < grid.numCols(); j++) {
            cout << grid.get(i, j);
        }
        cout << endl;
    }
}
