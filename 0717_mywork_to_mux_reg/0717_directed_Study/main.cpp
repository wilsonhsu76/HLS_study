#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "graph.h"

using namespace std;

int main()
{
	//test:
	// Create a graph given in the above diagram
	//Graph g(5);
	//g.addEdge(0, 1);
	//g.addEdge(0, 2);
	//g.addEdge(1, 2);
	//g.addEdge(2, 0);
	//g.addEdge(2, 3);
	//g.addEdge(3, 3);
	/*g.addEdge(0, 1); //mux test
	g.addEdge(1, 2);
	g.addEdge(2, 3);
	g.addEdge(3, 4);
	g.setVinfo(0, 1, "mult", 0.0f, 1, 1);
	g.setVinfo(1, 0, "add", 1.0f, 1, 1);
	g.setVinfo(2, 1, "mult", 2.0f, 1, 1);
	g.setVinfo(3, 0, "add", 3.0f, 1, 1);
	g.setVinfo(4, 0, "mult", 4.0f, 1, 1);*/

	/*Graph g(7);
	g.addEdge(0, 4);
	g.addEdge(1, 4);
	g.addEdge(2, 5);
	g.addEdge(3, 5);
	g.addEdge(4, 6);
	g.addEdge(5, 6);

	g.setVinfo(0, 0, "add", 0.0f, 1, 1);
	g.setVinfo(1, 1, "add", 0.0f, 1, 1);
	g.setVinfo(2, 2, "add", 0.0f, 1, 1);
	g.setVinfo(3, 3, "add", 0.0f, 1, 1);
	g.setVinfo(4, 0, "mult", 2.0f, 1, 1);
	g.setVinfo(5, 1, "mult", 2.0f, 1, 1);
	g.setVinfo(6, 4, "add", 6.0f, 1, 1);*/

	//mult_time:mt, add_time:at, clock_time: ct, target_clock_cycle target_cycle:
	//bool pipe_flag = false;
	//bool pipe_flag = g.checkDFFforGraph(4.0, 2.0, 2.0, 2);
	//bool pipe_flag = g.checkDFFforGraph(4.0, 2.0, 2.0, 3);
	//bool pipe_flag = g.checkDFFforGraph(4.0, 2.0, 2.0, 1);

	/*if (pipe_flag) {
		g.printDFFInfoBeforeAdd();
		g.printDFFInfoBeforeMult();
	}
	else {
		cout << "operation time takes too long to pipeline";
	}*/

	//cout << "Following is Depth First Traversal: ";
	//g.DFS(0);
	//cout << endl;
	//g.checkMuxforAdd();
	//g.checkMuxforMult();
	//g.printMuxInfoBeforeAdd();
	//g.printMuxInfoBeforeMult();
	//endtest

/*	Graph g(3);
	g.setVinfo(0, 0, "add", 0.0f, 8, 7);
	g.setVinfo(1, 1, "add", 10.0f, 5, 4);
	g.setVinfo(2, 2, "add", 15.0f, 3, 3);
	g.addEdge(0, 1);
	g.addEdge(1, 2);
	
	g.setGraph_FinishTime( 2, 4);
	g.fillInterConncetPart();
	g.findDFFsinInterConncet(1, 3);
	g.printInterConncetPart();*/

	Graph g(8);
	g.setVinfo(0, 0, "add", 0.0f, 1, 1);
	g.setVinfo(1, 1, "add", 0.0f, 1, 1);
	g.setVinfo(2, 2, "add", 0.0f, 1, 1);
	g.setVinfo(3, 3, "add", 2.0f, 1, 1);
	g.setVinfo(4, 4, "add", 2.0f, 1, 1);
	g.setVinfo(5, 5, "add", 4.0f, 1, 1);
	g.setVinfo(6, 6, "add", 4.0f, 1, 1);
	g.setVinfo(7, 7, "add", 6.0f, 1, 1);

	g.addEdge(0, 4);
	g.addEdge(1, 4);
	g.addEdge(2, 6);
	g.addEdge(3, 5);
	g.addEdge(4, 6);
	g.addEdge(5, 7);
	g.addEdge(6, 7);

	g.setGraph_FinishTime(2, 4);
	g.fillInterConncetPart_DD_Left();
	g.printInterConncetPart_DD_Left();
	g.binding_DD_Left();
	g.printDDBindingResult();

	return 0;
}
