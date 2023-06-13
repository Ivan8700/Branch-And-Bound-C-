#include "Node.h"
Node::Node(int m, int l, int u, int h, list<pair<int, int>>* s, int* sum, int c) : L(l), U(u), height(h), child(c), m(m)
{
	this->sumArray = new int[m];
	this->schedule = new list<pair<int, int>>[m];
	if (sum == NULL)
	{
		for (int i = 0; i < m; i++)
			this->sumArray[i] = 0;
	}
	else
		for (int i = 0; i < m; i++)
			this->sumArray[i] = sum[i];
	if (s != NULL)
		for (int i = 0; i < m; i++)
			this->schedule[i] = s[i];
}

Node::Node(const Node& n)
{
	this->sumArray = new int[m];
	this->schedule = new list<pair<int, int>>[m];
	this->L = n.L;
	this->U = n.U;
	this->height = n.height;
	for (int i = 0; i < m; i++)
	{
		this->schedule[i].assign(n.schedule[i].begin(), n.schedule[i].end());
		this->sumArray[i] = n.sumArray[i];
	}
	this->child = n.child;
}

Node::~Node()
{
	delete[] this->schedule;
	delete[] this->sumArray;
}

int Node::getL() { return this->L; }
void Node::setL(const int& l) { this->L = l; }
int Node::getU() { return this->U; }
void Node::setU(const int& u) { this->U = u; }
int Node::getH() { return this->height; }
void Node::setH(const int& h) { this->height = h; }
list<pair<int,int>>* Node::getSchedule() { return this->schedule; }
void Node::addNodeInShedule(const int& num, const int& indexOfJob, const int& indexMachine, int arr[])
{
	this->schedule[indexMachine].push_back(std::make_pair(num, indexOfJob));
	this->sumArray[indexMachine] += num;
}
int* Node::getSumArray() { return this->sumArray; }
int Node::getChild() { return this->child; }