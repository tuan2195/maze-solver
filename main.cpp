//
//  main.cpp
//  Project5
//
//  Created by Tuan Dao on 11/2/15.
//  Copyright © 2015 Tuan Dao. All rights reserved.
//

#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include "maze.h"
#include "graph.h"

using namespace std;

int main(int argc, char** argv)
{
    ifstream fin;

    if (argc != 2)
    {
        cout << "Usage: maze filename\n";
        return -1;
    }

    fin.open(argv[1]);
    if (!fin) {
        cerr << "Cannot open " << argv[1] << endl;
        exit(1);
    }
 
    maze m(fin);
	fin.close();
	cout << "Maze loaded!\n";
    //m.print(m.numRows()-1,m.numCols()-1,0,0);

    Graph g;
    m.mapMazeToGraph(g);
	cout << "Graph created!\n";

	Graph::vertex_descriptor goal = m.getGoal(g);
	stack <Graph::vertex_descriptor> path;

	cout << "Finding path using DFS recursive:\n";
	if (findPathDFSRecursive(g, path, goal))
		m.printSimplePath(g, path, goal);
	else 
        cout << "Path not found!";

	cout << "Finding path using DFS stack:\n";
	if (findPathDFSStack(g, path, goal))
		m.printSimplePath(g, path, goal);
	else 
        cout << "Path not found!";

	cout << "Finding shortest path using DFS stack:\n";
	if (findShortestPathDFS(g, path, goal))
		m.printSimplePath(g, path, goal);
	else 
        cout << "Path not found!";

	cout << "Finding shortest path using BFS queue:\n";
	if (findShortestPathBFS(g, path, goal))
		m.printSimplePath(g, path, goal);
	else 
        cout << "Path not found!";

    return 0;
}
