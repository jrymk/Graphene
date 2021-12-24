#include <iostream>
#include <random>
#include <ctime>
using namespace std;

vector<vector<int>> adj;
vector<int> parent;
vector<bool> visit;
vector<int> dist;
int frame = 0;
int v = 10;
int e = 11;

void bellman_ford(int u) {
	visit.resize(v, false);
	dist.resize(v, 1e9);
	parent.resize(v, -2);
	for (auto& x : adj) {
		for (auto& y : x) {
			if (y == 0)
				y = 1e9;
		}
	}
	std::cout << "save -f=./timeline/" << frame++ << ".gfn\n";

	for (int i = 0; i < v; i++)
		dist[i] = 1e9;

	dist[u] = 0;
	parent[u] = u;

	for (int i = 0; i < v - 1; i++) {
		for (int a = 0; a < v; ++a) {
			for (int b = 0; b < v; ++b) {
				if (dist[a] != 1e9 && adj[a][b] != 1e9 && dist[a] + adj[a][b] < dist[b]) {
					dist[b] = dist[a] + adj[a][b];
					std::cout << "setedgeprops -name=" << min(parent[b], b) << "," << max(parent[b], b)
							  << " -key=edgeColor -value=rgb(0,0,0)\n";
					std::cout << "setedgeprops -name=" << min(parent[b], b) << "," << max(parent[b], b)
							  << " -key=thickness -value=0.06\n";
					// 設定非最短路徑邊為黑色，粗細為預設0.06
					parent[b] = a;
					std::cout << "setedgeprops -name=" << min(a, b) << "," << max(a, b)
							  << " -key=edgeColor -value=rgb(255,0,0)\n";
					std::cout << "setedgeprops -name=" << min(a, b) << "," << max(a, b)
							  << " -key=thickness -value=0.2\n";
					// 設定最短路徑邊為紅色，粗細為加粗0.2
					std::cout << "timeline -new\n";
					// 在時間軸中加入一幀
				}
			}
		}
	}
}

int main() {
	std::cout << "echo -off\n";

	adj.resize(v);
	for (int i = 0; i < v; i++) {
		adj[i].resize(v);
		std::cout << "mkvertex -name=" << i << "\n";
		std::cout << "setvertexprops -name=" << i << " -key=label -value=" << i << "\n";
	}

	uniform_int_distribution<int> dis(0, v - 1);
	uniform_int_distribution<int> wDis(1, 100);
	mt19937 gen(time(0));
	for (int i = 0; i < e;) {
		int x = dis(gen), y = dis(gen);
		if (x > y)
			swap(x, y);
		if (adj[x][y] == 0 && adj[y][x] == 0) {
			int w = wDis(gen);
			adj[x][y] = w;
			adj[y][x] = w;
			std::cout << "mkedge -uname=" << x << " -vname=" << y << " -name=" << x << "," << y << "\n";
			std::cout << "setedgeprops -name=" << x << "," << y << " -key=label -value=" << w << "\n";
			i++;
		}
	}

	std::cout << "setvertexprops -name=0 -key=fillColor -value=rgb(255,255,0)\n";
	bellman_ford(0);
	std::cout << "exit\n";
}