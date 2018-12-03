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
vector <node> g;

double min(double a,double b){
    if(a<=b)
	return a;
    else
	return b;
}

//Function g(s) to be written that represents the path cost from s to the goal.

void compute_cost(node s,node sa,node sb){//Keep in mind that the total path cost is path length * cost associated with that path.
    if(sa is a diagonal neighborof s){
	s1 = sb; s2 = sa;
    }
    else{
	s1 = sa; s2 = sb;
    }

    c is traversal cost of cell with corners s, s1, s2;
    b is traversal cost of cell with corners s, s1 but not s2;

    if (min(c, b) = 1000000){
	vs = 1000000;
    }
    else if (g(s1) ≤ g(s2)){
        vs = min(c,b) + g(s1); //Fig 5(ii) of paper
    }
    else{
	f = g(s1) − g(s2);
	if (f ≤ b){
	    if (c ≤ f){
		vs = c*sqrt(2) + g(s2); //Directly s to s2
	    }
	    else{
		y = min(f/sqrt(c*c-f*f) , 1);
		vs = c*sqrt(1+y*y) + f(1 − y) + g(s2); //Fig 5(iv) of paper
	    }
	}
	else{
	    if (c ≤ b){
		vs = c*sqrt(2) + g(s2); //Directly s to s2
	    }
	    else{
		x = 1 − min( b/sqrt(c*c-b*b) , 1);
		vs = c*sqrt(1+(1-x)*(1-x)) + bx + g(s2); //Fig 5(iii) of paper
	    }
	}
    }
return vs;
}

int main(){
    node s,sa,sb;//Three points in the graph among which s is the starting point taking inputs for x,y and cost for each node.
    compute_cost(s,sa,sb);
}
