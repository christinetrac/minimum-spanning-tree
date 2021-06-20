#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
using namespace std;

class illegal_argument {
public:
    illegal_argument();
};

illegal_argument::illegal_argument(){
}

class DisjointSet{
public:
    int num;
    int *rank;
    int *parent;

    DisjointSet(int n);
    int findSet(int s);
    void unionSet(int a, int b);
};

DisjointSet::DisjointSet(int n){
    num = n;
    rank = new int[num + 1];
    parent = new int[num + 1];

    for(int i = 0; i<=n; i++){
        rank[i] = 0;
        parent[i] = i;
    }
}

int DisjointSet::findSet(int s) {
    if(parent[s] != s){
        parent[s] = findSet(parent[s]); //recursively find the set if s is not already the parent
    }
    return parent[s];
}

void DisjointSet::unionSet(int a, int b) {
    int parent_a = findSet(a);
    int parent_b = findSet(b);
    if(parent_a == parent_b){
        return; //same set already
    } else {
        if(rank[parent_a] > rank[parent_b]){ //which ever has smaller rank must be declared as the child
            rank[parent_a] += rank[parent_b];
            parent[parent_b] = parent_a;
        } else {
            rank[parent_b] += rank[parent_a];
            parent[parent_a] = parent_b;
        }
    }
}

/////////////////////////////////////////////////

class Mst{
public:
    vector<pair<double,pair<int, int>>> edges; //(weight,(u,v))
    int edge_count = 0; //number of edges
    int nodes_count; //number of vertices as stated by user
    bool *connected_array; //keeps track of accessible nodes (checks if graph is connected)

    Mst();
    void init(int m);
    void insert(int node_u, int node_v, double weight);
    void remove(int node_u, int node_v);
    void degree(int node_u);
    void return_edge_count();
    void clear();
    void mst();
    bool is_connected();
    void recursive_is_connected(int q);
};

Mst::Mst() {
}

void Mst::init(int m) {
    try{
        if(m>0){
            nodes_count = m;
            connected_array = new bool[m];
            for(int i = 0; i<m; i++){
                connected_array[i] = false;
            }
            cout << "success" << endl;
        } else {
            throw illegal_argument();
        }
    } catch (illegal_argument){
        cout << "failure" << endl;
    }
}

void Mst::insert(int node_u, int node_v, double weight) {
    try{
        if(weight > 0 && node_u <nodes_count && node_v <nodes_count && node_u!=node_v){
            for(vector <pair<double,pair<int, int>>>::iterator edge=edges.begin(); edge!=edges.end(); edge++){
                if(((edge->second.first == node_u) && (edge->second.second == node_v)) || ((edge->second.first == node_v) && (edge->second.second == node_u))){
                    edge->first = weight; //update the weight of existing edge
                    cout << "success" << endl;
                    return;
                }
            }
            edges.push_back({weight,{node_u,node_v}}); //create a new edge
            edge_count++;
            cout << "success" << endl;
        } else {
            throw illegal_argument(); //u, v and weight are outside valid range
        }
    } catch (illegal_argument){
        cout << "failure" << endl;
    }
}

void Mst::remove(int node_u, int node_v) {
    try{
        if(node_u <nodes_count && node_v <nodes_count){
            for(vector <pair<double,pair<int, int>>>::iterator edge=edges.begin(); edge!=edges.end(); edge++){
                if(((edge->second.first == node_u) && (edge->second.second == node_v)) || ((edge->second.first == node_v) && (edge->second.second == node_u))){
                    edges.erase(edge); //if the node is found then erase it
                    edge_count--;
                    cout << "success" << endl;
                    return;
                }
            }
            throw illegal_argument(); //u and  v do not have an edge
        } else {
            throw illegal_argument(); //u and v are outside valid range
        }
    } catch (illegal_argument){
        cout << "failure" << endl;
    }
}

