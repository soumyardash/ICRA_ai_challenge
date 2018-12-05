#include<ros/ros.h>
#include<bits/stdc++.h>
#include<iostream>
#include<math.h>

using namespace std;

typedef struct node{
    double x;
    double y;
    double cost;
    double key;
    node* bptr;
}node;

vector<vector<node*> > vec;
vector<vector<double> > rhs;
vector<vector<double> > g;
node* start;

double min(double a, double b){
    if(a<=b) return a;
    else return b;
}

node* determineAdjacent(node* s, node* s1, node* s2) {
    node* adj;
    if(s1.x == s2.x) {
	if(s1.y > s2.y) adj = vec[s1.x][s1.y+1];
	else adj = vec[s1.x][s1.y-1]
    }
    else if(s1.y == s2.y) {
	if(s1.x > s2.x) adj = vec[s1.x+1][s1.y]
	else adj = vec[s1.x-1][s1.y]
    }
    else ROS_ERROR("Cannot determine adjacent node");
}

node* determineNeighbour(node* s, node* s1, int c) {
    node* neighbour;
    if(s.x == s1.x) {
	if(s.y < s1.y) {
	    if(c) neighbour = vec[s.x-1][s1.y];
	    else neighbour = vec[s.x+1][s1.y];
	}
	else if(s.y > s1.y){
	    if(c) neighbour = vec[s.x+1][s1.y];
	    else neighbour = vec[s.x-1][s1.y];
	}
	else {
	    ROS_ERROR("Cannot determine neighbour node");
	}
    }
    else if(s.y == s1.y) {
	if(s.x < s1.x) {
	    if(c) neighbour = vec[s1.x][s1.y+1];
	    else neighbour = vec[s1.x][s1.y-1];
	}
	else if(s.x > s1.x){
	    if(c) neighbour = vec[s1.x][s1.y-1];
	    else neighbour = vec[s1.x][s1.y+1];
	}
	else {
	    ROS_ERROR("Cannot determine neighbour node");
	}
    }
    else ROS_ERROR("Cannot determine neighbour node");
}

double compute_cost(node* s,node* sa, node* sb){//Keep in mind that the total path cost is path length * cost associated with that path.
    node* s1, s2;
    if(abs(sa->x-s->x) == 1 && abs(sa->y-s->y) == 1) s1 = sb; s2 = sa;
    else s1 = sa; s2 = sb;
    c = s->cost;
    node* adj = determineAdjacent(s, s1, s2);
    b = min(s.cost, adj.cost);

    double vs;
    if (min(c, b) == 1000000.0){
	vs = 1000000.0;
    }
    else if (g[s1->x][s1->y] <= g[s2->x][s2->y]){
	vs = min(c,b) + g[s1->x][s1->y]; //Fig 5(ii) of paper
    }
    else{
	double f = g[s1->x][s1->y] - g[s2->x][s2->y];
	if(f <= b) {
	    if (c <= f){
		vs = c*sqrt(2) + g[s2->x][s2->y]; //Directly s to s2
	    }
	    else{
		double y = min(f/sqrt(c*c-f*f), 1);
		vs = c*sqrt(1+y*y) + f*(1 − y) + g[s2->x][s2->y]; //Fig 5(iv) of paper
	    }
	}
	else{
	    if (c <= b){
		vs = c*sqrt(2) + g[s2->x][s2->y]; //Directly s to s2
	    }
	    else{
		double x = 1 − min(b/sqrt(c*c-b*b), 1);
		vs = c*sqrt(1+(1-x)*(1-x)) + b*x + g[s2->x][s2->y]; //Fig 5(iii) of paper
	    }
	}
    }
    return vs;
}

pair<double, double> key(node* s) {
    double h = sqrt((s->x - start->x)*(s->x - start->x) + (s->y - start->y)*(s->y - start->y))/2
	return make_pair(min(g[s->x][s->y], rhs[s->x][s->y]) + h, min(g[s->x][s->y], rhs[s->x][s->y]));
}

vector<node*> open;

void insertInOpen(node* s) {
    int i, j;
    int inserted;
    for(i = 0, j = open.size() ; i < j ; ) {
	if(s->key < open[(i+j)/2]) j=(i+j)/2 - 1;
	else if(s->key > open[(i+j)/2]) i = (i+j)/2 + 1;
	else {
	    open.insert(open.begin() + (i+j)/2, s);
	    inserted = 1;
	    break;
	}
    }
    if(!inserted) open.insert(open.begin() + (i+j)/2, s);
}

int searchInOpen(node* s) {
    for(int i = 0, j = open.size() ; i < j ;) {
	if(s.key == open[(i+j)/2]) return (i+j)/2;
    }
    return -1;
}

