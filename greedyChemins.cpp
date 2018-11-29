#include <bits/stdc++.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#define pb push_back
//#define endl "\n"
#define PI ((double)M_PI)
#define INF 1e18

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define REP(i, n) FOR(i, 0, n)
#define IN(v,n) REP(i,n) cin >> v[i];
#define TRACE(x) if(DEBUG) cout << #x << " = " << (x) << endl
#define ALL(v) v.begin(), v.end()
#define SORT(v) sort(ALL(v))
#define REVERSE(v) reverse(ALL(v))
#define _ << " " <<

typedef int int_32;
#define int long long

using namespace std;

typedef long long ll;
typedef pair<int,int> pii;

template<typename T>
ostream& operator<< (ostream& os, const vector<T>& v);
template<typename T1, typename T2>
ostream& operator<< (ostream& os, const pair<T1,T2>& p) {
	os << '{' << p.first << ", " << p.second << '}';
	return os;
}
template<typename T>
ostream& operator<< (ostream& os, const vector<T>& v) {
	bool first = true; os << '{'; for(const T& e : v) {
		if(first) first = false; else os << ", "; os << e;
	} os << '}'; return os;
}

struct semicolon_is_space : std::ctype<char> {
	semicolon_is_space() : std::ctype<char>(get_table()) {}
	static mask const* get_table()
	{
		static mask rc[table_size];
		rc[';'] = std::ctype_base::space;
		rc['\n'] = std::ctype_base::space;
		return &rc[0];
	}
};

void semp(istream& is) {
	is.imbue(locale(cin.getloc(), new semicolon_is_space));
}

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos-prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	}
	while (pos < str.length() && prev < str.length());
	return tokens;
}

struct Node {
	int id = 0;
	double x;
	double y;
	bool root = false;
	bool connected = false;
	int maxDepth = 0;
	int parent = -1;
	vector<int>* dist;
};

ostream& operator<<(ostream& os, const Node& n) {
	os << "{" << n.id << ", "<< n.root << ", " << n.dist << "}";
	return os;
}

struct Solution {
	vector<Node> nodes;
	vector<vector<int>> loops;
	vector<vector<int>> paths;

	int score() {
		int ret = 0;
		for(vector<int>& l : loops) {
			REP(i,(int) l.size())
				ret += nodes[l[i]].dist->at(l[(i+1)%l.size()]);
		}
		for(vector<int>& l : paths) {
			REP(i,(int) l.size()-1)
				ret += nodes[l[i]].dist->at(l[i+1]);
		}
		return ret;
	}

	void cleanPaths() {
		paths.clear();
		for(Node& n : nodes) {
			if(n.root) {
				n.connected = true;
				n.maxDepth = 5;
			} else {
				n.connected = false;
				n.maxDepth = 0;
			}
			n.parent = -1;
		}
	}

	void generatePaths() {
		paths.clear();
		set<int> pathEnds;
		REP(i,(int)nodes.size()) pathEnds.insert(i);
		for(Node& n : nodes) {
			pathEnds.erase(n.parent);
		}
		for(int pathEnd : pathEnds) {
			if(nodes[pathEnd].root) continue;
			paths.resize(paths.size()+1);
			vector<int>& path = paths[paths.size()-1];
			int pivot = pathEnd;
			while(pivot != -1) {
				path.pb(pivot);
				pivot = nodes[pivot].parent;
			}
			REVERSE(path);
		}
	}

