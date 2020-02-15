#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <chrono>
using namespace std;
const int MAXN = 1e5; //max possible |V(G)|
long long V, E; // size of input graph
vector<int> G[MAXN], D[MAXN]; //input graph D is directed graph
set<int> C, I;
int degree[MAXN];
int k, s;
long double alpha = 0;
int ans_size = V;
void input_graph()
{
    ifstream fin("route.in");
    fin >> k >> s;
    fin >> V >> E;
    int tmp_size = E;
    while(tmp_size--){
        int u, v;
        fin >> u >> v;
        u--;--v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    fin.close();
}

void print_graph(vector<int> G[], int size)
{
    for(int i = 0; i < size; i++){
        cout << i << " : ";
        for(int j = 0; j < G[i].size(); j++)
            cout << G[i][j] << " ";
        cout << endl;
    }

}

void create_Dgraph()//TODO fix this, it creates Dgraph for the whole graph
{
    for(int i = 0 ; i < V ; i++)
        D[i].clear();
    long long cur_E = 0;
    for(int i: I){
        for(int j = 0; j < G[i].size(); j++){
            if(G[i].size() > G[G[i][j]].size())
                D[i].push_back(G[i][j]);
            else if(G[i].size() == G[G[i][j]].size())
                if(i > G[i][j])
                    D[i].push_back(G[i][j]);
        }
        cur_E += D[i].size();
    }
//    cout << "\n***********************\n";
//    cout << cur_E << " " << I.size() << endl;
//    cout << "\n***********************\n";
    long double cur_alpha = 2 * (double)cur_E / (I.size() * (I.size() - 1));
    if(cur_alpha > alpha && I.size() >= s) {
        alpha = cur_alpha;
        ans_size = I.size();
    }
}

int find_node_with_min_degree()
{
    int min_deg = 0, min_val = V + 1;
    for(int i:I)
        if(degree[i] < min_val){
            min_val = degree[i];
            min_deg = i;
        }
    return min_deg;
}

void remove_node(int u)
{
    for(int i = 0; i < G[u].size(); i++) {
        int v = G[u][i];
        auto uu = find(G[v].begin(), G[v].end(), u);
        if ( uu != G[v].end())
            G[v].erase(uu);
    }
    G[u].clear();
    I.erase(u);
}

void listing(int k, set<int> indices)
{
    if( k == 2){
        for(int i : indices)
            for(int j = 0; j < D[i].size(); j++) {
                if(indices.find(D[i][j]) != indices.end()) {
                    degree[i]++;
                    degree[D[i][j]]++;
                    for (int x:C)
                        degree[x]++;
                }
            }
    }
    else {
        for (int i : indices) {
            set<int> new_indices;
            for (int j = 0; j < D[i].size(); j++) {
                int nbr = D[i][j];
                if(indices.find(nbr) != indices.end())
                    new_indices.insert(nbr);
            }
            C.insert(i);
            listing(k - 1, new_indices);
            C.erase(i);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
//    cout << "\n***********************\n";
    input_graph();
    int graph_size = V;
    for(int i = 0; i < V; i++) {
        I.insert(i);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    while(I.size() >= s ) {
        create_Dgraph();
        listing(k, I);
        int min_deg = find_node_with_min_degree();
//        cout << "\n _______________________________ \n";
//        cout << "\n before:  " << min_deg << endl;
//        print_graph(G, V);
        remove_node(min_deg);
//        cout << "\n after:  " << min_deg << endl;
//        print_graph(G, V);
        for(int i = 0; i < V; i++)
            degree[i] = 0;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    cout << "Time: " << duration << "\nValue: " << fixed << setprecision(10) << alpha << "\nGraph Size: " << ans_size << endl;
}
