#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

int direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // 右下左上
int current[3][3] = {{3, 4, 1}, {6, 0, 2}, {7, 8, 5}};  // 当前状态
int row_0, col_0;  // 记录0的坐标
int totalTrial;    // 统计移动步数

int Manhattan() {  // 计算曼哈顿距离
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (current[i][j] == 0) continue;
            int row = current[i][j]/3;
            int col = current[i][j]%3;
            int distance = abs(row - i) + abs(col - j);
            sum += distance;
        }
    }
    return sum;
}

void print() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            cout << current[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void initial() {
    for (int i = 0; i < 3; ++i) {  // 初始状态为目标状态
        for (int j = 0; j < 3; ++j) {
            current[i][j] = i*3+j;
        }
    }
    row_0 = 0, col_0 = 0;
    int last = -1;   // 上一次移动方向
    for (int i = 0; i < 20; i++) {  // 随机打乱
        bool upset = false;
        while (!upset) {  // 打乱成功才跳出循环
            int dir = rand() % 4;  // 随机选取一个方向
            if (last != -1 && last != dir && abs(last-dir) == 2)  continue;  // 避免反向走
            int x = row_0 + direction[dir][0];
            int y = col_0 + direction[dir][1];
            if (x >= 0 && x < 3 && y >= 0 && y < 3) {  // 方向可行
                swap(current[row_0][col_0], current[x][y]);  // 交换0和相邻数字的位置
                row_0 = x, col_0 = y;     // 更新0的坐标
                last = dir;               // 更新此次移动方向
                upset = true;             // 标记打乱成功
            }
        }
    }
}

// 判定是否有解
bool check() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            if (current[i][j] != i*3+j)
                return false;
    }
    return true;
}

// 爬山法
bool hillClimbing() {
    for (int trial = 0; trial < 200; trial++) {
        int curManha = Manhattan();  // 当前状态
        int minMan = 99999, minX = 0, minY = 0;
        for (int i = 0; i < 4; i++) {  // 在后继状态中找最小值
            int x = row_0 + direction[i][0];
            int y = col_0 + direction[i][1];
            if (x >= 0 && x < 3 && y >= 0 && y < 3) {  // 方向可行
                swap(current[row_0][col_0], current[x][y]);  // 交换0和相邻位置
                int nextManha = Manhattan();
                if (nextManha < minMan) {  // 获取下一状态的最小值
                    minMan = nextManha;
                    minX = x, minY = y;
                }
                swap(current[x][y], current[row_0][col_0]);  // 复原0和相邻位置
            }
        }
        if (curManha > minMan) {  // 最小值优于当前状态
            swap(current[row_0][col_0], current[minX][minY]);
            row_0 = minX, col_0 = minY;
        }
        if (check()) {   // 成功找到解
            totalTrial += trial;
            return true;
        }
    }
    return false;
}

// 首选爬山法
bool firstchose() {
    for (int trial = 0; trial < 500; trial++) {
        // 随机选取第一个优于当前状态的下一步
        bool next = false;
        int times = 0;
        while (!next) {
            int dir = rand() % 4;
            int curManha = Manhattan();
            int x = row_0 + direction[dir][0];
            int y = col_0 + direction[dir][1];
            if (x >= 0 && x < 3 && y >= 0 && y < 3) {  // 方向可行
                swap(current[row_0][col_0], current[x][y]);
                int nextManha = Manhattan();
                if (nextManha < curManha) {
                    row_0 = x, col_0 = y;
                    next = true;
                } else {
                    swap(current[x][y], current[row_0][col_0]);
                }
            }
            if (++times > 20) break;
        }
        if (check()) {   // 成功找到解
            totalTrial += trial;
            return true;
        }
    }
    return false;
}

// 模拟退火算法
bool simulated() {
    double temperature = 5;  // 初始温度
    int trial = 0;
    while (temperature > 0.00001) {
        vector<int> v;    // 选出可行的方向
        for (int i = 0; i < 4; i++) {
            int x = row_0 + direction[i][0];
            int y = col_0 + direction[i][1];
            if (x >= 0 && x < 3 && y >= 0 && y < 3) {  // 方向可行
                v.push_back(i);
            }
        }
        int curManha = Manhattan();      // 当前状态的曼哈顿距离之和
        int dir = v[rand() % v.size()];  // 随机选取一个可行方向
        int x = row_0 + direction[dir][0];
        int y = col_0 + direction[dir][1];
        swap(current[row_0][col_0], current[x][y]);  // 交换0和相邻节点的位置
        int nextManha = Manhattan();    // 交换之后的曼哈顿距离之和
        int E = nextManha - curManha;
        if (E < 0) {                    // 下一状态优于当前状态
            row_0 = x, col_0 = y;       // 更新0的位置
            trial++;
        } else if (exp((-1)*E/temperature) > ((double)(rand() % 1000) / 1000)) { // 以一定的概率选取
            row_0 = x, col_0 = y;
            trial++;
        } else {  // 不成功的话，复原0和相邻节点的位置
            swap(current[x][y], current[row_0][col_0]);
        }
        temperature *= 0.999;        // 温度下降
        if (check()) {   // 成功找到解
            totalTrial += trial;
            return true;
        }
    }
    return false;
}

// 最陡上升爬山法
int steepestAscent() {
    int count = 0;
    for (int i = 0; i < 1000; i++) {
        initial();
        //print();
        if (hillClimbing()) {
          count++;
          //cout << 1 << endl;
        }

    }
    return count;
}

// 首选爬山法
int firstChose() {
    int count = 0;
    for (int i = 0; i < 1000; i++) {
        initial();
        if (firstchose()) {
          count++;
          //print();
        }
    }
    return count;
}

// 随机重新开始爬山法
int randomRestart() {
    bool find = false;
    while (!find) {
        initial();
        find = hillClimbing();
    }
    return find;
}

// 模拟退火搜索
int simulatedAnnealing() {
    int count = 0;
    for (int i = 0; i < 1000; i++) {
        initial();
        if (simulated())
            count++;
    }
    return count;
}

int main(int argc, char const *argv[]) {
    srand((int)time(0));

    totalTrial = 0;
    cout << "steepest-Ascent searching..." << endl;
    int count = steepestAscent();
    cout << "steepest-Ascent[success/total]: " << count << "/1000" << endl;
    cout << "average steps: " << totalTrial/count << endl;

    totalTrial = 0;
    cout << "random-Restart searching..." << endl;
    int count2 = randomRestart();
    cout << "random-Restart: " << count2 << endl;
    cout << "average steps: " << totalTrial/count2 << endl;

    totalTrial = 0;
    cout << "first-Chose searching..." << endl;
    int count3 = firstChose();
    cout << "first-Chose[success/total]: " << count3 << "/1000" << endl;
    cout << "average steps: " << totalTrial/count3 << endl;

    totalTrial = 0;
    cout << "simulated-Annealing searching..." << endl;
    int count4 = simulatedAnnealing();
    cout << "simulated-Annealing[success/total]: " << count4 << "/1000" << endl;
    cout << "average steps: " << totalTrial/count4 << endl;

    return 0;
}