void updateState(node* s) {
    if(!(g[s->x][s->y] - rhs[s->x][s->y] < 0.00001)) {
	s.key = key(s);
	insertInOpen(s);
    }
    else {
	int i = searchInOpen(s);
	if(i != -1) open.erase(open.begin() + i);
    }
}

void computeShortestPath() {
    while(open[0]->key < start->key || abs(g[start->x][start->y] - rhs[start->x][start->y]) > 0.0001) { //4
	node* s = open[0]; //5
	if(g[s->x][s->y] > rhs[s->x][s->y]) { //6
	    g[s->x][s->y] = rhs[s->x][s->y]; //7
	    open.erase(open.begin()); //8
	    node* s1;
	    node* s2;
	    for(int i = -1 ; i < 2 ; i++) {
		for(int j = -1 ; j < 2 ; j++) {
		    if(!(i == 0 && j == 0)) {
			if(s->x+i < vec.size() && s->x+i >= 0 && s->y+j < vec[s->x+i].size() && s->y+j > 0) {
			    s1 = vec[s->x+i][s->y+j];
			    //TODO 10, 11
			    double rhsold = rhs[s1->x][s1->y]; //12
			    double cost = computeCost(s1, s, determineNeighbour(s1, s, 0));
			    if(rhs[s1->x][s1->y] > cost){ //13
				rhs[s1->x][s1->y] = cost; //14
				s1->bptr = s; //15
			    }
			    s2 = determineNeighbour(s1, s, 1);
			    if(rhs[s1->x][s1->y] > computeCost(s1, s, s2)) { //16
				rhs[s1->x][s1->y] = computeCost(s1, s2, s); //17
				s1->bptr = s2; //18
			    }
			    if(!(abs(rhsold - rhs[s1->x][s1->y]) < 0.00001)) { //19
				updateState(s1); //20
			    }
			}
		    }
		}
	    }
	}
	else { //21
	    node* sk;
	    double min = 0.0;
	    for(int i = -1 ; i < 2 ; i++) {
		for(int j = -1 ; j < 2 ; j++) {
		    if(!(i == 0 && j == 0)) {
			if(s->x+i < vec.size() && s->x+i >= 0 && s->y+j < vec[s->x+i].size() && s->y+j > 0) {
			    double cost = computeCost(s, vec[s->x+i][s->y+j], determineNeighbour(s, vec[s->x+i][s->y+j], 0));
			    if(cost < min) {
				min = cost;
				sk = vec[s->x+i][s->y+j];
			    }
			}
		    }
		}
	    }
	    rhs[s->x][s->y] = min; //22
	    s->bptr = sk; //23
	    if(g[s->x][s->y] < rhs[s->x][s->y]) { //24
		g[s->x][s->y] = 1000000.0; //25
		for(int i = -1 ; i < 2 ; i++) {
		    for(int j = -1 ; j < 2 ; j++) { //26
			if(!(i == 0 && j == 0)) {
			    if(s->x+i < vec.size() && s->x+i >= 0 && s->y+j < vec[s->x+i].size() && s->y+j > 0) {
				node* s1 = vec[s->x+i][s->y+j];
				if(s1->bptr == s || s1->bptr == determineNeighbour(s1, s, 0)){ //27
				    if(rhs[s1->x][s1->y] != computeCost(s1, s1->bptr, determineNeighbour(s1, s1->bptr, 0))) { //28
					if(g[s1->x][s1->y] < rhs[s1->x][s1->y] || searchInOpen(s1) == -1) { //29
					    rhs[s1->x][s1->y] = 1000000.0; //30
					    updateState(s1); //31
					}
					else { //32
					    node* skk;
					    double mink;
					    for(int i = -1 ; i < 2 ; i++) {
						for(int j = -1 ; j < 2 ; j++) {
						    if(!(i == 0 && j == 0)) {
							if(s1->x+i < vec.size() && s1->x+i >= 0 && s1->y+j < vec[s1->x+i].size() && s1->y+j > 0) {
							    double cost = computeCost(s1, vec[s1->x+i][s1->y+j], determineNeighbour(s1, vec[s1->x+i][s1->y+j], 0));
							    if(cost < min) {
								mink = cost;
								skk = vec[s1->x+i][s1->y+j];
							    }
							}
						    }
						}
					    }
					    rhs[s1->x][s1->y] = mink; //33
					    s1->bptr = skk; //34
					    updateState(s1); //35
					}
				    }
				}
			    }
			}
		    }
		}
	    }
	    updateState(s); //36
	}
    }
}

int main(){
    node s,sa,sb;//Three points in the graph among which s is the starting point taking inputs for x,y and cost for each node.
    compute_cost(s,sa,sb);
}
