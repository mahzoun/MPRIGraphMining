#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
using namespace std;
const int MAXN = 1e6; //max possible |V(G)|
long long V, E; // size of input graph
vector<int> G[MAXN], G_help[MAXN], D[MAXN]; //input graph D is directed graph
int mio[MAXN], degree[MAXN];
map <pair<int,int>, bool> is_neigh;
int k, s;
bool removed[MAXN];
bool graph_degree_comp(vector<int> i ,vector<int> j) { return ( i.size() > j.size() ); }

void input_graph()
{
    ifstream fin("dolphins.in");
    fin >> k >> s;
    fin >> V >> E;
    int tmp_size = E;
    for(int i = 0; i < V; i++)
        G[i].push_back(i);
    while(tmp_size--){
        int u, v;
        fin >> u >> v;
        u--;--v;
        G[u].push_back(v);
        G[v].push_back(u);
        G_help[u].push_back(v);
        G_help[v].push_back(u);

    }
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

void core_ordering(vector<int> G[])
{
    for(int i = 0; i < V; i++){
        mio[i] = G[i].size();
    }
}


void create_Dgraph(vector<int> G[], set<int> indices)//TODO fix this, it creates Dgraph for the whole graph
{
    for(int i = 0 ; i < V ; i++)
        D[i].clear();
    for(int i: indices){
        for(int j = 0; j < G[i].size(); j++){
            if(mio[i] < mio[G[i][j]])
                D[i].push_back(G[i][j]);
        }
    }
}

void listing(int k, set<int> indices, set<int> C)
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
            listing(k - 1, new_indices, C);
            C.erase(i);
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    input_graph();
    int graph_size = V;
    set<int> I;
    long double alpha = E * 2 / (V * (V - 1));
    for(int i = 0; i < V; i++) {
        I.insert(i);
    }
    set<int> C;
    core_ordering(G_help);
    while(graph_size > s) {
        for(int i = 0; i < V; i++)
            degree[i] = 0;
        create_Dgraph(G_help, I);
        listing(k, I, C);
//        for(int i = 0 ; i < V; i++)
//            cout<<degree[i] << " ";
//        cout << endl;
//        print_graph(G_help, V);
        int min_deg = 0, min_val = V + 1;
        for(int i:I)
            if(removed[i] == 0 & degree[i] < min_val){
                min_val = degree[i];
                min_deg = i;
        }
        I.erase(min_deg);
        removed[min_deg] = 1;
        //remove node with min_deg and calculate alpha
        cout << "before earse node " << min_deg << " " << degree[min_deg] << endl;
//        print_graph(G_help, V);
        for(int i = 0; i < G_help[min_deg].size(); i++){
            int nbr = G_help[min_deg][i];
            G_help[nbr].erase(find(G_help[nbr].begin(), G_help[nbr].end(), min_deg));
        }
        G_help[min_deg].erase(G_help[min_deg].begin(), G_help[min_deg].end());
//        cout << "after earse node " << min_deg << endl;
//        print_graph(G_help, V);
        graph_size--;
        long long cur_E = 0;
        for(int i : I)
            cur_E += G_help[i].size();
        cout << cur_E << " " << graph_size << endl;
        long double cur_alpha = (double)cur_E / (graph_size * (graph_size - 1));
        if(cur_alpha > alpha) {
            cout << endl << "------>" << cur_alpha << endl;
            print_graph(G_help, V);
            alpha = cur_alpha;
        }
    }
    cout << fixed << setprecision(10) << alpha << endl;
}