void Mst::degree(int node_u) {
    try{
        if(node_u < nodes_count){
            int degree = 0;
            for(vector <pair<double,pair<int, int>>>::iterator edge=edges.begin(); edge!=edges.end(); edge++){
                if((edge->second.first == node_u) || (edge->second.second == node_u)){
                    degree++; //if the node is found in an edge then increment the degree
                }
            }
            cout << "degree of " << node_u << " is " << degree << endl;
        } else {
            throw illegal_argument(); //u is not valid node
        }
    } catch (illegal_argument){
        cout << "failure" << endl;
    }
}

void Mst::return_edge_count() {
    cout << "edge count is " << edge_count << endl;
}

void Mst::clear() {
    edges.clear();
    edge_count = 0;
    for(int i = 0; i<nodes_count; i++){
        connected_array[i] = false;
    }
    cout << "success" << endl;
}

/////////////// check if graph is connected ////////////

bool Mst::is_connected() {
    bool connected = true;
    if(edges.empty()){
        connected = false;
    } else {
        int q = edges[0].second.first;
        recursive_is_connected(q);
        for(int i = 0; i<nodes_count; i++){
            if(!connected_array[i]){
                connected = false; //if any entry is false in connected_array, this means a node was not accessible so graph is disconnected
            }
        }
    }
    for(int i = 0; i<nodes_count; i++){
        connected_array[i] = false; //reset connected_array for the future calculation of mst
    }
    return connected;
}

void Mst::recursive_is_connected(int q) {
    connected_array[q] = true;  //set array entry of node when that node has been accessed to true
    for(vector <pair<double,pair<int, int>>>::iterator edge=edges.begin(); edge!=edges.end(); edge++){
        if(edge->second.first == q){
            if(!connected_array[edge->second.second]){
                recursive_is_connected(edge->second.second);
            }
        } else if (edge->second.second == q) {
            if(!connected_array[edge->second.first]){
                recursive_is_connected(edge->second.first);
            }
        }
    }
}

//////////////////////////////////////////////////////

void Mst::mst() {
    try{
        double total_weight = 0;
        sort(edges.begin(), edges.end()); //sort the edges
        DisjointSet set(nodes_count);

        bool connected = is_connected(); //only calculate mst if the graph is connected
        if(connected){
            for(vector <pair<double,pair<int, int>>>::iterator edge=edges.begin(); edge!=edges.end(); edge++){
                int node_u = edge->second.first;
                int node_v = edge->second.second;
                int set_u = set.findSet(node_u);
                int set_v = set.findSet(node_v);
                if(set_u != set_v){
                    double weight  = edge->first;
                    total_weight += weight; //add to total weight only if sets are different then union the sets
                    set.unionSet(set_u, set_v);
                }
            }
            cout << "mst " << total_weight << endl;
        } else {
            throw illegal_argument();
        }
    } catch (illegal_argument){
        cout << "not connected" << endl;
    }
}

int main() {
    Mst test;
    string input;
    while(getline(cin, input)){
        if(input.substr(0, 1) == "n"){
            int m = stoi(input.substr(2));
            test.init(m);
        }
        if(input.substr(0, 1) == "i"){
            string str = input.substr(2);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.insert(stoi(result[0]), stoi(result[1]), stod(result[2]));
        } else if(input.substr(0, 6) == "degree"){
            int u = stoi(input.substr(7));
            test.degree(u);
        } else if(input.substr(0, 1) == "d"){
            string str = input.substr(2);
            vector<string> result;
            stringstream ss(str);
            while(ss.good()){
                string substr;
                getline(ss, substr, ';');
                result.push_back(substr);
            }
            test.remove(stoi(result[0]), stoi(result[1]));
        } else if(input.substr(0) == "edge_count"){
            test.return_edge_count();
        } else if (input.substr(0) == "clear"){
            test.clear();
        } else if (input.substr(0) == "mst"){
            test.mst();
        }
    }
    return 0;
}
