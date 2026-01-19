#pragma warning(disable:4996)
#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include<vector>
#include <string>
#include<ctime>
using namespace std;
int trap(vector<int> height,int n, vector<int>& water) {
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
            water[left]= leftmax - height[left];
            left++;
        }
        else {
            res += rightmax - height[right];
            water[right]=rightmax - height[right];
            right--;
        }
    }
    return res;
}

void drawBarChart(vector<int>height, int size) {
    vector<int>water(size);
    trap(height, size, water);
    int maxVal = 0;
    // 寻找数组中的最大值
    for (int i = 0; i < size; ++i) {
        if (height[i] > maxVal) {
            maxVal = height[i];
        }
    }

    int barWidth = 50; // 柱状图每个柱子的宽度
    //int gap = 20;      // 柱子之间的间隔
    int startX = 80;  // 第一个柱子的起始 x 坐标
    int startY = 500;  // 柱状图的起始 y 坐标
    int axisY = 50;    // y 轴的起始坐标
    int axisX = 80;    // x 轴的起始坐标
    int axisLength = startY - axisY; // 坐标轴的长度
    int scaleGap = 50; // 刻度间隔

    // 绘制坐标轴
    setcolor(BLACK);
    line(axisX, startY, axisX, axisY); // y 轴
    line(axisX, startY, startX + (size + 1) * barWidth, startY); // x 轴

    // 绘制刻度
    for (int i = 0; i <= maxVal; ++i) {
        line(axisX - 5, startY - i * scaleGap, axisX, startY - i * scaleGap); // y 轴刻度线
        char text[10];
        sprintf(text, "%d", i); // 将整数转换为字符串
        outtextxy(axisX - 30, startY - i * scaleGap - 10, text); // 输出刻度值
    }
   
    int xScaleGap = (startX + (size + 1) * barWidth - startX) / size;
 
    

    // 绘制 x 轴刻度和标签
    for (int i = 0; i < size; ++i) {
        line(startX + (i + 1) * barWidth, startY, startX + (i + 1) * barWidth, startY + 5); // x 轴刻度线
        char text[10];
        sprintf(text, "%d", i+1); // 将整数转换为字符串，这里可以根据实际情况更改标签内容
        outtextxy(startX + (i + 1) * barWidth - 10, startY + 20, text); // 输出刻度值，调整位置使其在刻度线下方
    }
    // 绘制柱状图

    setfillcolor(BLACK); // 设置填充色为黑色
    for (int i = 0; i < size; ++i) {
        bar(startX + i * (barWidth), startY - height[i] * scaleGap,
            startX + (i + 1) * barWidth, startY);

        char text[10];
        sprintf(text, "%d", height[i]); // 将整数转换为字符串
        settextstyle(30, 0, _T("楷体"));
        outtextxy(550 + 25 * i, 10, text); // 输出数字
        if (i == 0) {
            outtextxy(535, 10, "[");
        }
        if (i == size-1) {
            outtextxy(550 + 25 * i + 15, 10, "]");
        }

    }

    setfillcolor(BLUE);
    for (int j = 0; j < water.size(); j++) {
        bar(startX + j * (barWidth), startY - water[j] * scaleGap - height[j] * scaleGap,
            startX + (j + 1) * barWidth, startY - height[j] * scaleGap);
    }

    int trapResult = trap(height, size, water); // 假设trapResult是trap函数的返回值
    char buffer[20]; // 缓冲区，用于存储格式化后的字符串
    sprintf(buffer, "%d", trapResult); // 将整数格式化为字符串
    settextstyle(40, 0, _T("楷体"));
    outtextxy(535, 60, "雨水量为："); // 输出数字
    outtextxy(720, 60,buffer);

    settextstyle(30,0,_T("楷体"));
    setfillcolor(YELLOW);
    solidrectangle(550, 140, 630, 180);
    outtextxy(560, 145, "刷新");
    solidrectangle(670, 140, 750, 180);
    outtextxy(680, 145, "退出");

}
void random(vector<int>&height) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < height.size(); i++) {
        height[i] = rand() % 10;
    }
}
void choice(HDC hdc) {
    srand(static_cast<unsigned int>(time(nullptr)));
    int a = rand() % 9+1;
    vector<int>height(a);
    random(height);
    int size = height.size(); // 数组的大小
    setbkcolor(WHITE);    // 设置背景色为白色
    cleardevice();        // 清除绘图窗口，以便背景色生效
    drawBarChart(height, size); // 绘制柱状图
    BitBlt(hdc, 0, 0, 800, 600, NULL, 0, 0, SRCCOPY); // 将绘制好的图像复制到屏幕上
}
void handleClick(int x, int y, HDC hdc) {
    // 检测是否在重新调用函数区域内
    if (x >= 550 && x <= 630 && y >= 140 && y <= 180) {
        // 重新调用函数
        choice(hdc);
    }
    if (x >= 680 && x <= 750 && y >= 140 && y <= 180) {
        // 退出程序
        exit(0);
    }
}
int main() {
    initgraph(800, 600); // 初始化绘图窗口
    HDC hdc = GetImageHDC();
    choice(hdc); // 第一次绘制
    while (true) {
        MOUSEMSG msg = GetMouseMsg(); // 获取鼠标事件
        // 检测是否为左键单击事件
        if (msg.uMsg == WM_LBUTTONUP) {
            // 检测点击位置并处理
            handleClick(msg.x, msg.y, hdc);
        }
    }
    // 关闭图形界面
    closegraph();
    return 0;
}

