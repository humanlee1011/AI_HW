/*
  Author: @15331169lixiaoyun
  datetime: 2017/10/09
  description: 跟踪A*搜索从Bucharest到Lugoj的操作，并使用直线距离作为估算因子。
    请展示出该算法中可能考虑到的点以及每个点中的f，g，h值。
*/
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <unordered_set>
#include <unordered_map>
#define INF 0xFFFFFFF

usign namespace std;

struct {
  vector<unordered_set<string> > neighbors;
  unordered_map<string, int> id;
  double dist[100][100];
  double cost(string& src, string& dest) {
    return dist[id[src]][id[dest]];
  }
} Graph;

template<typename T, typename priority_t>
struct priorityQueue {
  typedef pair<priority_t, T> PQElement;
  priority_queue<PQElement, vector<PQElement>,
                 std::greater<PQElement>> elements;

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

priorityQueue<string, double> openset;
unordered_map<string, double> g_value;
vector<string> closeset;
map<string, int> signs;
unordered_map<string, string> came_from;
double distance[100][100];
void A_star(string& src, string& dest);
double cal_h(string& src, string& dest);
void make_graph();

double cal_h(string& src) {
  //straight line distance
  return dist[signs[src]];
}

void make_graph() {
  
}

void A_star(Graph& graph, string& src, string& dest) {
  string node;
  int rnd = 0;
  came_from[src] = src;
  openset.put(src, cal_h(src, dest));
  while (!openset.empty()) {
    cout << "#" << rnd << endl;
    node = openset.get();
    if (node == dest)
      break;
    for (auto next: graph.neighbors[graph.id[node]]) {
      double cost = g_value[node] + graph.cost(node, dest);
      if (!g_value.count(next) || cost < g_value[next]) {
        g_value[next] = cost;//update the cost
        h_value = cal(node, dest);
        double f_value = cost + h_value;
        openset.put(next, f_value);
        cout << "\tNode: " << node << "\n\tf value: " << f_value
            << "\n\tg value: " << cost << "\n\th value:" << h_value << endl;
        came_from[next] = node;
      }
    }
  }
  cout << "The best path: ";
  string cur = dest;
  while (cur != src) {
    cout << came_from[cur] << " ";
  }
  cout << "The total cost is : " << f_value[dest] << endl;
  cout << "Done.\n";
}
