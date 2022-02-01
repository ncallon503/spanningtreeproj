/* Nathan Callon
7/17/2021
CS251
Minimum Spanning Tree, Graph Theory Assignment*/

#include <iostream>		
#include <algorithm>		
#include <vector>		
#include <utility>
#include <string> 
#include <algorithm>

using namespace std;

struct edge {
	int start;
	int end;
	int weight;
};

struct vertex {
	int whichVertexStart = 0;
	int whichVertexEnd = 0;
	int weight = 0;
	bool reversed = false;
};

struct addedV {
	addedV(int size)
	{
		isAdded = new bool[size];
	}
	bool* isAdded;
};

bool compareByWeight(const edge& a, const edge& b)
{
	return a.weight < b.weight;
}

void fillWamUpFomAdjMatrix(vector<edge>& wam) {
	cout << endl << "Enter the weighted adjacency matrix values, -1 to stop\n\n ";
	int numOfVertices = 0;
	edge Edge;
	do {

		cin >> Edge.weight;
		wam.push_back(Edge);
		if (Edge.weight == -1) {
			wam.pop_back();
			break;
		}
	} while (true);
	numOfVertices = sqrt(wam.size());
	//arr[i * cols + j]
	for (int i = 0; i < numOfVertices; ++i) {
		for (int j = 0; j < numOfVertices; ++j)
		{
			wam.at(i * numOfVertices + j).start = i;
			wam.at(i * numOfVertices + j).end = j;
		}
	}
}


void printGraph(vector<edge>& WAM)
{
	int weight = 0;
	int vertices = sqrt(WAM.size());

	for (int i = 0; i < vertices; ++i)
	{
		for (int j = 0; j < vertices; ++j) {
			weight = WAM.at(i * vertices + j).weight;
			cout << "\t" << weight;
		}
		cout << endl;
	}

}


int find_set(int i, vector <int>& parent) {
	// If i is the parent of itself
	if (i == parent[i])
		return i;
	else
		// Else if i is not the parent of itself
		// Then i is not the representative of his set,
		// so we recursively call Find on its parent
		return find_set(parent[i], parent);
}

void union_set(int u, int v, vector <int>& parent) {
	parent[u] = parent[v];
}

int getTheMStEdge(vector <edge>& visited, int a, int b) {
	int u, v;
	bool ok = false;
	int weight = 0;
	for (int i = 0; i < visited.size(); ++i)
	{
		u = visited[i].start;
		v = visited[i].end;
		ok = (a == u && b == v) || (a == v && b == u);
		if (ok) {
			weight = visited[i].weight;
			break;
		}
	}
	return weight;
}

void printMST(int vertices, vector <edge>& visited) {
	cout << endl;
	for (int i = 0; i < vertices; ++i)
	{
		for (int j = 0; j < vertices; ++j) {
			cout << "\t" << getTheMStEdge(visited, i, j);
		}
		cout << endl;
	}
}

