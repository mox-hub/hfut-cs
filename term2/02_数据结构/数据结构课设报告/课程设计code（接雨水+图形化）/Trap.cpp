#include<iostream>
using namespace std;
int trap(int height[],int n) {
	if (!n)
		return 0;
	int left = 0, right = n - 1;
	int leftmax = height[0], rightmax = height[n - 1];
	int res = 0;
	while (left <= right) {
		leftmax = max(leftmax, height[left]);
		rightmax = max(rightmax, height[right]);

		if (leftmax < rightmax) {
			res += leftmax - height[left];
			left++;
		}
		else {
			res += rightmax - height[right];
			right--;
		}
	}
	return res;
}
int main() {
	int arr[] = {1,0,2,1,0,1,3,2,1,2,1};
	int n = sizeof(arr) / sizeof(arr[0]);
	cout << trap(arr, n);
}