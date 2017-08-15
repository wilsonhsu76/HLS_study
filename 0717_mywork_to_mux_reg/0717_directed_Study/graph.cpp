#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

Graph::Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
	vList = new Vertex[V];
	mult_mux_array = new bool[V];
	add_mux_array = new bool[V];
	mult_DFF_array = new bool[V];
	add_DFF_array = new bool[V];
	interInfo = new InterConncet[V]; //temp test for V edges.
	reg_binding = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); // Add w to v¡¦s list.
}

//index:v, implement Num:u, implementation type:s, operation start time:t, bitwidth:b
void Graph::setVinfo(int v, int u, string s, float t, int ib, int ob)
{
	vList[v].opNum = u;
	vList[v].op_str = s;
	vList[v].index = v;
	vList[v].start_time = t;
	vList[v].in_bitwidth = ib;
	vList[v].out_bitwidth = ob;
}

void Graph::DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and print it
	visited[v] = true;
	cout << v << " ";

	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited);
}

// DFS traversal of the vertices reachable from v. 
// It uses recursive DFSUtil()
void Graph::DFS(int v)
{
	// Mark all the vertices as not visited
	bool *visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Call the recursive helper function to print DFS traversal
	DFSUtil(v, visited);
}

void Graph::checkMuxforMult()
{
	int x = this->V;
	bool *test_array = new bool[x];
	for (int i = 0; i < this->V; i++) {
		test_array[i] = false;  //init false
	}
	for (int i = 0; i < this->V; i++) {
		if (this->vList[i].op_str == "mult") {
			if (test_array[this->vList[i].opNum] == true) {
				this->mult_mux_array[this->vList[i].opNum] = true;  //means there is two operators use the same implementation
			}
			test_array[this->vList[i].opNum] = true;
		}
	}
	delete test_array;
}

void Graph::checkMuxforAdd()
{
	int x = this->V;
	bool *test_array = new bool[x];
	for (int i = 0; i < this->V; i++) {
		test_array[i] = false;  //init false
	}
	for (int i = 0; i < this->V; i++) {
		if (this->vList[i].op_str == "add") {
			if (test_array[this->vList[i].opNum] == true) {
				this->add_mux_array[this->vList[i].opNum] = true;  //means there is two operators use the same implementation
			}
			test_array[this->vList[i].opNum] = true;
		}
	}
	delete test_array;
}

void Graph::printMuxInfoBeforeAdd()
{
	cout << "[Add]Mux Info: ";
	for (int i = 0; i < this->V; i++) {
		if (this->add_mux_array[i] == true)
			cout << "true" << "  ";
		else
			cout << "false" << "  ";
	}
	cout << endl;
}

void Graph::printMuxInfoBeforeMult()
{
	cout << "[Mult]Mux Info: ";
	for (int i = 0; i < this->V; i++) {
		if (this->mult_mux_array[i] == true)
			cout << "true" << "  ";
		else
			cout << "false" << "  ";
	}
	cout << endl;
}


