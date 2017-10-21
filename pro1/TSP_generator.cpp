#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <unordered_map>

#define INF 0xFFFFFFF

using namespace std;

unordered_map<int, double> g_value;
double h_value;
double distances[100][100];
struct Location {
  double x, y;
  int idx;
  Location(double i = 0, double j = 0, int idx = 0) {
    x = i, y = j;
    idx = idx;
  }
};

template<typename T, typename priority_t>
struct priorityQueue {
  typedef pair<priority_t, T> PQElement;
  priority_queue<PQElement, vector<PQElement>,
                 std::greater<PQElement> > elements;

  inline bool empty() const { return elements.empty(); }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  inline T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

double cal_distances(Location& src, Location& dest) {
  return pow(pow(abs(src.x - dest.x), 2) + pow(abs(src.y - dest.y), 2), 0.5);
}

double cal_cost(Location& src, Location& dest) {
  return distances[src.idx][dest.idx];
}

// construct a graph of n citys
vector<Location> TSP_instances_generator(int n) {
  srand((int)time(0));
  vector<Location> graph;
  for (int i = 0; i < n; i++)
    graph.push_back(Location(random(), random()), i);
}

double cal_h_MST(vector<Location>& graph) {
  double cost = 0;
  bool visited[graph.size()];
  double dist[graph.size()];
  //calculate all distancess between citys on graph
  for (int i = 0; i < graph.size(); i++)
    for (int j = i + 1; j < graph.size(); j++)
      distances[j][i] = distances[i][j] = cal_distances(graph[i], graph[j]);

  memset(visited, false, sizeof(visited));
  memset(dist, INF, sizeof(dist));
  for (int i = 0; i < graph.size(); i++)
    dist[i] = distances[0][i];
  for (int i = 1; i < graph.size(); i++) {
    int v;
    double min = INF;
    for (int j = 0; j < graph.size(); j++) {
      if (!visited[j] && min > dist[j]) {
        min = dist[j];
        v = j;
      }
    }
    if (min < INF) {
      visited[v] = true;
      cost += min;
      for (int j = 0; j < graph.size(); j++) {
        if (!visited[j] && dist[j] > distances[j][v]) {
          dist[j] = distances[v][j];
        }
      }
    }
  }
  return cost;
}



void A_star(vector<Location>& graph, Location& src, Location& dest, int n) {
  priorityQueue<Location, double> openset;
  unordered_map<int, int> came_from;

  bool flag = false;
  openset.put(src, cal_h_MST(src));
  came_from[src.idx] = src.idx;
  while (!openset.empty()) {
    Location city = openset.get();
    if (city.idx == dest.idx && flag)
      break;
    flag = true;
    for (int i = 0; i < n; i++) {
      cur = graph[i];
      double cost = g_value[i] + cal_cost(cur, city);
      if (!g_value.count(cur) || cost < g_value[i]) {
        g_value[i] = cost;
        double h_value = cal_h_MST(cur);
        double f_value = cost + h_value;
        openset.put(cur, f_value);
        came_from[cur.idx] = city.idx;
      }
    }
  }
}

int main() {
  vector<Location> graph = TSP_instances_generator(10);
  cal_h_MST(graph);
  A_star(graph, graph[1], graph[1]);
}