void Prims(vector<edge>& wam) {
	int vertices = sqrt(wam.size());
	vector<int> includedVer;
	vector<vertex> adjacent;

	int count = 0, indexNonZero = 0, whichVertex = 0;

	std::cout << "\nRemoving duplicates to make our lives easier:\n";

	for (int i = 0; i < vertices; ++i)
	{
		for (int j = i + 1; j < vertices; ++j) {
			wam.at(j * vertices + i).weight = 0;
		}
	}

	vector<edge> temporary(wam);

	for (int i = 0; i < temporary.size(); i++)
	{
		temporary[i].weight = 0;
	}

	printGraph(wam);

	std::cout << "\nChoosing first vertex v:\n";

	int whichRow = 0;
	int tester = 0;

	for (int i = 0; i < wam.size(); ++i) {

		tester = sqrt(wam.size());
		if (i % tester == 0 && i != 0) whichRow++;

		if (wam.at(i).weight > 0)
		{
			indexNonZero = i;
			break;
		}
	}

	includedVer.push_back(whichRow); //think about it, if whichRow is 0, pushes back v0, if whichRow is 2, pushes back v2, etc

	int numOfIncluded = 1;
	vertex temp;

	bool* added = new bool[vertices];
	for (int i = 0; i < vertices; i++)
		added[i] = false;

	added[includedVer[0]] = true;

	int indexOfSmallest = 0;
	int smallest = 0;

	while (true)
	{
		adjacent.clear();

		for (int j = 0; j < numOfIncluded; j++)
		{
			for (int i = 0; i < wam.size(); i++)
			{
				if (wam.at(i).start == includedVer[j] && wam.at(i).weight > 0 && !added[wam.at(i).end]) /*if we find a "startpoint" of an edge that starts at a vertex in our set of included
					vertexes so far, then the endpoint will be included in possible adjacencies (we check with added[wam.at(i).end] to make sure
					we don't add an edge to a vertex that's already included*/
				{
					temp.reversed = false;
					temp.weight = wam.at(i).weight;
					temp.whichVertexEnd = wam.at(i).end;
					temp.whichVertexStart = wam.at(i).start;
					adjacent.push_back(temp);

					std::cout << "Pushed back " << wam.at(i).end << " as adjacent to " << wam.at(i).start << " with weight " << wam.at(i).weight << "\n";
				}
				else if (wam.at(i).end == includedVer[j] && wam.at(i).weight > 0 && !added[wam.at(i).start]) //for annoying start/end reversal
				 //we don't have a directed graph, but if we only include the "if()" above, then it won't include an edge that's "labelled"
				 //in a way it doesnt like, like it sometimes will skip over an edge "from 3 to 2" but will take an edge "from 2 to 3".
				{
					temp.reversed = true;
					temp.weight = wam.at(i).weight;
					temp.whichVertexEnd = wam.at(i).start;
					temp.whichVertexStart = wam.at(i).end;
					adjacent.push_back(temp);

					std::cout << "Pushed back " << wam.at(i).end << " as adjacent to " << wam.at(i).start << " with weight " << wam.at(i).weight << "\n";
				}
			}
		}

		//find smallest cost
		smallest = 10000000;
		for (int i = 0; i < adjacent.size(); i++)
		{
			if (adjacent[i].weight < smallest)
			{
				smallest = adjacent[i].weight;
				indexOfSmallest = i;
			}
		}

		//finally, finally add the smallest cost edge and vertex to "included" ones and set added[(of smallest cost vertex/edge)] to true

		added[adjacent[indexOfSmallest].whichVertexEnd] = true;
		//if(!adjacent[indexOfSmallest].reversed)
		temporary[(adjacent[indexOfSmallest].whichVertexStart * vertices) + adjacent[indexOfSmallest].whichVertexEnd].weight = adjacent[indexOfSmallest].weight;
		//else
		temporary[(adjacent[indexOfSmallest].whichVertexEnd * vertices) + adjacent[indexOfSmallest].whichVertexStart].weight = adjacent[indexOfSmallest].weight;
		includedVer.push_back(adjacent[indexOfSmallest].whichVertexEnd);
		numOfIncluded++;

		if (!adjacent[indexOfSmallest].reversed)
			std::cout << "Added edge from " << adjacent[indexOfSmallest].whichVertexStart << " to " << adjacent[indexOfSmallest].whichVertexEnd << "\n";
		else
			std::cout << "Added edge from " << adjacent[indexOfSmallest].whichVertexEnd << " to " << adjacent[indexOfSmallest].whichVertexStart << "\n";


		if (includedVer.size() == vertices)
		{
			std::cout << "\nOur final minimum spanning tree using Prim's method:\n";
			printGraph(temporary);
			std::cout << "\n=====================================\n";
			break; //this is how we know all vertices are mapped
		}
		printGraph(temporary);
	}
}

void Kruskal(vector<edge>& wam) {
	int vertices = sqrt(wam.size());
	vector <edge> visited;
	vector <int> parent;
	int i = 0, ctr = 0, indexOfNonZero = 0, min = 0;
	cout << endl << endl;

	//cout << endl << " Following the example at  https://www.youtube.com/watch?v=CY7P5gooBy0" << endl;
	//cout << " I am going to retain just the upper triangle symmetric matrix weight values to remove doubles " << endl << endl;
	for (int i = 0; i < vertices; ++i)
	{
		for (int j = i + 1; j < vertices; ++j) {
			wam.at(j * vertices + i).weight = 0;
		}
	}
	//printGraph(wam);
	std::sort(wam.begin(), wam.end(), compareByWeight);
	for (i = 0; i < wam.size(); ++i) {
		if (wam.at(i).weight > 0)
		{
			indexOfNonZero = i;
			break;
		}
	}

	for (i = 0; i < vertices; ++i) {
		parent.push_back(i);
	}

	min = wam.at(indexOfNonZero).weight;

	int u, v;
	while (true)
	{

		u = wam.at(indexOfNonZero).start;
		v = wam.at(indexOfNonZero).end;
		u = find_set(u, parent);
		v = find_set(v, parent);
		if (u != v) {
			visited.push_back(wam.at(indexOfNonZero)); // add to tree
			cout << endl << "\tAdded edge " << wam.at(indexOfNonZero).start << " - " << wam.at(indexOfNonZero).end << " : " << wam.at(indexOfNonZero).weight;
			union_set(u, v, parent);
			++ctr;
		}
		indexOfNonZero++;
		if (ctr == vertices - 1)
			break;
	}
	cout << endl << "\n\tUsing Kruskal's method: " << endl;
	printMST(vertices, visited);
}

/*
	   0       146     50      120     102     41
		146     0       140     151     150     147
		50      140     0       83      122     72
		120     151     83      0       81      113
		102     150     122     81      0       84
		41      147     72      113     84      0
		-1
*/
int main()
{
	vector<edge> WAM;
	fillWamUpFomAdjMatrix(WAM);
	cout << endl << " The adjacency matrix of the graph to construct the MST of is: " << endl;
	cout << endl;
	vector<edge> WAM2(WAM);
	printGraph(WAM);
	Prims(WAM2);
	Kruskal(WAM);
	return 0;
}