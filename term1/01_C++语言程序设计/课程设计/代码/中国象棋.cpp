/*******************************************************************************
 * 文件名称：中国象棋.cpp
 * 功能描述：中国象棋双人对战游戏 - 主程序
 * 作者：
 * 创建日期：
 * 修改日期：2026-01-09
 * 编译环境：Visual Studio + EasyX图形库
 * 说明：
 *   - 游戏主循环，处理游戏流程
 *   - 使用双缓冲绘图防止闪烁
 *   - 支持重新开始和退出游戏
 ******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS 1  // 关闭安全警告
#include "中国象棋.h"               // 包含游戏头文件
#include <windows.h>                // Windows API

/**
 * 主函数
 * @brief 游戏主入口，控制游戏流程
 * @return 程序退出码
 */
int main()
{
	int n = 0;  // 菜单选择标志（0=未选择，1=重新开始，2=退出游戏）

	// ========== 游戏主循环 ==========
	do
	{
		n = 0;  // 重置菜单选择

		// 创建图形窗口（640x704）- 原800x880的80%
		initgraph(640, 704);

		// 初始化棋盘布局
		printfchess();

		// 启用双缓冲绘图（防止闪烁）
		BeginBatchDraw();

		// ========== 游戏进行循环 ==========
		while (1)
		{
			// 绘制棋盘和棋子（注意：这里不应该重复调用initgraph）
			// 清空绘图设备
			cleardevice();

			// 绘制棋盘背景
			setbkcolor(RGB(219, 121, 93));
			setlinecolor(BLACK);

			// 绘制棋盘网格
			for (int i = 0; i <= 640; i += 64)  // 原800*0.8=640, 原80*0.8=64
				line(64, i, 576, i);  // 原80*0.8=64, 原720*0.8=576
			for (int i = 64; i <= 576; i += 64)
			{
				line(i, 64, i, 320);   // 原80*0.8=64, 原400*0.8=320
				line(i, 384, i, 640);  // 原480*0.8=384, 原800*0.8=640
			}
			line(64, 320, 64, 384);    // 楚河汉界左边竖线
			line(576, 320, 576, 384);  // 楚河汉界右边竖线

			// 绘制九宫格斜线
			line(256, 64, 384, 192);   // 黑方九宫格 \ (原320*0.8, 原80*0.8, 原480*0.8, 原240*0.8)
			line(256, 192, 384, 64);   // 黑方九宫格 /
			line(256, 512, 384, 640);  // 红方九宫格 \ - 左上到右下
			line(256, 640, 384, 512);  // 红方九宫格 / - 左下到右上

			// 绘制"楚河汉界"
			settextcolor(BLACK);
			settextstyle(48, 0, _T("楷体"));  // 原60*0.8=48
			outtextxy(144, 328, _T("楚"));    // 原180*0.8=144, 原410*0.8≈328
			outtextxy(208, 328, _T("河"));    // 原260*0.8=208
			outtextxy(384, 328, _T("汉"));    // 原480*0.8=384
			outtextxy(448, 328, _T("界"));    // 原560*0.8=448

			// 显示当前回合方
			if (g % 2 == 1)
			{
				settextcolor(RED);
				settextstyle(32, 0, "楷体");  // 原40*0.8=32
				outtextxy(512, 32, "红方走棋"); // 原640*0.8=512, 原40*0.8=32
			}
			else
			{
				settextcolor(BLACK);
				settextstyle(32, 0, "楷体");  // 原40*0.8=32
				outtextxy(512, 32, "黑方走棋"); // 原640*0.8=512, 原40*0.8=32
			}

			// 绘制所有棋子
			for (int i = 0; i < ROW; i++)
			{
				for (int j = 0; j < COL; j++)
				{
					if (map[i][j].id != SPACE)
					{
						fillcircle(map[i][j].x, map[i][j].y, 30);  // 原38*0.8≈30
						fillcircle(map[i][j].x, map[i][j].y, 26);  // 原33*0.8≈26
						settextstyle(32, 27, "隶书");              // 原40*0.8=32, 原34*0.8≈27
						setbkmode(TRANSPARENT);
						settextcolor(map[i][j].type);
						outtextxy(map[i][j].x - 28, map[i][j].y - 16, chessname[map[i][j].id]); // 原35*0.8≈28, 原20*0.8=16
					}
				}
			}

			// 高亮显示选中的棋子
			if (start == END && begin.x != -1 && begin.y != -1)
			{
				int x = map[begin.y][begin.x].x;
				int y = map[begin.y][begin.x].y;
				setlinecolor(GREEN);
				setlinestyle(PS_SOLID, 2);  // 原3*0.8≈2（四舍五入）
				circle(x, y, 30);           // 原38*0.8=30
				setlinecolor(BLACK);
				setlinestyle(PS_SOLID, 1);
			}

			// 显示将军提示（如果被将军）
			if (IsInCheck())
			{
				settextcolor(RED);
				settextstyle(48, 0, "楷体");
				outtextxy(224, 320, "将军!");
			}

			// 刷新显示（双缓冲）
			FlushBatchDraw();

			// 处理鼠标控制（棋子移动）
			Mousecontrol();

			// 检查绝杀（在棋子移动后检查）
			bool inCheck = IsInCheck();
			if (inCheck)
			{
				bool checkmate = IsCheckmate();
				if (checkmate)
				{
					// 绝杀！显示2秒后结束游戏
					settextcolor(RED);
					settextstyle(64, 0, "楷体");
					outtextxy(200, 320, "绝杀!");
					FlushBatchDraw();
					Sleep(2000);
					break;  // 结束游戏循环
				}
			}

			// 判断胜负
			int a = Judge();
			if (a == 0)  // 游戏结束
				break;
		}

		// ========== 游戏结束菜单循环 ==========
		EndBatchDraw();  // 结束双缓冲

		while (n == 0)
		{
			// 等待玩家选择（重新开始/退出游戏）
			n = choise();
		}

		// 关闭图形窗口
		closegraph();

	} while (n == 1);  // 如果选择"重新游戏"，则继续循环

	return 0;  // 程序正常退出
}
