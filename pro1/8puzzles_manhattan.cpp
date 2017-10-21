#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <map>
#define puzzle_size 3

using namespace std;

struct Node {
  //记录父亲node
  Node* parent;
  //记录移动的步数
  int move;
  //记录这个状态中各行各列的数字, blank = 0
  vector<vector<int> > state;
  int blank;
  //default constructor
  Node() {
    move = 0, blank = puzzle_size - 1;
    parent = NULL;
    state.assign(3, vector<int>(3));
  }

  Node(vector<vector<int> > s, int m, Node* p) {
    state = s;
    move = m;
    parent = p;
  }

  bool isFinished() {
    return cal_misplace() == 0;
    // for (int i = 0; i < puzzle_size; i++) {
    //   for (int j = 0; j < puzzle_size; j++) {
    //     if (i == 2 && j == 2) break;
    //     if (state[i][j] != i * puzzle_size + j + 1)
    //       return false;
    //   }
    // }
    // return true;
  }

  bool isValid(int x, int y) {
    return x < 3 && x >= 0 && y < 3 && y >= 0;
  }


  vector<Node*> getNeighbors() {
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};
    int blankx = -1, blanky = -1;
    for (int i = 0; i < puzzle_size; i++)
      for (int j = 0; j < puzzle_size; j++)
        if (state[i][j] == 0) {
          blankx = i, blanky = j;
          break;
        }
    vector<Node*> neighbors;
    for (int i = 0; i < 4; i++) {
      vector<vector<int> > s = state;
      if (isValid(blankx + dx[i], blanky + dy[i])) {
        swap(s[blankx + dx[i]][blanky + dy[i]], s[blankx][blanky]);
        Node* newnode = new Node(s, move + 1, this);
        neighbors.push_back(newnode);
      }
    }
    return neighbors;
  }
  int cal_Manhattan() {
    int sum = 0;
    for (int i = 0; i < puzzle_size; i++)
      for (int j = 0; j < puzzle_size; j++)
        sum += abs(i - state[i][j]) + abs(j - state[i][j]);
    return sum;
  }
  int cal_misplace() {
    int cnt = 0;
    for (int i = 0; i < puzzle_size; i++) {
      for (int j = 0; j <puzzle_size; j++) {
        if (i == 2 && j == 2) break;
        if (state[i][j] != i * puzzle_size + j + 1)
          cnt++;
      }
    }
    return cnt;
  }
};

struct cmp {
	bool operator() (Node *a, Node *b) {
		return a->cal_misplace() > b->cal_misplace();
	}
};

void A_star(Node* n) {
  map<vector<vector<int> >, bool> visited;
  priority_queue<Node*, vector<Node*>, cmp> Q;
  Q.push(n);
  bool flag = false;
  while (!Q.empty()) {
    Node* cur = Q.top();
    visited[cur->state] = true;
    Q.pop();
    if (cur->isFinished()) {
      flag = true;
      cout << "Total Moves: " << cur->move << endl;
      return;
    }
    vector<Node*> neighbors = cur->getNeighbors();
    for (vector<Node*>::iterator it = neighbors.begin(); it != neighbors.end(); it++)
      if (!visited[(*it)->state])
        Q.push(*it);
  }
  if (!flag)
    cout << "Can't solve this puzzle.\n";
}

int main() {
  vector<vector<int> > s;
  Node* n = new Node();
  int state[puzzle_size * puzzle_size] = {6, 4, 7, 8, 5, 0, 3, 2, 1};
  //int state[puzzle_size * puzzle_size] = {8, 6, 7, 2, 5, 4, 3, 0, 1};
  for (int i = 0; i < puzzle_size; i++)
    for (int j = 0; j < puzzle_size; j++)
      n->state[i][j] = state[i * puzzle_size + j];
  A_star(n);
}