//mult_time:mt, add_time:at, clock_time: ct, target_clock_cycle target_cycle:
//return value: true: it can pipeline. False: it can't
bool Graph::checkDFFforGraph(float mt, float at, float ct, int target_cycle)
{
	for (int i = 0; i < this->V; i++) {
		if (this->vList[i].op_str == "add") {
			float st = this->vList[i].start_time;
			int imp_index = this->vList[i].opNum;
			int start_cycle = (int)(st / ct);   //todo: non divide as int
			int end_cycle = (int)((st + at) / ct);  //(any time, next cycvle edge] no need assert DFF
			if (end_cycle - start_cycle > target_cycle) {
				return false;   // it is possible do pipeline with this implementation
			}

			if ((st != 0) && (start_cycle%target_cycle == 0)) {
				this->add_DFF_array[imp_index] = true;
				continue;
			}
			int tmp = (int)((st + mt) / (target_cycle*ct));
			if ((end_cycle / target_cycle) != (start_cycle / target_cycle) && ((st + mt) - tmp*(target_cycle*ct) != 0)) {
				this->add_DFF_array[imp_index] = true;
				this->vList[i].start_time = (start_cycle + 1)*ct;
			}
			list<int>::iterator j;
			for (j = adj[i].begin(); j != adj[i].end(); j++) {
				if(this->vList[*j].start_time < (this->vList[i].start_time + at))
					this->vList[*j].start_time = this->vList[i].start_time + at;
			} //should access from smaller time to larger time (toDo)
		}
		else {
			float st = this->vList[i].start_time;
			int imp_index = this->vList[i].opNum;
			int start_cycle = (int)(st / ct);
			int end_cycle = (int)((st + mt) / ct);  //(any time, next cycvle edge] no need assert DFF
			if (end_cycle - start_cycle > target_cycle) {
				return false;   // it is possible do pipeline with this implementation
			}

			if ((st != 0) && (start_cycle%target_cycle == 0)) {
				this->mult_DFF_array[imp_index] = true;
				continue;
			}	
			int tmp = (int)((st + mt) / (target_cycle*ct));
			if ((end_cycle/ target_cycle) != (start_cycle/target_cycle) && ((st+mt) - tmp*(target_cycle*ct) !=0)){
				this->mult_DFF_array[imp_index] = true;
				this->vList[i].start_time = (start_cycle + 1)*ct;
			}

			list<int>::iterator j;
			for (j = adj[i].begin(); j != adj[i].end(); j++) {
				if (this->vList[*j].start_time < (this->vList[i].start_time + mt))
					this->vList[*j].start_time = this->vList[i].start_time + mt;
			} //should access from smaller time to larger time (toDo)

		}
	}
	return true;
}

void Graph::printDFFInfoBeforeAdd()
{
	cout << "[Add]DFF Info: ";
	for (int i = 0; i < this->V; i++) {
		if (this->add_DFF_array[i] == true)
			cout << "true" << "  ";
		else
			cout << "false" << "  ";
	}
	cout << endl;
}

void Graph::printDFFInfoBeforeMult()
{
	cout << "[Mult]DFF Info: ";
	for (int i = 0; i < this->V; i++) {
		if (this->mult_DFF_array[i] == true)
			cout << "true" << "  ";
		else
			cout << "false" << "  ";
	}
	cout << endl;
	cout << "node4_start_time:" << this->vList[4].start_time << endl;
	cout << "node6_start_time:" << this->vList[6].start_time << endl;
}

void Graph::setGraph_FinishTime(float at, float mt)
{
	for (int i = 0; i < this->V; i++) {
		if (this->vList[i].op_str == "add")
			this->vList[i].end_time = this->vList[i].start_time + at;
		else if (this->vList[i].op_str == "mult")
			this->vList[i].end_time = this->vList[i].start_time + mt;
		else
			this->vList[i].end_time = 0; //default
	}

}


void Graph::fillInterConncetPart()
{
	int edge_count = 0;
	for (int i = 0; i < this->V; i++) {
		list<int>::iterator j;
		for (j = adj[i].begin(); j != adj[i].end(); j++) {
			this->interInfo[edge_count].index = edge_count;
			this->interInfo[edge_count].start_time = this->vList[i].end_time;
			this->interInfo[edge_count].end_time = this->vList[*j].start_time;
			this->interInfo[edge_count].bitwidth = this->vList[*j].in_bitwidth;
			this->interInfo[edge_count].from_Vertex = i;
			this->interInfo[edge_count].to_Vertex = (*j);
			this->interInfo[edge_count].conncet_feature = 0;
			this->interInfo[edge_count].DFFcounts = 0; //init
			edge_count++;
		}
	}
}

void Graph::findDFFsinInterConncet(float cycle_time, int target_cycle)
{
	float stage_time = (float)(cycle_time * target_cycle);
	for (int i = 0; i < this->V; i++) {
		if (this->interInfo[i].from_Vertex == this->interInfo[i].to_Vertex) //toDo: init
			break;
		else {
			int x = (int)(this->interInfo[i].end_time/ stage_time);
			int y = (int)(this->interInfo[i].start_time / stage_time);
			float tmp = this->interInfo[i].start_time - (y*stage_time);
			if (tmp == 0)
				this->interInfo[i].DFFcounts = x - y + 1;
			else
				this->interInfo[i].DFFcounts = x - y;
		}
	}
}

