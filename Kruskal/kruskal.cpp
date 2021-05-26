#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
using namespace std;


typedef pair <int,int> ii;
typedef vector <ii> vii;

vector<int> parent;
vector<int> k_rank;

struct graph
{
    int V;
    int E;
    vector <vii> adj_list;
    vector <pair<int, ii>> edgeList;
} typedef Graph;

void make_set(int x){
	parent[x] = x;
	k_rank[x] = 0;
}

//acha o parente do nó x
int find_set(int x){
	if(parent[x] != x){
		parent[x] = find_set(parent[x]);
	}
	return parent[x];
}

//união
void union_set(int x, int y){
	
	if(k_rank[x] >= k_rank[y]){
		parent[y] = x;
		if(k_rank[x] == k_rank[y]){
			k_rank[x]++;
		}
	}

	else{
		parent[x] = y;
	}
}

//retorna as arestas da árvore geradora mínima
vector <pair<int, ii>> kruskal(vector <vii> adj_list, vector <pair<int, ii>> edgeList, int m){

	vector <pair<int, ii>> T;

	for(int v = 0; v < adj_list.size(); v++){
		make_set(v);
	}
	for(auto uv : edgeList){
		if(find_set(uv.second.first) != find_set(uv.second.second)){
			T.push_back(uv);
			union_set(find_set(uv.second.first),find_set(uv.second.second));
		}
	}

	return T;

}

int main(){

    int u, v, w;

    Graph g;

    scanf("%d%d", &g.V, &g.E);

    g.adj_list.resize(g.V);
	parent.resize(g.V);
	k_rank.resize(g.V);

	for (int i = 0; i < g.E; ++i)
	{
	   scanf("%d%d%d", &u, &v, &w);
	   g.adj_list[u].push_back( ii(v, w) );
	   g.adj_list[v].push_back( ii(u, w) );
	   g.edgeList.push_back(make_pair(w, ii(u,v)));
	}

	sort(g.edgeList.begin(), g.edgeList.end());

	vector <pair<int, ii>> Tree = kruskal(g.adj_list, g.edgeList, g.E);

	for(int i = 0; i < Tree.size(); i++){
		printf("The edges of MST : (%d,%d)\n", Tree[i].second.first, Tree[i].second.second);
	}

	return 0;
}
