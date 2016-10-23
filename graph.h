//
//  graph.h
//  Project5
//
//  Created by Tuan Dao on 11/6/15.
//  Copyright © 2015 Tuan Dao. All rights reserved.
//

#ifndef graph_h
#define graph_h
#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace boost;
using namespace std;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
    pair<int,int> cell; // maze cell (x,y) value
    Graph::vertex_descriptor prev; // predecessor node
    int weight;
    bool visited;
    bool marked;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
    int weight;
    bool visited;
    bool marked;
};

void clearVisited(Graph &g) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
        g[*vItr].visited = 0;
    }
}
// Mark all nodes in g as not visited.

void setNodeWeights(Graph &g, int w) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
        g[*vItr].weight = w;
    }
}
// Set all node weights to w.

void clearMarked(Graph &g) {
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
        g[*vItr].marked = 0;
    }
}

ostream &operator<<(ostream &os, const struct VertexProperties &v)
{
    os << "(Y,X)=(" << v.cell.first << "," << v.cell.second << "), weight=" << v.weight << ", visited=" << v.visited << ", marked=" << v.marked << "\n";
    return os;
}

ostream &operator<<(ostream &os, const struct EdgeProperties &e)
{
    os << "weight=" << e.weight << ", visited=" << e.visited << ", marked=" << e.marked << "\n";
    return os;
}

ostream &operator<<(ostream &os, const Graph &g) {
    string lnbrk = "-----------------------------------------------------------\n";
    
    pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vAdjRange;
    pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
    for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr) {
        os << lnbrk << "Vertex #" << *vItr << " @ "<< g[*vItr] << "Adjacent with:\n";
        vAdjRange = adjacent_vertices(*vItr, g);
        for (Graph::adjacency_iterator vAdj = vAdjRange.first; vAdj != vAdjRange.second; ++vAdj) {
            os << "Vertex #" << *vAdj << " @ " << g[*vAdj];
        }
    }
    
    pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
    for (Graph::edge_iterator eItr= eItrRange.first; eItr != eItrRange.second; ++eItr) {
        os << "Edge #" << *eItr << ": " << g[*eItr];
    }
	
    return os;
}

template <typename T> void clearStack(stack<T> &s) {
	while (!s.empty()) {
		s.pop();
	}
}

bool findPathDFSRecursiveUtil(Graph &g, stack <Graph::vertex_descriptor> &path, Graph::vertex_descriptor v, Graph::vertex_descriptor goal) {
	if (v == goal) return 1;
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(v, g);
	for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
		if (!g[*vItr].visited) {
			g[v].visited = 1;
			path.push(*vItr);
			if (findPathDFSRecursiveUtil(g, path, *vItr, goal))	return 1;
			else path.pop();
		}
	}
	return 0;
}

bool findPathDFSRecursive(Graph &g, stack <Graph::vertex_descriptor> &path, Graph::vertex_descriptor goal) {
	clearVisited(g);
	clearStack(path);
	path.push(0);
	return (findPathDFSRecursiveUtil(g, path, 0, goal));
}

bool findPathDFSStack(Graph &g, stack <Graph::vertex_descriptor> &path, Graph::vertex_descriptor goal) {
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange;
	stack <Graph::vertex_descriptor> s;
	Graph::vertex_descriptor v = 0;
	clearVisited(g);
	clearStack(path);
	g[v].prev = -1;
	s.push(v);
	while (!s.empty()) {
		v = s.top();
		s.pop();
		if (v == goal) {
			do {
				path.push(v);
				v = g[v].prev;
			} while (v != -1);
			return 1;
		}
		if (!g[v].visited) {
			g[v].visited = 1;
			vItrRange = adjacent_vertices(v, g);
			for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
				if (!g[*vItr].visited) {
					g[*vItr].prev = v;
					s.push(*vItr);
				}
			}
		}
	}
	return 0;
}

void findShortestPathDFSUtil(Graph &g, stack <Graph::vertex_descriptor> &path, stack <Graph::vertex_descriptor> &tempPath, Graph::vertex_descriptor v, Graph::vertex_descriptor goal, bool &found, unsigned int &length) {
	if (v == goal) {
		found = 1;
		if (tempPath.size() < length) {
			length = tempPath.size();
			path = tempPath;
		}
	}
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(v, g);
	for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
		if (!g[*vItr].visited) {
			g[v].visited = 1;
			tempPath.push(*vItr);
			findShortestPathDFSUtil(g, path, tempPath, *vItr, goal, found, length);
			g[v].visited = 0;
			tempPath.pop();
		}
	}
}
bool findShortestPathDFS(Graph &g, stack <Graph::vertex_descriptor> &path, Graph::vertex_descriptor goal) {
	bool found = 0;
	unsigned int length = LargeValue;
	stack <Graph::vertex_descriptor> tempPath;
	clearVisited(g);
	clearStack(path);
	tempPath.push(0);
	findShortestPathDFSUtil(g, path, tempPath, 0, goal, found, length);
	return found;
}

