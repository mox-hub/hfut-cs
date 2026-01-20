#include<iostream>
#include<vector>
using namespace std;

int trap(vector<int>& height) {
    int sum = 0;
    int n = height.size();
    for (int i = 0; i < n; i++) {
        int leftMax = 0;
        for (int j = 0; j <= i; j++) {
            if (height[j] > leftMax) {
                leftMax = height[j];
            }
        }
        int rightMax = 0;
        for (int j = i; j < n; j++) {
            if (height[j] > rightMax) {
                rightMax = height[j];
            }
        }
        sum += min(leftMax, rightMax) - height[i];
    }
    return sum;
}

int main() {
    vector<int>height = { 5, 4, 3, 3, 9 };
    cout << "接的雨水量为"<<trap(height);
}