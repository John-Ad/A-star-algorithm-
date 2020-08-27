#include <iostream>
#include <vector>

#define INF 99999

using namespace std;

class graph
{
private:
	struct vertex {
		int v;
		bool closed;
		int distFromSrc;
		int hDist;
		vertex* prev;

		int x;
		int y;
	};
	struct adjVert {
		int v;
		int weight;
	};

	vector<vertex> vertices;
	vector<vector<adjVert>> adjList;

public:
	graph(int width, int size) {		// the graph is grid based and each vertex has x and y values
		vertex v;
		v.distFromSrc = INF;
		v.hDist = INF;
		v.closed = false;
		v.prev = NULL;

		int xCount = 0;
		int yCount = 0;
		int wdth = width;

		for (int i = 0; i < size; i++)		// the index matches the vertex number allowing for access without needing to loop through and find a specific vertex
		{									// this loop also initialises the x and y values of each vertex based on the width given
			if (i == wdth)
			{
				yCount += 10;
				xCount = 0;
				wdth += width;
			}
			v.v = i;
			v.x = xCount;
			v.y = yCount;
			vertices.push_back(v);
			xCount += 10;
		}

		//for debugging
		/*for (int i = 0; i < vertices.size(); i++)
		{
			cout << vertices[i].v << ", " << vertices[i].x << ", " << vertices[i].y << endl;
		}*/
	}

	void populateList(int x) {
		int xHolder{ x - 1 };		// xHolder indicates the last element of the current row in the grid
		adjVert v;

		for (int i = 0; i < vertices.size(); i++)
		{
			adjList.push_back(vector<adjVert>());
		}

		for (int i = 0; i < adjList.size(); i++)			// refer to a drawn grid when working through this algorithm
		{
			for (int e = 0; e < adjList.size(); e++)
			{
				if (e == i + 1 && i != xHolder)		// links a vertex to the one to the right of it
				{
					v.v = vertices[e].v;
					v.weight = 1;
					adjList[i].push_back(v);
				}
				else if (e == i - 1 && e != xHolder)	// links a vertex to the one to the left of it
				{
					v.v = vertices[e].v;
					v.weight = 1;
					adjList[i].push_back(v);
				}
				else if (e == i + x)				//links a vertex to the one under it
				{
					v.v = vertices[e].v;
					v.weight = 1;
					adjList[i].push_back(v);
				}
				else if (e == i - x)				//links a vertext to the one above it
				{
					v.v = vertices[e].v;
					v.weight = 1;
					adjList[i].push_back(v);
				}

				if (e == xHolder + 1)					//xholder signals the end of a row. Doubling it moves the algorithm to the next row
				{
					xHolder += x;
				}
			}
			xHolder = x - 1;
		}
	}
	void addWalls() {						//refer to a drawn out grid
		vertices[1].closed = true;
		vertices[10].closed = true;
		vertices[7].closed = true;
	}
	void adjVerts(int e, vector<adjVert>& adjVert) {
		if (adjVert.size() > 0)		//function automatically clears the vector passed in if necessary
			adjVert.clear();

		for (int i = 0; i < adjList[e].size(); i++)
		{
			adjVert.push_back(adjList[e][i]);
		}
	}
	void shortestPath(int start, int end) {
		bool pathFound = false;			//controls main loop
		int currentVert = start;		//represents current vertex
		int vertToDel = INF;
		int shortestDist = INF;			//helps choose the next vertex to visit
		int hX = 0;						//heuristic x and y variables
		int hY = 0;

		vector<vertex> open;		//open vertices
		vector<vertex> path;		//used to display path
		vector<adjVert> adjVert;		//used to store adjacent vertices

		currentVert = start;
		vertices[currentVert].distFromSrc = 0;
		vertices[currentVert].hDist = 0;
		open.push_back(vertices[currentVert]);

		do
		{
			for (int i = 0; i < open.size(); i++)
			{
				if (open[i].hDist < shortestDist)
				{
					shortestDist = open[i].hDist;
					currentVert = open[i].v;
					vertToDel = i;
				}
			}
			open.erase(open.begin() + vertToDel);
			shortestDist = INF;
			vertToDel = INF;

			if (currentVert == end)
				pathFound = true;

			adjVerts(currentVert, adjVert);
			for (int i = 0; i < adjVert.size(); i++)
			{
				if (vertices[currentVert].distFromSrc + adjVert[i].weight < vertices[adjVert[i].v].distFromSrc)
				{
					vertices[adjVert[i].v].distFromSrc = vertices[currentVert].distFromSrc + adjVert[i].weight;
					vertices[adjVert[i].v].prev = &vertices[currentVert];

					//gets the number of tiles between the two vertices
					hX = vertices[end].x - vertices[adjVert[i].v].x;
					hY = vertices[end].y - vertices[adjVert[i].v].y;

					if (hX < 0)
						hX = 0 - hX;
					if (hY < 0)
						hY = 0 - hY;
					//---------------------------------------------------

					vertices[adjVert[i].v].hDist = vertices[adjVert[i].v].distFromSrc + (hX + hY);	//heuristic value
				}

				if (!vertices[adjVert[i].v].closed)
				{
					open.push_back(vertices[adjVert[i].v]);
				}
			}
			vertices[currentVert].closed = true;	//close current vertex before choosing a new one
			cout << "current vert: " << currentVert << endl;
		} while (!pathFound);

		//displays path
		path.push_back(vertices[end]);
		while (path[path.size() - 1].prev != NULL)
		{
			path.push_back(*path[path.size() - 1].prev);
		}
		cout << "start: " << vertices[start].v << endl << "end: " << vertices[end].v << endl;
		for (int i = path.size() - 1; i >= 0; i--)
		{
			cout << endl << path[i].v;
		}
	}
};

int main()
{
	graph g(5, 15);				// param 1 is the width, param 2 is the total number of vertices
	g.populateList(5);				// param is the width
	g.addWalls();
	g.shortestPath(0, 14);		// param 1 is the start, param 2 is the end

	cin.get();
	return 0;
}