void Graph::printInterConncetPart()
{
	for (int i = 0; i < this->V; i++) {
		if (this->interInfo[i].from_Vertex == this->interInfo[i].to_Vertex) //toDo: init
			break;
		else {
			cout << "[inter_index]:" << this->interInfo[i].index << endl;
			cout << "from Node" << this->interInfo[i].from_Vertex << "  to Node" << this->interInfo[i].to_Vertex;
			//cout << "  bitwidth: " << this->interInfo[i].bitwidth << endl;
			cout << "  start time:" << this->interInfo[i].start_time << "  end time:" << this->interInfo[i].end_time <<endl;
			//cout << "  DFF counts: " << this->interInfo[i].DFFcounts << endl << endl;
		}
	}

}

void Graph::fillInterConncetPart_DD_Left()
{
	int edge_count = 0;
	for (int i = 0; i < this->V; i++) {
		list<int>::iterator j;
		for (j = adj[i].begin(); j != adj[i].end(); j++) {
			this->interInfo[edge_count].index = edge_count;
			this->interInfo[edge_count].start_time = this->vList[i].end_time;
			this->interInfo[edge_count].end_time = this->vList[*j].end_time;
			this->interInfo[edge_count].from_Vertex = i;
			this->interInfo[edge_count].to_Vertex = (*j);
			this->interInfo[edge_count].binding = false;
			edge_count++;
		}
	}
}

void Graph::printInterConncetPart_DD_Left()
{
	for (int i = 0; i < this->V; i++) {
		if (this->interInfo[i].from_Vertex == this->interInfo[i].to_Vertex) //toDo: init
			break;
		else {
			cout << "[org_reg_index]:" << this->interInfo[i].index << endl;
			cout << "between Node" << this->interInfo[i].from_Vertex << "  and Node" << this->interInfo[i].to_Vertex;
			//cout << "  bitwidth: " << this->interInfo[i].bitwidth << endl;
			cout << "  start time:" << this->interInfo[i].start_time << "  end time:" << this->interInfo[i].end_time << endl;
			cout << endl;
			//cout << "  DFF counts: " << this->interInfo[i].DFFcounts << endl << endl;
		}
	}
}

void Graph::binding_DD_Left()
{
	for (int rb_v = 0; rb_v < this->V; rb_v++)
	{
		for (int v = 0; v < this->V; v++)
		{
			if (this->interInfo[v].binding == false)
			{
				if (reg_binding[rb_v].size() == 0)
				{
					reg_binding[rb_v].push_back(v);
					this->interInfo[v].binding = true;
				}
				else
				{
					list<int>::iterator i;
					for (i = reg_binding[rb_v].begin(); i != reg_binding[rb_v].end(); ++i)  //check overlap
					{
						float st = this->interInfo[v].start_time;
						float ft = this->interInfo[v].end_time;
						if (st >= this->interInfo[*i].start_time && st < this->interInfo[*i].end_time)
							break;
						if (ft > this->interInfo[*i].start_time && ft <= this->interInfo[*i].end_time)
							break;
					}
					if (i == reg_binding[rb_v].end())  //not overlap in this track
					{
						reg_binding[rb_v].push_back(v);
						this->interInfo[v].binding = true;
					}
				}
			}
		}
	}
	
}

void Graph::printDDBindingResult()
{
	for (int rb_v = 0; rb_v < this->V; rb_v++)
	{
		if (reg_binding[rb_v].size() > 0)
		{
			cout << "reg" << rb_v << " binding with: ";
			list<int>::iterator i;
			for (i = reg_binding[rb_v].begin(); i != reg_binding[rb_v].end(); ++i)  //check overlap
			{
				cout << *i << "  ";
			}
			cout << endl;
		}
	}

}



