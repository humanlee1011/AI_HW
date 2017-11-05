/*
  生成大量的八数码问题，并用一下算法分别求解
  1. 爬山法（最陡上升发和首选爬山法）
  2. 随机重启爬山法
  3. 模拟退火算法
  计算search cost 和 问题的解决率

  评估函数：现状态和最终状态的曼哈顿距离之和

  author：15331169lixiaoyun
  last modified time： 2017/10/23
*/

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

int blankx, blanky;
int totalCost = 0;
int direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // 右下左上
int state[3][3];  // 当前状态


void initial() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      state[i][j] = i * 3 + j;
    }
  }
  blankx = blanky = 0 ;
  //shuffle 8 puzzles

  int last = -1;   // 上一次移动方向
  for (int i = 0; i < 20; i++) {  // 随机打乱
      bool upset = false;
      while (!upset) {  // 打乱成功才跳出循环
          int dir = rand() % 4;  // 随机选取一个方向
          if (last != -1 && last != dir && abs(last-dir) == 2)  continue;  // 避免反向走
          int x = blankx + direction[dir][0];
          int y = blanky + direction[dir][1];
          if (x >= 0 && x < 3 && y >= 0 && y < 3) {  // 方向可行
              swap(state[blankx][blanky], state[x][y]);  // 交换0和相邻数字的位置
              blankx = x, blanky = y;     // 更新0的坐标
              last = dir;               // 更新此次移动方向
              upset = true;             // 标记打乱成功
          }
      }
  }
}

int cal_Manhattan() {
  int sum = 0;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      if (state[i][j] == 0) continue;
      int row = state[i][j] / 3;
      int col = state[i][j] % 3;

      sum += abs(i - row) + abs(j - col);
      //sum += s;
    }
  return sum;
}

bool isValid(int x, int y) {
  return x >= 0 && x < 3 && y >= 0 && y < 3;
}

bool isFinished() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 2 && j == 2) break;
      if (state[i][j] != i * 3 + j)
        return false;
    }
  }
  return true;
}

bool hillClimbing_steepestAscent() {
  for (int i = 0; i < 100; i++) {
    int cur = cal_Manhattan();
    int min = 99999, minX = 0, minY = 0;
    for (int j = 0; j < 4; j++) {
      int x = blankx + direction[j][0];
      int y = blanky + direction[j][1];
      if (isValid(x, y)) {
        swap(state[blankx][blanky], state[x][y]);
        int curManhattan = cal_Manhattan();
        if (curManhattan < min) {
          minX = x, minY = y;
          min = curManhattan;
        }
        swap(state[blankx][blanky], state[x][y]);
      }
    }
    if (min < cur) {
      swap(state[blankx][blanky], state[minX][minY]);
      blankx = minX, blanky = minY;
    }
    if (isFinished()) {
      totalCost += i;
      return true;
    }
  }
  return false;
}

bool hillClimbing_firstChoice() {
  for (int i = 0; i < 100; i++) {
    bool next = false;
    int curManhattan = cal_Manhattan();
    int times = 0;
    while (!next) {
      //为防止死循环，设置最大循环次数
      if (times++ > 20) break;
      // 随机地生成后继节点直到生辰给一个优于当前结点的后继
      int dir = rand() % 4;
      int x = blankx + direction[dir][0];
      int y = blanky + direction[dir][1];
      if (isValid(x, y)) {
        swap(state[x][y], state[blankx][blanky]);
        int nextManhattan = cal_Manhattan();
        if (nextManhattan < curManhattan) {
          blankx = x, blanky = y;
          next = true;
        }
        else
          swap(state[x][y], state[blankx][blanky]);
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
  //初定温度5度
  double T = 5;
  while (T--) {//降温
    int dir = rand() % 4;
    int x = blankx + direction[dir][0];
    int y = blanky + direction[dir][1];
    int curManhattan = cal_Manhattan();
    if (isValid(x, y)) {
      steps++;
      swap(state[x][y], state[blankx][blankx]);
      int nextManhattan = cal_Manhattan();
      int deltaE = curManhattan - nextManhattan;
      if (deltaE > 0)
        blankx = x, blanky = y;
      else if (rand() % 100 > exp(deltaE / (double)T) * 100) {
        swap(state[x][y], state[blankx][blankx]);//reject
        T *= 0.99;
      }
      else {
        blankx = x, blanky = y;//accept
        T *= 0.99;
      }
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
  cout << "average steps: " << totalCost * 1 / (double)num * cnt << endl;
}

int main() {
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
