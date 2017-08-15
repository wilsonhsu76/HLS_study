#ifndef __GRAPH_H__
#define __GRAPH_H__
#define MAX_EDGE_WEIGHT 5000000
#include <stdio.h>

#include<iostream>
#include<list>
using namespace std;

// Graph class represents a directed graph using adjacency list representation
typedef struct _Vertex {
	string op_str;     //operating system
	int opNum;       //record op Num
	int index;         //vertex_index
	float start_time;   //operation start time
	float end_time;
	int in_bitwidth;
	int out_bitwidth;
} Vertex;

typedef struct _InterConncet {
	float start_time;   //operation start time
	float end_time;     //next start time
	int bitwidth;
	int index;
	int DFFcounts;
	int from_Vertex;
	int to_Vertex;
	int conncet_feature; // reserve
	bool binding;  //for DD_bind
} InterConncet;

class Graph
{
	int V;    // No. of vertices
	list<int> *adj;    // Pointer to an array containing adjacency lists
	Vertex *vList;  //record each vertex content
	void DFSUtil(int v, bool visited[]);  // A function used by DFS
	bool *mult_mux_array;  //if=1 ,before implementation_index existing mux
	bool *add_mux_array;   //if=1 ,before implementation_index existing mux
	bool *mult_DFF_array;  //if=1 ,before implementation_index existing DFF
	bool *add_DFF_array;  //if=1 ,before implementation_index existing DFF
	InterConncet *interInfo;
	list<int> *reg_binding;  //one reg for each list
	

public:
	Graph(int V);   // Constructor
	void addEdge(int v, int w);   // function to add an edge to graph
	void DFS(int v);    // DFS traversal of the vertices reachable from v
	void setVinfo(int v, int u, string s, float t, int ib, int ob);  //set vertex info
	void setGraph_FinishTime(float at, float mt);
	void checkMuxforMult();  //check where should add mux
	void checkMuxforAdd();   //check where should add mux
	void printMuxInfoBeforeAdd();
	void printMuxInfoBeforeMult();
	bool checkDFFforGraph(float mt, float at, float ct, int target_cycle);
	void printDFFInfoBeforeAdd();
	void printDFFInfoBeforeMult();
	void fillInterConncetPart();
	void findDFFsinInterConncet(float cycle_time, int target_cycle);
	void printInterConncetPart();
	void fillInterConncetPart_DD_Left();
	void printInterConncetPart_DD_Left();
	void binding_DD_Left();
	void printDDBindingResult();
};

#endif