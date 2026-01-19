#include<iostream>
#include<vector>
#include<stack>
using namespace std;
int trap(vector<int>& height) {
    int res = 0;
    stack<int> stack;
    for (int i = 0; i < height.size(); i++) {
        while (!stack.empty() && height[stack.top()] < height[i]) {
            int top = stack.top();
            stack.pop();
            if (stack.empty()) {
                break;
            }
            int width = i - stack.top() - 1;
            int h = min(height[i], height[stack.top()]) - height[top];
            res += width * h;
        }
        stack.push(i);
    }
    return res;
}

int main() {
    vector<int>height = { 5,4,3,3,9 };
    cout << trap(height) << endl;

}