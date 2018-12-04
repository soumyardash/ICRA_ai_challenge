#include<ros/ros.h>
#include<bits/stdc++.h>
#include<iostream>
#include<math.h>

using namespace std;

typedef struct node{
    double x;
    double y;
    double cost;
}node;

vector<vector<node> > vec;
vector<vector<double> > rhs;
vector<vector<double> > g;
node* start;

double min(double a,double b){
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

void compute_cost(node* s,node* sa, node* sb){//Keep in mind that the total path cost is path length * cost associated with that path.
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
		if (f <= b){
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

queue <node> open;

void updateState() {
	if(g[s->x][s->y] - rhs[s->x][s->y] < 0.00001)
}

int main(){
    node s,sa,sb;//Three points in the graph among which s is the starting point taking inputs for x,y and cost for each node.
    compute_cost(s,sa,sb);
}
