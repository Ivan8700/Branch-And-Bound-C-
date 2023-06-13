#pragma once
#include <iostream>
#include <list>
using std::cout; using std::cin; using std::endl;
using std::pair; using std::list;

class Node
{
private:
	int m;
	int L;
	int U;
	int height;
	list<pair<int, int>>* schedule;
	int* sumArray;
	int child;
	
public:
	Node(int m,int l = -1, int u = -1, int h = 0, list<pair<int, int>>* s = NULL, int* sum = NULL, int c = -1);
	Node(const Node& n);
	~Node();
	int getL();
	void setL(const int& l);
	int getU();
	void setU(const int& u);
	int getH();
	void setH(const int& h);
	list<pair<int, int>>* getSchedule();
	void addNodeInShedule(const int& num, const int& indexOfJob, const int& indexMachine, int arr[]);
	int* getSumArray();
	int getChild();

};

