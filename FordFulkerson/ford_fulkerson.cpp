#include <iostream>
#include <bits/stdc++.h>

using namespace std;

typedef vector <int> vi;
typedef vector <vi> vvi;

struct graph
{
    int V;
    int E;
    vvi adj_list;
} typedef Graph;

/*Retorna true se tem um caminho de begin para end no grafo residual, também preenche parent[] com o caminho */
bool bfs(Graph g, vvi residual, int begin, int end, int parent[])
{
    bool visited[g.V];
    memset(visited, false, sizeof(bool) * g.V);
    visited[begin] = true;
    queue<int> q;
    q.push(begin);

    int dequeued;

    while(!q.empty()){
        dequeued = q.front();
        q.pop();

        for(int i = 0; i < g.V; i++){
            if(residual[dequeued][i] > 0 && !visited[i]){
                parent[i] = dequeued;
                visited[i] = true;
                q.push(i);
            }
        }
    }

    return visited[end];
}

//retorna o fluxo maximo de begin até end em um grafo g
int ford_fulkerson(Graph g, int begin, int end)
{
    int u, v, max_flow = 0;
    int *parent = new int[g.V];

    vvi residual;
    residual.resize(g.V);

    for(int i = 0; i < g.V; i++){
        residual[i].resize(g.V);
        for(int j = 0; j < g.V; j++){
            residual[i][j] = g.adj_list[i][j]; 
        }
    }

    while(bfs(g, residual, begin, end,parent)){
        int path = INT_MAX;

        for ( v = end; v != begin; v = parent[v]){
			u = parent[v];
			path = (residual[u][v] < path)? residual[u][v] : path;
		}

		for( v = end; v != begin; v = parent[v]){
			u = parent[v];
			residual[u][v] -= path;
			residual[v][u] += path;
		}
        max_flow += path;

    }

    return max_flow;
}

int main(){
    Graph g;
    int vertex1, vertex2, weight, begin, end;

    scanf("%d%d", &g.V, &g.E);
    scanf("%d%d", &begin, &end);

    g.adj_list.resize(g.V);

    for (int i = 0; i < g.V; i++){
        g.adj_list[i].resize(g.V);
    }

    for (int i = 0; i < g.E; i++){
        scanf("%d%d%d", &vertex1, &vertex2, &weight);

        g.adj_list[vertex1][vertex2] = weight;
    }

    printf("The maximum possible flow is: %d\n", ford_fulkerson(g, begin, end));

    return 0;
}
