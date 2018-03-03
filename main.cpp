#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>

using namespace std;

class MyGraph {
private:
    struct Edge{
        int x, y, state = 0;
        bool isWall = false;
        Edge(int _x, int _y, bool _isWall, int _radius){
            x = _x;
            y = _y;
            isWall = _isWall;
        }

        friend bool operator ==(const Edge &a, const Edge &b){
            return (a.x == b.x && a.y == b.y && a.isWall == b.isWall);
        }
    };

    double getDistance(Edge& e1, Edge& e2){
        long long lx = ((long long)e2.x - (long long)e1.x)*((long long)e2.x - (long long)e1.x);
        long long ly = (e1.isWall || e2.isWall) ? 0 : ((long long)e2.y - (long long)e1.y)*((long long)e2.y - (long long)e1.y);
        int penalty = (e2.isWall || e1.isWall) ?  (((e1.isWall && !e2.isWall) || (!e1.isWall && e2.isWall)) ? radius : 0) : radius*2;
        return sqrt(lx + ly) - penalty;
    }

    vector<vector<double>> distances;
    vector<Edge> edges;
    int radius = 0;

public:
    MyGraph(int r){
        radius = r;
    }

    void addEdge(int x, int y, bool isWall){
        edges.emplace_back(Edge(x, y, isWall, isWall ? 0 : radius));
    }

    bool solve(double start){
        bool result = BFS(start);
        for(Edge& e : edges)
            e.state = 0;
        return result;
    }

    bool BFS(double d){
        vector<int> oldWave;
        vector<int> newWave;
        oldWave.emplace_back(0);
        while (!oldWave.empty()){
            for(int i : oldWave)
                edges[i].state = 1;
            for(int j : oldWave) {
                for (int i = 1; i < edges.size(); i++) {
                    if(edges[i].state != 1) {
                        double dist = getDistance(edges[j], edges[i]);
                        if (dist <= d && !(edges[j].isWall && edges[i].isWall)) {
                            newWave.emplace_back(i);
                        }
                    }
                }
            }
            for(int i : newWave)
                if(edges[i] == edges[edges.size() - 1])
                    return true;
            oldWave = newWave;
            newWave.clear();
        }
        return false;
    }

    unsigned int size(){
        return edges.size();
    }
};

double binSearch(MyGraph &g, double maxRad){
    if(g.solve(0)) return 0;
    double left = 0, right = maxRad, mid = 0;
    while (right - left > 0.000000001) {
        mid = left + (right - left) / 2;

        if (!g.solve(mid))
            left = mid;
        else
            right = mid;
    }
    return left;
}

int main() {
    int xl, xr;
    cin >> xl >> xr;
    int r, n;
    cin >> r >> n;
    MyGraph graph(r);
    graph.addEdge(xl, 0, true);
    for(int i = 0; i < n; i++){
        int tmpA, tmpB;
        cin >> tmpA >> tmpB;
        if(tmpA > xl - r && tmpB < xr + r)
            graph.addEdge(tmpA, tmpB, false);
    }
    graph.addEdge(xr, 0, true);
    cout.precision(3);
    double alternativeAnswer = xr - xl;
    cout << fixed << ((graph.size() > 2) ? binSearch(graph, xr - xl) : alternativeAnswer);
    return 0;
}