	void greedy() {
		cleanPaths();
		set<pair<int, pair<int,              int>>> distanceToInternet;
		//       dist      to be connected   root
		vector<set<pair<int,int>>> distanceToInternetForNode(nodes.size());
		// Initialization
		for(Node& n : nodes) if(!n.connected) { // Si on a besoin de connecter la node
			for(Node& n2 : nodes) if (n2.connected) { // On trouve les nodes connectées
				distanceToInternetForNode[n.id].insert({n2.dist->at(n.id), n2.id});
			}
			const pair<int,int>& bestDist = *distanceToInternetForNode[n.id].begin();
			distanceToInternet.insert({bestDist.first, {n.id, bestDist.second}});
		}
		// Iterate & update
		while(!distanceToInternet.empty()) {
			int dist, toConnect, root;
			dist = distanceToInternet.begin()->first;
			toConnect = distanceToInternet.begin()->second.first;
			root = distanceToInternet.begin()->second.second;
			assert(nodes[root].connected && nodes[root].maxDepth > 0 && !nodes[toConnect].connected);
			assert(!nodes[toConnect].root);
			assert(dist == nodes[root].dist->at(toConnect));
			// Let's connect
			distanceToInternet.erase(distanceToInternet.begin());
			distanceToInternetForNode[toConnect].clear();
			nodes[toConnect].connected = true;
			nodes[toConnect].maxDepth = nodes[root].maxDepth - 1;
			nodes[toConnect].parent = root;
			if(!nodes[root].root) {
				nodes[root].maxDepth = 0;
				// TODO on ne peut plus connecter d'autres nodes par cette node
				REP(i, (int)nodes.size()) if(!nodes[i].connected) { // Nodes to update
					auto bestDist = distanceToInternetForNode[i].begin();
					pair<int, pair<int,int>> oldBest = {bestDist->first, {i, bestDist->second}};
					distanceToInternetForNode[i].erase({nodes[root].dist->at(i), root});
					bestDist = distanceToInternetForNode[i].begin();
					pair<int, pair<int,int>> newBest = {bestDist->first, {i, bestDist->second}};
					if(oldBest != newBest) {
						distanceToInternet.erase(oldBest);
						distanceToInternet.insert(newBest);
					}
				}
			}
			if(nodes[toConnect].maxDepth > 0) {
				// TODO on peut connecter d'autres nodes par cette node
				REP(i, (int)nodes.size()) if(!nodes[i].connected) { // Nodes to update
					auto bestDist = distanceToInternetForNode[i].begin();
					pair<int, pair<int,int>> oldBest = {bestDist->first, {i, bestDist->second}};
					distanceToInternetForNode[i].insert({nodes[toConnect].dist->at(i), toConnect});
					bestDist = distanceToInternetForNode[i].begin();
					pair<int, pair<int,int>> newBest = {bestDist->first, {i, bestDist->second}};
					if(oldBest != newBest) {
						distanceToInternet.erase(oldBest);
						distanceToInternet.insert(newBest);
					}
				}
			}
		}
		// There should be nothing left to connect
		for(Node& n : nodes) {
			bool ok = true;
			if(!n.connected) {
				cerr << n << " not connected." << endl;
				ok = false;
			}
			assert(ok);
		}

		generatePaths();
	}

	void output(ostream& os) {
		for(vector<int>& l : loops) {
			os << "b ";
			int n = l.size();
			REP(i, n) {
				os << l[i] << " \n"[i==n-1];
			}
		}
		for(vector<int>& l : paths) {
			os << "c ";
			int n = l.size();
			REP(i, n) {
				os << l[i] << " \n"[i==n-1];
			}
		}
	}

	void simulatedAnnealing() {
		default_random_engine generator;
		uniform_int_distribution<int> pickLoopD(0,loops.size()-1);
		auto pickLoop = bind(pickLoopD, generator);

		int alpha = 0.9999;
		int T = 300;
		for(int noMoveTimer = 10000; noMoveTimer > 0; noMoveTimer--) {
			
		}
	}
};


int_32 main(int_32 argc, char** argv) {
	//ios_base::sync_with_stdio(0);

	if(argc != 2) {
		cout << "wrong usage" << endl;
		return 0;
	}
	string folder = argv[1];

	// Load input
	ifstream distances(folder+"/distances.csv");
	ifstream nodes(folder+"/nodes.csv");
	assert(distances);
	assert(nodes);
	semp(nodes);
	string drop;
	getline(nodes, drop);

	Solution solution;
	vector<Node>& allNodes = solution.nodes;
	Node cn;
	string type;
	while(nodes >> cn.x >> cn.y >> type) {
		cn.root = type[0] == 'd';
		allNodes.pb(cn);
		cn.id++;
	}
	vector<vector<int>> dists(allNodes.size());
	for(Node& n : allNodes) {
		n.dist = &dists[n.id];
		n.dist->resize(allNodes.size());
		REP(i, (int)allNodes.size()) distances >> n.dist->at(i);
	}

	/*for(Node& n : allNodes)
		cout << n << endl;*/

	// Load output
	ifstream loops(folder+"/loops.out");
	assert(loops);
	string line;
	while(getline(loops, line)) {
		if(line.length() == 0) break;
		vector<string> splitted = split(line, " ");
		if(splitted.size() < 2) break;
		if (splitted[0][0] == 'b') {
			// C'est une boucle !
			assert(splitted.size() <= 31);
			bool atLeastOneRoot = false;
			solution.loops.resize(solution.loops.size()+1);
			FOR(i, 1, (int)splitted.size()) {
				int id = stoi(splitted[i]);
				if(allNodes[id].root) atLeastOneRoot = true;
				allNodes[id].root = true;
				allNodes[id].connected = true;
				allNodes[id].maxDepth = 5;
				solution.loops[solution.loops.size()-1].pb(id);
			}
			assert(atLeastOneRoot);
		} else if (splitted[0][0] == 'c') {
			// C'est un chemin
			assert(splitted.size() <= 7);
			assert(false); // Not implemented
		} else assert(false);
	}

	solution.greedy();

	ofstream out(folder+".out");
	solution.output(out);
	TRACE(solution.score());

	return 0;
}
