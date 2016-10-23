//
//  maze.h
//  Project5
//
//  Created by Tuan Dao on 11/6/15.
//  Copyright © 2015 Tuan Dao. All rights reserved.
//

#ifndef maze_h
#define maze_h

// Sample solution for project #5

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>
#include <boost/graph/adjacency_list.hpp>
#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"

using namespace std;

class maze
{
public:
    maze(ifstream &fin);
    void print(int,int,int,int);
	void printSimplePath(Graph& g, stack<Graph::vertex_descriptor> &s, Graph::vertex_descriptor end);
    bool isLegal(int i, int j);
    void mapMazeToGraph(Graph &g);
	void printPath(Graph& g, stack<Graph::vertex_descriptor> &s, Graph::vertex_descriptor end);
	int numRows(){return rows;};
    int numCols(){return cols;};
    friend ostream &operator<<(ostream &os, const Graph &g);
	Graph::vertex_descriptor getGoal(Graph &g);
private:
    int rows; // number of rows in the maze
    int cols; // number of columns in the maze
    
    matrix<bool> value;
    matrix<Graph::vertex_descriptor> index;
};

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
    fin >> rows;
    fin >> cols;
    
    char x;
    
    value.resize(rows,cols);
    index.resize(rows,cols);
    for (int i = 0; i <= rows-1; i++)
        for (int j = 0; j <= cols-1; j++)
        {
            index[i][j] = INT_MAX;
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;
        }
    
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{   
    if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
        throw rangeError("Bad value in maze::print");
    
    if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
        throw rangeError("Bad value in maze::print");
    
    for (int i = 0; i <= rows-1; i++)
    {
        for (int j = 0; j <= cols-1; j++)
        {
            if (i == goalI && j == goalJ)
                cout << "*";
            else
                if (i == currI && j == currJ)
                    cout << "+";
                else
                    if (value[i][j])
                        cout << " ";
                    else
                        cout << "X";
        }
        cout << endl;
    }
    cout << endl;
}

void maze::printSimplePath(Graph& g, stack<Graph::vertex_descriptor> &s, Graph::vertex_descriptor end) {
	matrix<char> map;
	map.resize(rows, cols);
	for (int i = 0; i <= rows - 1; i++) {
		for (int j = 0; j <= cols - 1; j++) {
			if (isLegal(i, j))
				map[i][j] = ' ';
			else 
				map[i][j] = 'H';
		}
	}

	while (!s.empty()) {
		map[g[s.top()].cell.first][g[s.top()].cell.second] = '.';
		s.pop();
	}

	for (int i = 0; i <= rows - 1; i++) {
		for (int j = 0; j <= cols - 1; j++) {
			cout << map[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to go to cell (i,j).
{
    if (i < 0 || i > rows || j < 0 || j > cols)
        throw rangeError("Bad value in maze::isLegal");
    
    return value[i][j];
}


void maze::mapMazeToGraph(Graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
    Graph::vertex_descriptor v;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (value[i][j]) {
                v = add_vertex(g);
                g[v].cell.first = i;
                g[v].cell.second = j;
                index[i][j] = v;
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (value[i][j]) {
                v = index[i][j];
                // Check up
                if (i>0 && value[i-1][j]) {
                    add_edge(v, index[i-1][j], g);
                }
                // Check down
                if (i<rows-1 && value[i+1][j]) {
                    add_edge(v, index[i+1][j], g);
                }
                // Check left
                if (j>0 && value[i][j-1]) {
                    add_edge(v, index[i][j-1], g);
                }
                // Check right
                if (j<cols-1 && value[i][j+1]) {
                    add_edge(v, index[i][j+1], g);
                }
            }
        }
    }
}

void maze::printPath(Graph& g, stack<Graph::vertex_descriptor> &s, Graph::vertex_descriptor end) {
    vector<Graph::vertex_descriptor> temp;
    while (!s.empty()) {
        temp.push_back(s.top());
        s.pop();
    }
    int goalY = g[end].cell.first;
    int goalX = g[end].cell.second;
    while (!temp.empty()) {
        print(goalY, goalX, g[temp.back()].cell.first, g[temp.back()].cell.second);
		cout << "Current location: (" << g[temp.back()].cell.first << "," << g[temp.back()].cell.second << ")\n";
        s.push(temp.back());
        temp.pop_back();
    }

	cout << "Single path:\n";
	printSimplePath(g, s, end);
}

Graph::vertex_descriptor maze::getGoal(Graph &g) {
	return index[rows - 1][cols - 1];
}

#endif /* maze_h */
