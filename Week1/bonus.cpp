#include <iostream>
using std::cin;
using std::cout;
using std::endl;

struct Node
{
    char name[10];
};

struct Edge
{
    Node start;
    Node end;
};

struct Graph
{
    unsigned nodeCount = 0;
    unsigned edgeCount = 0;
    Node* nodes = nullptr;
    Edge* edges = nullptr;
};

Graph initializeGraph()
{
    unsigned m = 0;
    cin >> m;

    Graph g;
    g.edgeCount = m;

    return g;
}

void deleteGraph(Graph& graph)
{
    delete[] graph.edges;
    delete[] graph.nodes;
}

void printNodesInGraph(const Graph& graph)
{
    if (!graph.nodes)
        return;

    unsigned short nodesCount = graph.nodeCount;

    cout << "List of nodes: \n";

    for (unsigned short i = 0; i < nodesCount; i++)
    {
        Node currentNode = graph.nodes[i];
        cout << currentNode.name <<  '\n';
    }
}

void printEdgesInGraph(const Graph& graph)
{
    if (!graph.edges)
        return;

    unsigned short edgesCount = graph.edgeCount;

    cout << "List of edges: \n";

    for (unsigned short i = 0; i < edgesCount; i++)
    {
        cout << graph.edges[i].start.name << " -> " << graph.edges[i].end.name << '\n';
    }
}

bool doesNodeExist(const Graph& graph, const Node& node)
{
    if (!graph.nodes)
    {
        return false;
    }

    for (unsigned i = 0; i < graph.nodeCount; i++)
    {
        Node currentNode = graph.nodes[i];

        if (strcmp(currentNode.name, node.name) == 0)
        {
            return true;
        }
    }

    return false;
}

bool doesEdgeExist(const Graph& graph, const Edge& edge)
{
    if (!graph.edges)
    {
        return false;
    }

    for (unsigned i = 0; i < graph.edgeCount; i++)
    {
        Edge currentEdge = graph.edges[i];
        //cout << "CESN: \'" << currentEdge.start.name << "\' -> CEEN: \'" << currentEdge.end.name << "\'" << endl;
        //cout << "ESN: \'" << edge.start.name << "\' -> EEN: \'" << edge.end.name << "\'" << endl;
        if (strcmp(currentEdge.start.name, edge.start.name) == 0 && strcmp(currentEdge.end.name, edge.end.name) == 0)
        {
            return true;
        }
    }

    return false;
}

void addNewNodeToList(Graph& graph, const Node& newNode)
{
    graph.nodeCount++;
    size_t new_size = graph.nodeCount;
    Node* newList = new Node[new_size];

    if (graph.nodes)
    {
        for (unsigned i = 0; i < new_size - 1; i++)
        {
            newList[i] = graph.nodes[i];
        }
    }

    newList[new_size - 1] = newNode;

    delete graph.nodes;

    graph.nodes = newList;
}

void addNewEdgeToList(Graph& graph, const Edge& newEdge)
{
    graph.edgeCount++;
    size_t new_size = graph.edgeCount;
    Edge* newList = new Edge[new_size];

    if (graph.edges)
    {
        for (unsigned i = 0; i < new_size - 1; i++)
        {
            newList[i] = graph.edges[i];
        }
    }

    newList[new_size - 1] = newEdge;

    delete graph.edges;

    graph.edges = newList;
}

void connectNodes(Graph& graph, const Node& n1, const Node& n2)
{
    if (!doesNodeExist(graph, n1))
    {
        addNewNodeToList(graph, n1);
    }
    if (!doesNodeExist(graph, n2))
    {
        addNewNodeToList(graph, n2);
    }
    Edge e = { n1, n2 };
    if (doesEdgeExist(graph, e))
    {
        return;
    }
    else
    {
        addNewEdgeToList(graph, e);
    }
}

void readEdge(Graph& graph)
{
    Node n1, n2;;
    cin >> n1.name >> n2.name;

    connectNodes(graph, n1, n2);
}

bool areNodesInEdge(const Graph& graph, const Node& n1, const Node& n2)
{
    if (!graph.edges)
        return false;

    for (unsigned i = 0; i < graph.edgeCount; i++)
    {
        Edge currentEdge = graph.edges[i];
        if ((strcmp(currentEdge.start.name, n1.name) == 0 && strcmp(currentEdge.end.name, n2.name) == 0) ||
            (strcmp(currentEdge.start.name, n2.name) == 0 && strcmp(currentEdge.end.name, n1.name) == 0))
        {
            return true;
        }
    }

    return false;
}

bool isNodeConnectedToAllOthers(const Graph& graph, const Node& node)
{
    for (unsigned i = 0; i < graph.nodeCount; i++)
    {
        const Node currentNode = graph.nodes[i];
        if (!areNodesInEdge(graph, node, currentNode) && strcmp(node.name, currentNode.name) != 0)
        {
            return false;
        }
    }
}

bool isGraphFull(const Graph& graph)
{
    for (unsigned i = 0; i < graph.nodeCount; i++)
    {
        if (!isNodeConnectedToAllOthers(graph, graph.nodes[i]))
        {
            return false;
        }
    }

    return true;
}

int main()
{
    unsigned _edgeCount;
    cin >> _edgeCount;
    Graph graph;

    for (unsigned i = 0; i < _edgeCount; i++)
    {
        readEdge(graph);
    }

    printNodesInGraph(graph);
    printEdgesInGraph(graph);
    cout << "The graph is " << (isGraphFull(graph) ? "full.\n" : "not full.\n");
    deleteGraph(graph);
}