bool findShortestPathBFS(Graph &g, stack <Graph::vertex_descriptor> &path, Graph::vertex_descriptor goal) {
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange;
	queue <Graph::vertex_descriptor> q;
	Graph::vertex_descriptor v = 0;
	clearVisited(g);
	clearStack(path);
	g[v].prev = -1;
	q.push(v);
	while (!q.empty()) {
		v = q.front();
		q.pop();
		if (v == goal) {
			do {
				path.push(v);
				v = g[v].prev;
			} while (v != -1);
			return 1;
		}
		if (!g[v].visited) {
			g[v].visited = 1;
			vItrRange = adjacent_vertices(v, g);
			for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
				if (!g[*vItr].visited) {
					g[*vItr].prev = v;
					q.push(*vItr);
				}
			}
		}
	}
	return 0;
}

#endif /* graph_h */

/*

 // Create an empty graph
 Graph g;
 
 // Create a graph with 5 nodes and no edges
 Graph g(5)
 
 // Returns the number of edges
 int num_edges(g)
 
 // Returns the number of vertices
 int num_vertices(g)
 
 // Create a new node in the graph and return its vertex descriptor
 Graph::vertex_descriptor add_vertex(Graph &g);
 
 // Access a property of a node
 g[v].visited = true;
 
 // Get a pair containing iterators pointing the beginning and end of the
 // list of nodes
 pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(Graph &g);
 
 // Loop over all nodes in the graph
 for (Graph::vertex_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
 
 // Access a node using a vertex iterator
 g[*vItr]
 
 // Get a pair containing iterators pointing to the beginning and end of the
 // list of nodes adjacent to node v
 pair<Graph::adjacency_iterator, Graph::adjacency_iterator>
 vItrRange = adjacent_vertices(Graph::vertex_descriptor v, Graph &g);
 
 // Loop over adjacent nodes in the graph
 for (Graph::adjacency_iterator vItr= vItrRange.first; vItr != vItrRange.second; ++vItr)
 
 // Get a pair containing iterators pointing the beginning and end of the
 // list of edges
 pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(Graph &g);
 
 // Loop over all edges in the graph
 for (Graph::edge_iterator eItr= eItrRange.first; eItr != eItrRange.second; ++eItr)
 
 // Add an edge (u,v) to the graph.  newEdge.first is the edge descriptor for the
 // new edge.  newEdge.second is true if the edge didn't already exist, and false
 // otherwise
 pair<Graph::edge_descriptor, bool> newEdge =
 add_edge(Graph::vertex_descriptor u, Graph:: vertex_descriptor v, Graph &g);
 
 Graph::edge_descriptor e;
 
 // Access a property of an edge
 g[e].visited = true;
 g[*eItr].visited = true;
 
 // If edge (u,v) exists, checkEdge.first is that edge and checkEdge.second is true.
 // Otherwise checkEdge.second is false.
 pair<Graph::edge_descriptor, bool> checkEdge =
 edge(Graph::vertex_descriptor u, Graph::vertex_descriptor v, Graph &g);
 
 // Returns the target vertex of edge e.
 vertex_descriptor target(Graph::edge_descriptor e, Graph &g)
 
 // Returns the source vertex of edge e.
 vertex_descriptor source(Graph::edge_descriptor e, Graph &g)
 
 // Remove the edge (u,v) from the graph.
 void remove_edge(Graph::vertex_descriptor u, Graph::vertex_descriptor v, Graph &g)

 //bool found = 0;
 //pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange;
 //stack <Graph::vertex_descriptor> s;
 //Graph::vertex_descriptor v = 0;
 //size_t shortest = LargeValue;
 //stack <Graph::vertex_descriptor> tempPath;
 //clearVisited(g);
 //clearStack(path);
 //clearStack(tempPath);
 //g[v].prev = -1;
 //s.push(v);
 //while (!s.empty()) {
 //	v = s.top();
 //	s.pop();
 //	if (v == goal) {
 //		do {
 //			tempPath.push(v);
 //			v = g[v].prev;
 //		} while (v != -1);
 //	}
 //	if (!g[v].visited) {
 //		g[v].visited = 1;
 //		vItrRange = adjacent_vertices(v, g);
 //		for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
 //			if (!g[*vItr].visited) {
 //				g[*vItr].prev = v;
 //				s.push(*vItr);
 //			}
 //		}
 //	}
 //}
 //return found;
 //s.push(0);
 //while (!s.empty()) {
 //	v = s.top();
 //	s.pop();
 //	tempPath.push(v);
 //	if (v == goal) {
 //		found = 1;
 //		if (tempPath.size() < shortest) {
 //			shortest = tempPath.size();
 //			path = tempPath;
 //		}
 //	};
 //	if (g[v].weight != weight) {
 //		tempPath.pop();
 //		Graph::vertex_descriptor temp = tempPath.top();
 //		weight = g[v].weight;
 //		while (g[temp].weight >= weight) {
 //			tempPath.pop();
 //			temp = tempPath.top();
 //		}
 //		tempPath.push(v);
 //	}
 //	if (!g[v].visited) {
 //		weight = g[v].weight;
 //		g[v].visited = 1;
 //		vItrRange = adjacent_vertices(v, g);
 //		weight++;
 //		for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) {
 //			if (!g[*vItr].visited) {
 //				g[*vItr].weight = weight;
 //				s.push(*vItr);
 //			}
 //		}
 //	}
 //}
 //return found;
*/