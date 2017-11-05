/*
  生成大量的八数码问题，并用一下算法分别求解
  1. 爬山法（最陡上升发和首选爬山法）
  2. 随机重启爬山法
  3. 模拟退火算法
  计算search cost 和 问题的解决率

  author：15331169lixiaoyun
  last modified time： 2017/10/22
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cstring>

using namespace std;



struct Node {
  vector<vector<int> > state;
  int cost;
  Node* parent;
  //default constructor
  Node() {
    cost = 0;
    parent = NULL;
    for (int i = 0; i < 3; i++) {
      vector<int> s;
      for (int j = 0; j < 3; j++)
        s.push_back(i * 3 + j + 1);
      state.push_back(s);
    }
  }

  Node(vector<vector<int> >& s, int c, Node* p) {
    state = s;
    cost = c;
    p = parent;
  }

  bool isFinished() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (i == 2 && j == 2) break;
        if (state[i][j] != i * 3 + j + 1)
          return false;
      }
    }
    return true;
  }


  int cal_Manhattan() {
    int sum = 0;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++) {
        int row = state[i][j] / 3;
        int col = state[i][j] % 3;
        sum += abs(i - row) + abs(j - col);
      }
    return sum;
  }

  bool isValid(int r, int c) {
    return r >= 0 && r < 3 && c >= 0 && c < 3;
  }

  bool canSolve() {
    int cnt = 0;
    for (int i = 8; i >= 0; i--)
      for (int j = i - 1; j >= 0; j--)
        if (state[i / 3][i % 3] < state[j / 3][j % 3])
          cnt++;
    return cnt % 2 == 0;
  }

  void shuffle() {
    int i = 0;
    int s[9];
    memset(s, -1, sizeof(s));
    while (i < 9) {
      int pos = rand() % 9;
    //cout << pos << endl;
      if (s[pos] == -1)
        s[pos] = i++;
    }
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        state[i][j] = s[i * 3 + j];
  }

  vector<Node*> getNeighbors() {

    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};
    int blankx = 0, blanky = 0;
    // find blank's position
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        if (state[i][j] == 0)
          blankx = i, blanky = j;

    vector<Node*> neighbors;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (isValid(blankx + dx[i], blanky + dy[j])) {
          vector<vector<int> > s = state;
          swap(s[blankx + dx[i]][blanky + dy[j]], s[blankx][blanky]);
          Node* newnode = new Node(s, cost + 1, this);
          neighbors.push_back(newnode);
        }
      }
    }
    return neighbors;
  }
  int cal_h() {
    return cal_Manhattan();
  }
};


Node puzzlesGenerator() {
  while (1) {
    Node* node = new Node();
    node->shuffle();
    if (node->canSolve())
      return *node;
  }
}

/* solution1: 爬山法，最陡上升法*/
int* hillClimbing_steepestAscent(Node* root) {
  int *ans = new int(2);
  Node* cur = root;
  while (cur != NULL) {
    if (cur->isFinished()) {
      ans[0] = cur->cost;
      ans[1] = 1;//solved
      cout << "cost: " << cur->cost << " Finished.\n";
      return ans;
    }
    vector<Node*> neighbors = cur->getNeighbors();
    Node* next = NULL;
    int min = 1000000;
    for (auto neigh: neighbors){
      int misplace = neigh->cal_Manhattan();
      if (min > misplace) {
        next = neigh;
        min = misplace;
      }
    }
    if (min >= cur->cal_Manhattan())
      break;
    cur = next;
  }
  cout << "cost: " << cur->cost << " Unfinished.\n";
  ans[0] = cur->cost;
  ans[1] = 0;//remain unsolved
  return ans;
}


int main() {
  srand((int)time(NULL));
  cout << "Random generating 100 instances to test hill climbing completing by steepest ascent.\n";
  int num = 100;
  int solvedCnt = 0;
  int solvedCost = 0, unsolvedCost = 0;
  for (int i = 0; i < num; i++) {
    cout << "#" << i + 1 << " test: \n";
    Node temp = puzzlesGenerator();
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        cout << temp.state[i][j] << " ";
    cout << endl;
    int *testAns;
    testAns = hillClimbing_steepestAscent(&temp);
    if (testAns[1]) {
      solvedCost += testAns[0];
      solvedCnt++;
    }
    else
      unsolvedCost += testAns[0];
  }
  cout << "Test done!\nThe result is:\n";
  cout << "resolution rate: " << solvedCnt * 0.01 << endl;
  cout << "average solved cost: " << solvedCost * 0.01 << endl;
  cout << "average unsolved cost: " << unsolvedCost * 0.01 << endl;
  return 0;
}
