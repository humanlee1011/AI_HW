/*
  生成大量的啊皇后问题，并用一下算法分别求解
  1. 爬山法（最陡上升发和首选爬山法）
  2. 随机重启爬山法
  3. 模拟退火算法
  计算search cost 和 问题的解决率

  评估函数h：相互攻击的皇后对数
  author：15331169lixiaoyun
  last modified time： 2017/10/23
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>

using namespace std;

int totalCost;
int state[8]; //每行中皇后所在位置[0,7]

void initial() {
  memset(state, -1, sizeof(state));
  for (int i = 0; i < 8; i++) {
    state[i] = rand() % 8;
  }
}

bool isValid(int x, int y) {
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}

int cal_collision() {
  int cnt = 0;
  for (int i = 0; i < 8; i++) {
    int col = state[i];
    for (int j = i + 1; j < 8; j++) {
      if (col == state[j]) cnt++;
      if ((i + col) == (j + state[j]))  cnt++;
      if ((i - col) == (j - state[j]))  cnt++;
    }
  }
  return cnt;
}

bool isFinished() {
  return cal_collision() == 0;
}

bool hillClimbing_steepestAscent() {
  for (int trial = 0; trial < 100; trial++) {
    int cur = cal_collision();
    int min = 99999, minX = 0, minY = 0;
    for (int i = 0; i < 8; i++) {
      int curstate = state[i];
      for (int j = 0; j < 8; j++) {
        if (j == curstate) break;
        //移动j
        state[i] = j;
        int nextCollision = cal_collision();
        if (nextCollision < min) {
          min = nextCollision;
          minX = i, minY = j;
        }
        //复原
        state[i] = curstate;
      }
    }
    if (min < cur) {
      state[minX] = minY;
    }
    if (isFinished()) {
      totalCost += trial;
      return true;
    }
  }
  return false;
}

bool hillClimbing_firstChoice() {
  for (int i = 0; i < 100; i++) {
    bool next = false;
    int curCollision = cal_collision();
    int times = 0;
    while (!next) {
      //为防止死循环，设置最大循环次数
      if (times++ > 1000) break;
      // 随机地生成后继节点直到生辰给一个优于当前结点的后继
      int r = rand() % 8;
      int c = rand() % 8;
      if (state[r] != c) {
        int curstate = state[r];
        state[r] = c;
        int nextCollision = cal_collision();
        if (nextCollision < curCollision)
          next = true;
        else//还原现场.
          state[r] = curstate;
      }
    }
    if (isFinished()) {
      totalCost += i;
      return true;
    }
  }
  return false;
}

bool hillClimbing_randomRestart() {
  // 多次尝试
  while(!hillClimbing_steepestAscent()) {
    initial();
  }
  return true;
}

bool simulatedAnnealing() {
  //模拟退火搜索
  int steps = 0;
  //初定温度50度
  double T = 5;
  while (T > 0.0001) {//降温
    int curCollision = cal_collision();
    int r = rand() % 8;
    int c = rand() % 8;
    int curstate = state[r];
    if (curstate != c) {
      steps++;
      state[r] = c;
      int nextCollision = cal_collision();
      int deltaE = curCollision - nextCollision;
      if (deltaE > 0)
        continue;
      else if (rand() % 100 > exp(deltaE / (double)T) * 100) { //还原
        state[r] = curstate;//复原
        T *= 0.99;
      }
      else
        T *= 0.99;
    }
    if (isFinished()) {
      totalCost += steps;
      return true;
    }
  }
  return false;
}

int hillClimbing(bool (*climbingFunc)(), int num) {
  int cnt = 0;
  for (int i = 0; i < num; i++) {
    initial();
    if (climbingFunc())
      cnt++;
  }
  return cnt;
}

void print(char* str, int cnt, int num) {
  cout << "Hill climbing by " << str << ": \n";
  cout << "resolution rate(success / total): " << cnt << " / " << num << endl;
  cout.precision(3);
  double avg = cnt?totalCost / (double)cnt:0;
  cout << "average steps: " << avg << endl;
}

int main() {
  //cout << cal_collision() << endl;
  srand((int)time(0));
  cout << "Random generating 100 instances to test hill climbing completing by steepest ascent.\n";
  int num = 1000;

  totalCost = 0;
  int cnt = hillClimbing(hillClimbing_steepestAscent, num);
  print((char*)"steepest ascent", cnt, num);

  totalCost = 0;
  cnt = hillClimbing(hillClimbing_firstChoice, num);
  print((char*)"first choice", cnt, num);

  totalCost = 0;
  cnt = hillClimbing(hillClimbing_randomRestart, num);
  print((char*)"first choice", cnt, num);

  totalCost = 0;
  cnt = hillClimbing(simulatedAnnealing, num);
  print((char*)"simulated annealing", cnt, num);
  return 0;
}
