// #include <memory>
// #include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

// DP, f(l, r, k)表示移除[l, r]加上该区间右边等于Ar的k个元素组成的这个序列的最大积分
// {6, 3, 6, 5, 6, 7, 6, 6, 8, 6}, l = 1, r = 5那么f(l, r, 3)就是{[6, 3, 6, 5, 6], 7, 6, 6, 8, 6}, 其实7和8已经先被移除了
// 状态转移方程 f(l, r, k) = max(f(l, r - 1, 0) + (k + 1)^2, max_i=l_r-1{|f(l, i, k + 1) + f(i + 1, r - 1, 0)| * ϵ(ai == ar)})
// f(l, r - 1, 0) + (k + 1)^2表示把ar和后面的k个数一起删除, 再删除[l, r - 1]
// max_i=l_r-1{|f(l, i, k + 1) + f(i + 1, r - 1, 0)| * ϵ(ai == ar)}代表当ai(l <= i < r) = ar的时候先删掉[i + 1, r - 1]区间再删[l, i]和后面的k + 1个ar序列, 这样可以让ar序列比原先更长
// f(1, n, 0)即为答案
// https://blog.csdn.net/wangqing008/article/details/39430381
// 慢的原因是数据存储的局部性问题（data locality）:
// 1. 使用二位数组Depart[500][21]的时候，所有数据在内存中是连续存放的。访问内存时，内存访问的局部性较强，Cache命中的概率较大。L1 Cache访问延迟只有几个指令周期，而内存访问延迟则达到几百个指令周期。
// 2. Vector中的数据是连续存放的，但是Vector本身只保存指向数据块的指针。Vector建立在栈上，保存数据的数据块在堆上。因此二位动态数组Vector <Vector<int>>中，指向行的所有指针是连续存放的。每个行的数据是连续存放的，但是行与行之间是不连续存放的。因此跨行访问时局部性降低，Cache命中率下降。所以用时间更多
// time: O(n^4)
// space: O(n^3)
class Solution {
  public:
    int removeBoxes(vector<int> &boxes) {
        vector<vector<vector<int>>> idp(100, vector<vector<int>>(100, vector<int>(100, 0)));
        // memset(idp, 0, sizeof(idp));
        return calculatePoints(idp, boxes, 0, boxes.size() - 1, 0);
    }
  private:
    // int idp[100][100][100];  // 本地调试会爆栈
    // vector<vector<vector<int>>> idp;
    int calculatePoints(vector<vector<vector<int>>> &idp, vector<int> &boxes, int l, int r, int k) {
        if (l > r) return 0;
        if (idp[l][r][k] != 0) return idp[l][r][k];
        while (r > l && boxes[r] == boxes[r - 1]) { // 递归剪枝, 消除连续区间即可, 让k尽可能大到收敛
            --r;
            ++k;
        }
        idp[l][r][k] = calculatePoints(idp, boxes, l, r - 1, 0) + (k + 1) * (k + 1); // 将Ar和后面的k个数一起删除
        for (int i = l; i < r; ++i)
            if (boxes[i] == boxes[r])
                idp[l][r][k] = max(idp[l][r][k], calculatePoints(idp, boxes, l, i, k + 1) + calculatePoints(idp, boxes, i + 1, r - 1, 0));    // 将Ar拼接的更长再删除
        return idp[l][r][k];
    }
};

// DP, f(l, r, k)表示移除[l, r]加上该区间右边等于Ar的k个元素组成的这个序列的最大积分
// {6, 3, 6, 5, 6, 7, 6, 6, 8, 6}, l = 1, r = 5那么f(l, r, 3)就是{[6, 3, 6, 5, 6], 7, 6, 6, 8, 6}, 其实7和8已经先被移除了
// 状态转移方程 f(l, r, k) = max(f(l, r - 1, 0) + (k + 1)^2, max_i=l_r-1{|f(l, i, k + 1) + f(i + 1, r - 1, 0)| * ϵ(ai == ar)})
// f(l, r - 1, 0) + (k + 1)^2表示把ar和后面的k个数一起删除, 再删除[l, r - 1]
// max_i=l_r-1{|f(l, i, k + 1) + f(i + 1, r - 1, 0)| * ϵ(ai == ar)}代表当ai(l <= i < r) = ar的时候先删掉[i + 1, r - 1]区间再删[l, i]和后面的k + 1个ar序列, 这样可以让ar序列比原先更长
// f(1, n, 0)即为答案
// time: O(n^4)
// space: O(n^3)
class Solution0 {
  public:
    int removeBoxes(vector<int> &boxes) {
        // vector<vector<vector<int>>> idp(100, vector<vector<int>>(100, vector<int>(100, 0)));
        memset(idp, 0, sizeof(idp));
        return calculatePoints(boxes, 0, boxes.size() - 1, 0);
    }
  private:
    int idp[100][100][100];
    // vector<vector<vector<int>>> idp;
    int calculatePoints(vector<int> &boxes, int l, int r, int k) {
        if (l > r) return 0;
        if (idp[l][r][k] != 0) return idp[l][r][k];
        while (r > l && boxes[r] == boxes[r - 1]) { // 递归剪枝, 消除连续区间即可, 让k尽可能大到收敛
            --r;
            ++k;
        }
        idp[l][r][k] = calculatePoints( boxes, l, r - 1, 0) + (k + 1) * (k + 1); // 将Ar和后面的k个数一起删除
        for (int i = l; i < r; ++i)
            if (boxes[i] == boxes[r])
                idp[l][r][k] = max(idp[l][r][k], calculatePoints(boxes, l, i, k + 1) + calculatePoints(boxes, i + 1, r - 1, 0));    // 将Ar拼接的更长再删除
        return idp[l][r][k];
    }
};

int main() {
    Solution s;
    vector<int> boxes = {1, 3, 2, 2, 2, 3, 4, 3, 1};
    cout << s.removeBoxes(boxes) << endl;
    return 0;
}