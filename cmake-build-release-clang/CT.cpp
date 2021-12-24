#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int n;

vector<vector<int>> dfs(vector<vector<int>> vec, int now, vector<vector<bool>> row, vector<vector<bool>> col,
						vector<vector<bool>> block) {
	if (now == n * n)
		return vec;
	int r = now / n, c = now % n, b = r / n * 3 + c / n;
	if (vec[r][c] != -1)
		dfs(vec, now + 1, row, col, block);
	for (int i = 0; i < n; i++) {
		if (row[r][i] || col[c][i] || block[b][i])
			continue;
		row[r][i] = true;
		col[c][i] = true;
		block[b][i] = true;
		dfs(vec, now + 1, row, col, block);
		row[r][i] = false;
		col[c][i] = false;
		block[b][i] = false;
	}
}

int main() {
	string sp;
	cin >> sp;
	n = sp[0] - '0';
	char q = sp[1];
	vector<int> mapping(128, '0');
	for (int i = 2; i < 2 + n; i++) {
		mapping[sp[i]] = i - 1;
	}

	vector<vector<int>> vec(10, vector<int>(10, 0));
	vector<vector<bool>> row(10, vector<bool>(10, false));
	vector<vector<bool>> col(10, vector<bool>(10, false));
	vector<vector<bool>> block(10, vector<bool>(10, false));

	for (int i = 0; i < n * n; i++) {
		string s;
		cin >> s;
		for (int j = 0; j < n * n; j++) {
			char c = s[j];
			if (c != q) {
				row[i][mapping[c]] = true;
				col[j][mapping[c]] = true;
				block[i / 3 * 3 + j / 3][mapping[c]] = true;
			} else
				vec[i][j] = -1;
		}
	}

	vector<vector<int>> ans = dfs(vec, 0, row, col, block);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << ans[i][j];
		}
		cout << '\n';
	}
}

// vector<vector<int>> adj;
// vector<int> parent;
// vector<bool> visit;
// vector<int> dist;
// int frame = 0;
// int v = 20;
// int e = 13;

// void bellman_ford(int u) {
// 	visit.resize(v, false);
// 	dist.resize(v, 1e9);
// 	parent.resize(v, -2);
// 	for (auto& x : adj) {
// 		for (auto& y : x) {
// 			if (y == 0)
// 				y = 1e9;
// 		}
// 	}

// 	for (int i = 0; i < v; i++)
// 		dist[i] = 1e9;

// 	dist[u] = 0;
// 	parent[u] = u;

// 	for (int i = 0; i < v - 1; i++) {
// 		for (int a = 0; a < v; ++a) {
// 			for (int b = 0; b < v; ++b) {
// 				if (dist[a] != 1e9 && adj[a][b] != 1e9) {
// 					if (dist[a] + adj[a][b] < dist[b]) {
// 						dist[b] = dist[a] + adj[a][b];
// 						parent[b] = a;
// 					}
// 				}
// 			}
// 		}
// 	}
// }

// int main() {
// 	int n, m, s;
// 	cin >> n >> m >> s;
// 	v = n;
// 	adj.resize(v);
// 	for (int i = 0; i < v; i++)
// 		adj[i].resize(v);
// 	for (int i = 0; i < m; i++) {
// 		int u, v, w;
// 		cin >> u >> v >> w;
// 		u--;
// 		v--;
// 		adj[u][v] = w;
// 		adj[v][u] = w;
// 	}
// 	vector<pair<int, int>> subways;
// 	for (int i = 0; i < s; i++) {
// 		int u, w;
// 		cin >> u >> w;
// 		u--;
// 		subways.push_back({u, w});
// 		// adj[0][u] = w;
// 		// adj[u][0] = w;
// 	}
// 	bellman_ford(0);
// 	int cnt = 0;
// 	for (int i = 0; i < n; i++) {
// 		cerr << dist[i] << "\n";
// 	}
// 	for (int i = 0; i < s; i++) {
// 		if (dist[subways[i].first] <= subways[i].second || dist[subways[i].first] == 1e9)
// 			cnt++;
// 	}
// 	cout << cnt << "\n";
// }

// #include <iostream>
// #include <random>
// #include <ctime>
// using namespace std;

// vector<vector<int>> adj;
// vector<int> parent;
// vector<bool> visit;
// vector<int> dist;
// int frame = 0;
// int v = 20;
// int e = 13;

// void bellman_ford(int u) {
// 	visit.resize(v, false);
// 	dist.resize(v, 1e9);
// 	parent.resize(v, -2);
// 	for (auto& x : adj) {
// 		for (auto& y : x) {
// 			if (y == 0)
// 				y = 1e9;
// 		}
// 	}
// 	std::cout << "save -f=./timeline/" << frame++ << ".gfn\n";

// 	for (int i = 0; i < v; i++)
// 		dist[i] = 1e9;

// 	dist[u] = 0;
// 	parent[u] = u;

// 	for (int i = 0; i < v - 1; i++) {
// 		for (int a = 0; a < v; ++a) {
// 			for (int b = 0; b < v; ++b) {
// 				if (dist[a] != 1e9 && adj[a][b] != 1e9) {
// 					if (dist[a] + adj[a][b] < dist[b]) {
// 						dist[b] = dist[a] + adj[a][b];
// 						std::cout << "setedgeprops -name=" << min(parent[b], b) << "," << max(parent[b], b)
// 								  << " -key=edgeColor -value=rgb(0,0,0)\n";
// 						std::cout << "setedgeprops -name=" << min(parent[b], b) << "," << max(parent[b], b)
// 								  << " -key=thickness -value=0.06\n";
// 						parent[b] = a;
// 						std::cout << "setedgeprops -name=" << min(a, b) << "," << max(a, b)
// 								  << " -key=edgeColor -value=rgb(255,0,0)\n";
// 						std::cout << "setedgeprops -name=" << min(a, b) << "," << max(a, b)
// 								  << " -key=thickness -value=0.4\n";
// 						std::cout << "save -f=./timeline/" << frame++ << ".gfn\n";
// 					}
// 				}
// 			}
// 		}
// 	}
// }

// int main() {
// 	std::cout << "echo -on\n";

// 	adj.resize(v);
// 	for (int i = 0; i < v; i++) {
// 		adj[i].resize(v);
// 		std::cout << "mkvertex -name=" << i << "\n";
// 		std::cout << "setvertexprops -name=" << i << " -key=label -value=" << i << "\n";
// 	}
// 	std::cout << "configs -c4=0.0\n";

// 	uniform_int_distribution<int> wDis(1, 100);
// 	mt19937 gen(time(0));
// 	for (int i = 1; i < v; i++) {
// 		uniform_int_distribution<int> dis(0, i);
// 		std::cout << "mkedge -uname=" << dis(gen) << " -vname=" << i << "\n";
// 	}
// 	std::cout << "configs -c1=1.0 -c2=1.105427 -c3=1.03 -c4=0.005\n";

// 	// std::cout << "setvertexprops -name=0 -key=fillColor -value=rgb(255,255,0)\n";
// 	// bellman_ford(0);
// 	std::cout << "exit\n";
// }