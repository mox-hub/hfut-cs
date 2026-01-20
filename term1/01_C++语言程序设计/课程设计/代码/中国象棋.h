/*******************************************************************************
 * 文件名称：中国象棋.h
 * 功能描述：中国象棋双人对战游戏 - 头文件
 * 作者：
 * 创建日期：
 * 修改日期：2026-01-09
 * 编译环境：Visual Studio + EasyX图形库
 * 说明：
 *   - 实现完整的中国象棋规则
 *   - 支持双人对战模式
 *   - 使用EasyX库进行图形化显示
 *   - 包含边界检查，防止数组越界
 ******************************************************************************/

#pragma once

// ==================== 头文件包含 ====================
#include <easyx.h>      // EasyX图形库
#include <stdio.h>       // 标准输入输出
#include <conio.h>       // 控制台输入输出
#include <stdlib.h>      // 标准库函数
#include <graphics.h>    // 图形库

// ==================== 常量定义 ====================
#define ROW 10              // 棋盘行数（10行）
#define COL 9               // 棋盘列数（9列）
#define geziwidth 64        // 棋盘格子宽度（像素）- 原80 * 0.8
#define sxzywidth 64        // 棋盘边距（像素）- 原80 * 0.8

// 胜负判定常量
#define BLACK_GENERAL_ID 4  // 黑将的枚举ID
#define RED_GENERAL_ID 11   // 红帅的枚举ID

// ==================== 全局变量 ====================
static int type = RED;      // 当前回合方（RED=红方，BLACK=黑方）
static int g = 1;           // 回合计数器（奇数=红方，偶数=黑方）

// ==================== 数据结构定义 ====================

/**
 * 棋子结构体
 * @brief 存储棋子的位置、类型、ID等信息
 */
struct chess
{
	int x;         // 棋子在窗口中的X坐标（像素）
	int y;         // 棋子在窗口中的Y坐标（像素）
	int id;        // 棋子ID（对应piece枚举）
	int type;      // 棋子类型（RED或BLACK）
	int river;     // 过河标记（兵/卒专用，0=未过河，1=过河一次，2=可横移）
};

/**
 * 棋子枚举类型
 * @brief 定义所有棋子的类型和ID
 */
enum piece
{
	SPACE = -1,         // 空位（无棋子）
	// 黑方棋子（ID: 0-6）
	車, 馬, 象, 士, 将, 砲, 卒,
	// 红方棋子（ID: 7-13）
	俥, 马, 相, 仕, 帥, 炮, 兵,
	// 状态标记
	BEGIN, END          // 起子状态或落子状态
};

/**
 * 坐标结构体
 * @brief 存储鼠标点击的棋盘坐标
 */
struct START
{
	int x;  // 列坐标（0-8）
	int y;  // 行坐标（0-9）
};

// ==================== 全局数据定义 ====================
struct chess map[ROW][COL];         // 棋盘二维数组，每个位置对应一个棋子
struct START begin = { -1, -1 };    // 起始坐标（选中的棋子位置）
struct START end = { -1, -1 };      // 目标坐标（要移动到的位置）
int start = BEGIN;                  // 鼠标状态（BEGIN=选择棋子，END=选择目标位置）

// 棋子数组（用于初始化棋盘）
enum piece blackpiece[] = { 車, 馬, 象, 士, 将, 砲, 卒 };  // 黑方棋子列表
enum piece redpiece[] = { 俥, 马, 相, 仕, 帥, 炮, 兵 };    // 红方棋子列表

// 棋子名称数组（用于显示）
const char* chessname[] = { "車", "馬", "象", "士", "将", "砲", "卒",
                            "俥", "马", "相", "仕", "帥", "炮", "兵" };

// ==================== 函数声明 ====================

// ========== 基础游戏函数 ==========
void printfchess();           // 初始化棋盘
void printfboard();           // 绘制棋盘和棋子
void Mousecontrol();          // 处理鼠标输入和棋子移动
int Judge();                  // 判断胜负
int choise();                 // 游戏结束菜单选择

// ========== 将军和绝杀检测函数 ==========
bool IsUnderAttack(int row, int col, int attackType);           // 检查位置是否被攻击
bool IsInCheck();                                            // 检查是否将军
bool WouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol);  // 检查移动后是否送将
bool IsCheckmate();                                          // 检查是否绝杀

// ==================== 函数定义 ====================

/**
 * 初始化棋盘
 * @brief 按照中国象棋标准布局初始化所有棋子的位置
 * @note 黑方在上方（行0-4），红方在下方（行5-9）
 * @note 调用此函数后，棋盘恢复到初始状态
 */
void printfchess()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			enum piece chessid = SPACE;  // 默认为空位
			int chesstype;

			// ---------- 初始化黑方棋子（上方，行0-4） ----------
			if (i <= 4)
			{
				chesstype = BLACK;

				// 第一行：車馬象士将士象馬車
				if (i == 0)
				{
					if (j <= 4)
						chessid = blackpiece[j];          // 左半边：車馬象士将
					else
						chessid = blackpiece[8 - j];      // 右半边：象馬車（对称）
				}

				// 第三行：砲砲
				if (i == 2)
				{
					if (j == 1 || j == 7)
						chessid = blackpiece[5];          // 砲
				}

				// 第四行：卒卒卒卒卒
				if (i == 3)
				{
					if ((j + 1) % 2 == 1)                 // 奇数列放卒
						chessid = blackpiece[6];          // 卒
				}
			}
			// ---------- 初始化红方棋子（下方，行5-9） ----------
			else
			{
				chesstype = RED;

				// 最后一行（相对于红方）：俥马相仕帥仕相马俥
				if (9 - i == 0)
				{
					if (j <= 4)
						chessid = redpiece[j];            // 左半边：俥马相仕帥
					else
						chessid = redpiece[8 - j];        // 右半边：相马俥（对称）
				}

				// 炮位：炮炮
				if (9 - i == 2)
				{
					if (j == 1 || j == 7)
						chessid = redpiece[5];            // 炮
				}

				// 兵位：兵兵兵兵兵
				if (9 - i == 3)
				{
					if ((j + 1) % 2 == 1)                 // 奇数列放兵
						chessid = redpiece[6];            // 兵
				}
			}

			// ---------- 设置棋子属性 ----------
			map[i][j].river = 0;                         // 初始化过河标记
			map[i][j].id = chessid;                      // 设置棋子ID
			map[i][j].type = chesstype;                  // 设置棋子类型
			map[i][j].x = j * geziwidth + sxzywidth;     // 计算X像素坐标
			map[i][j].y = i * geziwidth + sxzywidth;     // 计算Y像素坐标
		}
	}
}

/**
 * 绘制棋盘和棋子
 * @brief 使用EasyX绘制完整的象棋棋盘和所有棋子
 * @note 窗口大小：800x880 像素
 */
void printfboard()
{
	// 创建图形窗口（800x880）
	initgraph(800, 880);

	// 设置背景色为木纹色
	setbkcolor(RGB(219, 121, 93));
	setlinecolor(BLACK);
	cleardevice();

	// ========== 绘制棋盘网格 ==========
	// 横线（10条）
	for (int i = 0; i <= 800; i += 80)
		line(80, i, 720, i);

	// 竖线（9条，中间断开形成楚河汉界）
	for (int i = 80; i <= 720; i += 80)
	{
		line(i, 80, i, 400);    // 黑方竖线
		line(i, 480, i, 800);   // 红方竖线
	}

	// 楚河汉界两边的竖线
	line(80, 400, 80, 480);
	line(720, 400, 720, 480);

	// ========== 绘制九宫格斜线 ==========
	line(320, 80, 480, 240);   // 黑方九宫格 \
	line(320, 240, 480, 80);   // 黑方九宫格 /
	line(320, 800, 480, 640);  // 红方九宫格 \
	line(320, 640, 480, 800);  // 红方九宫格 /

	// ========== 绘制"楚河汉界"文字 ==========
	settextcolor(BLACK);
	settextstyle(60, 0, _T("楷体"));
	outtextxy(180, 410, _T("楚"));
	outtextxy(260, 410, _T("河"));
	outtextxy(480, 410, _T("汉"));
	outtextxy(560, 410, _T("界"));

	// ========== 显示当前回合方 ==========
	if (g % 2 == 1)  // 红方回合
	{
		settextcolor(RED);
		settextstyle(40, 0, "楷体");
		outtextxy(640, 600, "红方落子");
	}
	if (g % 2 == 0)  // 黑方回合
	{
		settextcolor(BLACK);
		settextstyle(40, 0, "楷体");
		outtextxy(640, 110, "黑方落子");
	}

	// ========== 绘制所有棋子 ==========
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j].id != SPACE)  // 如果该位置有棋子
			{
				// 绘制棋子圆形背景（双层圆，形成边框效果）
				fillcircle(map[i][j].x, map[i][j].y, 38);  // 外圆
				fillcircle(map[i][j].x, map[i][j].y, 33);  // 内圆

				// 绘制棋子文字
				settextstyle(40, 34, "隶书");
				setbkmode(TRANSPARENT);                     // 文字背景透明
				settextcolor(map[i][j].type);               // 文字颜色（红/黑）
				outtextxy(map[i][j].x - 35, map[i][j].y - 20, chessname[map[i][j].id]);
			}
		}
	}

	// ========== 高亮显示选中的棋子 ==========
	if (start == END && begin.x != -1 && begin.y != -1)
	{
		int x = map[begin.y][begin.x].x;
		int y = map[begin.y][begin.x].y;
		// 绘制绿色高亮边框
		setlinecolor(GREEN);
		setlinestyle(PS_SOLID, 3);
		circle(x, y, 38);
		// 恢复默认样式
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 1);
	}

}

/**
 * 鼠标控制与棋子移动
 * @brief 处理鼠标点击事件，实现棋子选择和移动
 * @note 包含完整的象棋规则验证和边界检查
 */
void Mousecontrol()
{
	int n = 0;  // 移动标志（0=未移动，1=已选择目标，2=有效移动）
	ExMessage m;
	peekmessage(&m, EX_MOUSE);  // 获取鼠标消息

	if (m.message == WM_LBUTTONDOWN)  // 鼠标左键按下
	{
		// ---------- 计算点击位置对应的棋盘坐标 ----------
		int row = (m.y - sxzywidth) / geziwidth;
		int col = (m.x - sxzywidth) / geziwidth;

		// ========== 边界检查：防止数组越界 ==========
		if (row < 0 || row >= ROW || col < 0 || col >= COL)
		{
			return;  // 点击棋盘外，直接返回
		}

		// ========== 坐标修正：基于格子中心点调整 ==========
		int centerX = map[row][col].x;
		int centerY = map[row][col].y;

		// 根据鼠标点击位置相对于格子中心的位置，调整坐标
		if (m.x > centerX + geziwidth / 2 && m.y < centerY + geziwidth / 2)
			col++;          // 点击右侧，列+1
		else if (m.x < centerX + geziwidth / 2 && m.y > centerY + geziwidth / 2)
			row++;          // 点击下方，行+1
		else if (m.x > centerX + geziwidth / 2 && m.y > centerY + geziwidth / 2)
		{
			row++;          // 点击右下角
			col++;
		}

		// ========== 修正后再次检查边界 ==========
		if (row < 0 || row >= ROW || col < 0 || col >= COL)
		{
			return;
		}

		// ========== 处理棋子选择和目标位置 ==========
		if (start == BEGIN)  // 第一次点击：选择棋子
		{
			begin.x = col;
			begin.y = row;

			// 检查是否点击了当前回合方的棋子
			if (map[begin.y][begin.x].id != SPACE && map[begin.y][begin.x].type == type)
			{
				start = END;  // 进入落子状态
			}
		}
		else if (start == END)  // 第二次点击：选择目标位置
		{
			start = BEGIN;
			end.x = col;
			end.y = row;
			n = 1;  // 标记已选择目标位置
		}
	}

	// ========== 根据棋子类型验证并执行移动 ==========
	switch (map[begin.y][begin.x].id)
	{
		// ---------- 将/帅：九宫格内移动，每次一步 ----------
		case piece::将:
		case piece::帥:
			if (begin.y <= 4)  // 黑方将（上方九宫格：行0-2，列3-5）
			{
				for (int i = 0; i <= 2; i++)
				{
					for (int j = 3; j <= 5; j++)
					{
						// 验证：在九宫格内，走一步直线
						if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
							&& n == 1 && end.y == i && end.x == j && (begin.x == end.x || begin.y == end.y)
							&& (abs(begin.x - end.x) == 1 || abs(begin.y - end.y) == 1)
							&& map[begin.y][begin.x].type == type)
						{
							// 目标位置为空或敌方棋子
							if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
							{
								// 检查是否会导致送将（移动后自己的将/帅被攻击）
								if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
								{
									map[end.y][end.x].id = map[begin.y][begin.x].id;
									map[end.y][end.x].type = map[begin.y][begin.x].type;
									map[begin.y][begin.x].id = SPACE;
									n++;  // 标记移动成功
								}
							}
						}
					}
				}
			}
			else  // 红方帅（下方九宫格：行7-9，列3-5）
			{
				for (int i = 7; i <= 9; i++)
				{
					for (int j = 3; j <= 5; j++)
					{
						// 同上
						if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
							&& n == 1 && end.y == i && end.x == j && (begin.x == end.x || begin.y == end.y)
							&& (abs(begin.x - end.x) == 1 || abs(begin.y - end.y) == 1)
							&& map[begin.y][begin.x].type == type)
						{
							if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
							{
								// 检查是否会导致送将（移动后自己的将/帅被攻击）
								if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
								{
									map[end.y][end.x].id = map[begin.y][begin.x].id;
									map[end.y][end.x].type = map[begin.y][begin.x].type;
									map[begin.y][begin.x].id = SPACE;
									n++;
								}
							}
						}
					}
				}
			}
			break;

		// ---------- 士/仕：九宫格内斜走一步 ----------
		case piece::士:
		case piece::仕:
			if (begin.y <= 4)  // 黑方士
			{
				for (int i = 0; i <= 2; i++)
				{
					for (int j = 3; j <= 5; j++)
					{
						// 验证：在九宫格内，斜走一步
						if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
							&& n == 1 && end.y == i && end.x == j
							&& (abs(begin.x - end.x) == 1 && abs(begin.y - end.y) == 1)
							&& map[begin.y][begin.x].type == type)
						{
							if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
							{
								// 检查是否会导致送将（移动后自己的将/帅被攻击）
								if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
								{
									map[end.y][end.x].id = map[begin.y][begin.x].id;
									map[end.y][end.x].type = map[begin.y][begin.x].type;
									map[begin.y][begin.x].id = SPACE;
									n++;
								}
							}
						}
					}
				}
			}
			else  // 红方仕
			{
				for (int i = 7; i <= 9; i++)
				{
					for (int j = 3; j <= 5; j++)
					{
						// 同上
						if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
							&& n == 1 && end.y == i && end.x == j
							&& (abs(begin.x - end.x) == 1 && abs(begin.y - end.y) == 1)
							&& map[begin.y][begin.x].type == type)
						{
							if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
							{
								// 检查是否会导致送将（移动后自己的将/帅被攻击）
								if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
								{
									map[end.y][end.x].id = map[begin.y][begin.x].id;
									map[end.y][end.x].type = map[begin.y][begin.x].type;
									map[begin.y][begin.x].id = SPACE;
									n++;
								}
							}
						}
					}
				}
			}
			break;

		// ---------- 马/馬：走"日"字，检查蹩马腿 ----------
		case piece::马:
		case piece::馬:
			// 验证：走"日"字，检查蹩马腿（马腿处必须为空）
			if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
				&& n == 1 && (
					// 向右跳日字，检查左边蹩马腿
					(map[begin.y][begin.x - 1].id == SPACE && (begin.x - end.x == 2 && abs(begin.y - end.y) == 1)) ||
					// 向左跳日字，检查右边蹩马腿
					(map[begin.y][begin.x + 1].id == SPACE && (begin.x - end.x == -2 && abs(begin.y - end.y) == 1)) ||
					// 向下跳日字，检查上方蹩马腿
					(map[begin.y - 1][begin.x].id == SPACE && (abs(begin.x - end.x) == 1 && begin.y - end.y == 2)) ||
					// 向上跳日字，检查下方蹩马腿
					(map[begin.y + 1][begin.x].id == SPACE && (abs(begin.x - end.x) == 1 && begin.y - end.y == -2))
				)
				&& map[begin.y][begin.x].type == type)
			{
				if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
				{
					// 检查是否会导致送将（移动后自己的将/帅被攻击）
					if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
					{
						map[end.y][end.x].id = map[begin.y][begin.x].id;
						map[end.y][end.x].type = map[begin.y][begin.x].type;
						map[begin.y][begin.x].id = SPACE;
						n++;
					}
				}
			}
			break;

		// ---------- 象/相：走"田"字，不能过河，检查塞象眼 ----------
		case piece::象:
		case piece::相:
			if (begin.y <= 4)  // 黑方象（不能过河到红方）
			{
				// 验证：走"田"字，检查塞象眼，且不过河
				if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
					&& n == 1 && (
						// 右下象眼检查
						(map[begin.y - 1][begin.x - 1].id == SPACE && (begin.x - end.x == 2 && begin.y - end.y == 2)) ||
						// 左上象眼检查
						(map[begin.y + 1][begin.x + 1].id == SPACE && (begin.x - end.x == -2 && begin.y - end.y == -2)) ||
						// 左下象眼检查
						(map[begin.y - 1][begin.x + 1].id == SPACE && (begin.x - end.x == -2 && begin.y - end.y == 2)) ||
						// 右上象眼检查
						(map[begin.y + 1][begin.x + 1].id == SPACE && (begin.x - end.x == 2 && begin.y - end.y == -2))
					)
					&& end.y <= 4  // 不过河
					&& map[begin.y][begin.x].type == type)
				{
					if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
					{
						// 检查是否会导致送将（移动后自己的将/帅被攻击）
						if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
						{
							map[end.y][end.x].id = map[begin.y][begin.x].id;
							map[end.y][end.x].type = map[begin.y][begin.x].type;
							map[begin.y][begin.x].id = SPACE;
							n++;
						}
					}
				}
			}
			else  // 红方相（不能过河到黑方）
			{
				// 同上
				if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
					&& n == 1 && (
						(map[begin.y - 1][begin.x - 1].id == SPACE && (begin.x - end.x == 2 && begin.y - end.y == 2)) ||
						(map[begin.y + 1][begin.x + 1].id == SPACE && (begin.x - end.x == -2 && begin.y - end.y == -2)) ||
						(map[begin.y - 1][begin.x + 1].id == SPACE && (begin.x - end.x == -2 && begin.y - end.y == 2)) ||
						(map[begin.y + 1][begin.x + 1].id == SPACE && (begin.x - end.x == 2 && begin.y - end.y == -2))
					)
					&& end.y > 4  // 不过河
					&& map[begin.y][begin.x].type == type)
				{
					if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
					{
						// 检查是否会导致送将（移动后自己的将/帅被攻击）
						if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
						{
							map[end.y][end.x].id = map[begin.y][begin.x].id;
							map[end.y][end.x].type = map[begin.y][begin.x].type;
							map[begin.y][begin.x].id = SPACE;
							n++;
						}
					}
				}
			}
			break;

		// ---------- 兵/卒：过河前只能向前，过河后可横移 ----------
		case piece::兵:
		case piece::卒:
			if (map[begin.y][begin.x].river != 2)  // 未过河或刚过河一次
			{
				// 黑方卒（向下）
				if (begin.y <= 4)
				{
					// 只能向前走一步
					if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
						&& n == 1 && begin.y - end.y == -1 && begin.x == end.x
						&& map[begin.y][begin.x].type == type)
					{
						if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
						{
							// 检查是否会导致送将（移动后自己的将/帅被攻击）
							if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
							{
								map[begin.y][begin.x].river++;  // 标记过河
								map[end.y][end.x].id = map[begin.y][begin.x].id;
								map[end.y][end.x].type = map[begin.y][begin.x].type;
								map[end.y][end.x].river = map[begin.y][begin.x].river;
								map[begin.y][begin.x].id = SPACE;
								n++;
							}
						}
					}
				}
				// 红方兵（向上）
				else if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
					&& n == 1 && begin.y - end.y == 1 && begin.x == end.x
					&& map[begin.y][begin.x].type == type)
				{
					if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
					{
						// 检查是否会导致送将（移动后自己的将/帅被攻击）
						if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
						{
							map[begin.y][begin.x].river++;  // 标记过河
							map[end.y][end.x].id = map[begin.y][begin.x].id;
							map[end.y][end.x].type = map[begin.y][begin.x].type;
							map[end.y][end.x].river = map[begin.y][begin.x].river;
							map[begin.y][begin.x].id = SPACE;
							n++;
						}
					}
				}
			}
			else if (map[begin.y][begin.x].river == 2)  // 已过河，可横移
			{
				// 黑方卒
				if (begin.y > 4)
				{
					// 可以向前或向左右横移一步
					if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
						&& n == 1 && (begin.y - end.y == -1 || abs(begin.x - end.x) == 1)
						&& map[begin.y][begin.x].type == type)
					{
						if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
						{
							// 检查是否会导致送将（移动后自己的将/帅被攻击）
							if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
							{
								map[end.y][end.x].id = map[begin.y][begin.x].id;
								map[end.y][end.x].type = map[begin.y][begin.x].type;
								map[end.y][end.x].river = map[begin.y][begin.x].river;
								map[begin.y][begin.x].id = SPACE;
								n++;
							}
						}
					}
				}
				// 红方兵
				else if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
					&& n == 1 && (begin.y - end.y == 1 || abs(begin.x - end.x) == 1)
					&& map[begin.y][begin.x].type == type)
				{
					if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
					{
						// 检查是否会导致送将（移动后自己的将/帅被攻击）
						if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
						{
							map[end.y][end.x].id = map[begin.y][begin.x].id;
							map[end.y][end.x].type = map[begin.y][begin.x].type;
							map[end.y][end.x].river = map[begin.y][begin.x].river;
							map[begin.y][begin.x].id = SPACE;
							n++;
						}
					}
				}
			}
			break;

		// ---------- 車/俥：横竖直线移动，不能跨越棋子 ----------
		case piece::車:
		case piece::俥:
			// 验证：直线移动
			if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
				&& n == 1 && (begin.x == end.x || begin.y == end.y)
				&& map[begin.y][begin.x].type == type)
			{
				int a;
				// 向下移动
				if (begin.y - end.y < 0)
				{
					// 检查路径上是否有棋子
					for (a = begin.y; map[a + 1][begin.x].id == SPACE; a++);
					if (end.y <= a + 1)
					{
						if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
						{
							// 检查是否会导致送将（移动后自己的将/帅被攻击）
							if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
							{
								map[end.y][end.x].id = map[begin.y][begin.x].id;
								map[end.y][end.x].type = map[begin.y][begin.x].type;
								map[begin.y][begin.x].id = SPACE;
								n++;
							}
						}
					}
				}
				// 向上移动
				if (begin.y - end.y > 0)
				{
					for (a = begin.y; map[a - 1][begin.x].id == SPACE; a--);
					if (end.y >= a - 1)
					{
						if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
						{
							// 检查是否会导致送将（移动后自己的将/帅被攻击）
							if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
							{
								map[end.y][end.x].id = map[begin.y][begin.x].id;
								map[end.y][end.x].type = map[begin.y][begin.x].type;
								map[begin.y][begin.x].id = SPACE;
								n++;
							}
						}
					}
				}
				// 向右移动
				if (begin.x - end.x < 0)
				{
					for (a = begin.x; map[begin.y][a + 1].id == SPACE; a++);
					if (end.x <= a + 1)
					{
						if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
						{
							// 检查是否会导致送将（移动后自己的将/帅被攻击）
							if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
							{
								map[end.y][end.x].id = map[begin.y][begin.x].id;
								map[end.y][end.x].type = map[begin.y][begin.x].type;
								map[begin.y][begin.x].id = SPACE;
								n++;
							}
						}
					}
				}
				// 向左移动
				if (begin.x - end.x > 0)
				{
					for (a = begin.x; map[begin.y][a - 1].id == SPACE; a--);
					if (end.x >= a - 1)
					{
						if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
						{
							// 检查是否会导致送将（移动后自己的将/帅被攻击）
							if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
							{
								map[end.y][end.x].id = map[begin.y][begin.x].id;
								map[end.y][end.x].type = map[begin.y][begin.x].type;
								map[begin.y][begin.x].id = SPACE;
								n++;
							}
						}
					}
				}
			}
			break;

		// ---------- 炮/砲：横竖移动，吃子需跳过一个棋子 ----------
		case piece::炮:
		case piece::砲:
			{
				int h = 0, a = 0;  // h=路径上棋子数量，a=临时变量

				// 向下：统计路径上的棋子数
				if (n == 1 && end.y > begin.y && end.x == begin.x)
				{
					for (a = begin.y + 1; a < end.y; a++)
					{
						if (map[a][begin.x].id != SPACE)
							h++;  // 有棋子，计数+1
					}
				}
				// 向上：统计路径上的棋子数
				if (n == 1 && end.y < begin.y && end.x == begin.x)
				{
					for (a = begin.y - 1; a > end.y; a--)
					{
						if (map[a][begin.x].id != SPACE)
							h++;
					}
				}
				// 向右：统计路径上的棋子数
				if (n == 1 && end.y == begin.y && end.x > begin.x)
				{
					for (a = begin.x + 1; a < end.x; a++)
					{
						if (map[begin.y][a].id != SPACE)
							h++;
					}
				}
				// 向左：统计路径上的棋子数
				if (n == 1 && end.y == begin.y && end.x < begin.x)
				{
					for (a = begin.x - 1; a > end.x; a--)
					{
						if (map[begin.y][a].id != SPACE)
							h++;
					}
				}

				// 吃子：路径上必须恰好有一个棋子（炮架）
				if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
					&& n == 1 && (begin.x == end.x || begin.y == end.y)
					&& map[begin.y][begin.x].type == type && h == 1  // 一个炮架
					&& map[end.y][end.x].id != SPACE)  // 目标有棋子
				{
					if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
					{
						// 检查是否会导致送将（移动后自己的将/帅被攻击）
						if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
						{
							map[end.y][end.x].id = map[begin.y][begin.x].id;
							map[end.y][end.x].type = map[begin.y][begin.x].type;
							map[begin.y][begin.x].id = SPACE;
							n++;
						}
					}
				}

				// 移动：路径上不能有棋子
				if (begin.x != -1 && end.x != -1 && !((begin.x == end.x) && (begin.y == end.y))
					&& n == 1 && (begin.x == end.x || begin.y == end.y)
					&& map[begin.y][begin.x].type == type && h == 0  // 无炮架
					&& map[end.y][end.x].id == SPACE)  // 目标为空
				{
					if (map[end.y][end.x].id == SPACE || map[begin.y][begin.x].type != map[end.y][end.x].type)
					{
						// 检查是否会导致送将（移动后自己的将/帅被攻击）
						if (!WouldBeInCheckAfterMove(begin.y, begin.x, end.y, end.x))
						{
							map[end.y][end.x].id = map[begin.y][begin.x].id;
							map[end.y][end.x].type = map[begin.y][begin.x].type;
							map[begin.y][begin.x].id = SPACE;
							n++;
						}
					}
				}
			}
			break;
	}

	// ========== 处理回合切换 ==========
	if (n == 2)  // 有效移动成功
	{
		g++;  // 回合计数+1
		if (g % 2 == 1)
			type = RED;   // 切换到红方
		else
			type = BLACK; // 切换到黑方
	}
	else if (n == 1)  // 无效移动
	{
		start = BEGIN;  // 重置状态，等待重新选择
	}
}

/**
 * 判断胜负
 * @brief 检查双方将/帅是否还在棋盘上
 * @return 0=游戏结束（一方获胜），1=游戏继续
 */
int Judge()
{
	int n = 0, m = 0;

	// 扫描黑方九宫格（行0-2，列3-5）
	for (int i = 0; i <= 2; i++)
	{
		for (int j = 3; j <= 5; j++)
		{
			if (map[i][j].id == BLACK_GENERAL_ID)
				n++;  // 找到黑将
		}
	}

	// 扫描红方九宫格（行7-9，列3-5）
	for (int i = 7; i <= 9; i++)
	{
		for (int j = 3; j <= 5; j++)
		{
			if (map[i][j].id == RED_GENERAL_ID)
				m++;  // 找到红帅
		}
	}

	// 判断胜负
	if (m == 0 || n == 0)  // 一方的将/帅被吃
	{
		// 重置游戏状态
		type = RED;
		g = 1;

		// 创建游戏结束窗口（640x400 - 原800x500的80%）
		initgraph(640, 400);
		setbkcolor(RGB(5, 225, 250));  // 设置背景色
		cleardevice();
		setlinecolor(BLACK);
		setfillcolor(RGB(255, 128, 192));  // 按钮颜色
		settextcolor(RGB(255, 225, 0));    // 文字颜色

		// 绘制按钮
		fillrectangle(80, 240, 240, 320);   // 继续游戏按钮（原100*0.8, 原300*0.8, 原300*0.8, 原400*0.8）
		fillrectangle(400, 240, 560, 320);  // 退出游戏按钮（原500*0.8, 原300*0.8, 原700*0.8, 原400*0.8）

		// 显示获胜信息
		if (n == 0)  // 黑将被吃，红方获胜
		{
			settextstyle(80, 0, "楷体");     // 原100*0.8=80
			setbkmode(TRANSPARENT);  // 背景透明
			outtextxy(80, 80, "恭喜红方获胜"); // 原100*0.8=80
			settextstyle(32, 0, "楷体");      // 原40*0.8=32
			outtextxy(96, 264, "继续游戏");   // 原120*0.8=96, 原330*0.8=264
			outtextxy(416, 264, "退出游戏");  // 原520*0.8=416
		}
		if (m == 0)  // 红帅被吃，黑方获胜
		{
			settextstyle(80, 0, "楷体");     // 原100*0.8=80
			setbkmode(TRANSPARENT);
			outtextxy(80, 80, "恭喜黑方获胜"); // 原100*0.8=80
			settextstyle(32, 0, "楷体");      // 原40*0.8=32
			outtextxy(96, 264, "继续游戏");   // 原120*0.8=96, 原330*0.8=264
			outtextxy(416, 264, "退出游戏");  // 原520*0.8=416
		}
		return 0;  // 游戏结束
	}
	else
	{
		return 1;  // 游戏继续
	}
}

/**
 * 游戏结束菜单选择
 * @brief 处理游戏结束后的菜单交互（继续游戏/退出游戏）
 * @return 0=未选择，1=继续游戏，2=退出游戏
 */
int choise()
{
	ExMessage m;
	peekmessage(&m, EX_MOUSE);

	// 鼠标移动效果（按钮高亮）
	if (m.message == WM_MOUSEMOVE)
	{
		// 鼠标悬停在"继续游戏"按钮上
		if (m.y > 240 && m.y < 320 && m.x > 80 && m.x < 240)  // 原300*0.8=240, 原400*0.8=320, 原100*0.8=80, 原300*0.8=240
		{
			setfillcolor(RGB(255, 26, 140));  // 高亮颜色
			fillrectangle(80, 240, 240, 320);
			settextcolor(RGB(255, 225, 0));
			settextstyle(32, 0, "楷体");  // 原40*0.8=32
			outtextxy(96, 264, "继续游戏");  // 原120*0.8=96, 原330*0.8=264
			FlushBatchDraw();
		}
		// 鼠标悬停在"退出游戏"按钮上
		else if ((m.y > 240 && m.y < 320 && m.x > 400 && m.x < 560))  // 同上，右边按钮
		{
			setfillcolor(RGB(255, 26, 140));
			fillrectangle(400, 240, 560, 320);
			settextcolor(RGB(255, 225, 0));
			BeginBatchDraw();
			settextstyle(32, 0, "楷体");  // 原40*0.8=32
			outtextxy(416, 264, "退出游戏");  // 原520*0.8=416
			FlushBatchDraw();
		}
		// 鼠标移出按钮区域
		else
		{
			setfillcolor(RGB(255, 128, 192));  // 恢复默认颜色
			settextcolor(RGB(255, 225, 0));
			fillrectangle(80, 240, 240, 320);
			fillrectangle(400, 240, 560, 320);
			settextstyle(32, 0, "楷体");  // 原40*0.8=32
			outtextxy(96, 264, "继续游戏");  // 原120*0.8=96, 原330*0.8=264
			outtextxy(416, 264, "退出游戏");  // 原520*0.8=416
			FlushBatchDraw();
		}
	}

	// 鼠标点击事件
	if (m.message == WM_LBUTTONDOWN)
	{
		// 点击"继续游戏"
		if (m.y > 240 && m.y < 320 && m.x > 80 && m.x < 240)  // 原尺寸的80%
		{
			m.x = 0;
			m.y = 0;
			return 1;  // 返回1，继续游戏
		}
		// 点击"退出游戏"
		if (m.y > 240 && m.y < 320 && m.x > 400 && m.x < 560)  // 原尺寸的80%
		{
			return 2;  // 返回2，退出游戏
		}
	}

	return 0;  // 未选择
}

// ==================== 将军和绝杀判定函数 ====================

/**
 * 检查某个位置是否被攻击
 * @brief 判断指定位置是否在对方棋子的攻击范围内
 * @param row 要检查的行坐标
 * @param col 要检查的列坐标
 * @param attackType 攻击方的类型（RED或BLACK）
 * @return true=被攻击，false=未被攻击
 */
bool IsUnderAttack(int row, int col, int attackType)
{
	// 遍历攻击方的所有棋子
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j].id != SPACE && map[i][j].type == attackType)
			{
				int pieceId = map[i][j].id;

				// 根据棋子类型检查能否攻击到目标位置
				switch (pieceId)
				{
					// 車/俥：直线攻击
					case piece::車:
					case piece::俥:
						if ((i == row || j == col) && !(i == row && j == col))
						{
							bool canAttack = true;
							// 检查路径上是否有棋子阻挡
							if (i == row && j < col) // 向右
							{
								for (int k = j + 1; k < col; k++)
									if (map[i][k].id != SPACE) canAttack = false;
							}
							else if (i == row && j > col) // 向左
							{
								for (int k = j - 1; k > col; k--)
									if (map[i][k].id != SPACE) canAttack = false;
							}
							else if (j == col && i < row) // 向下
							{
								for (int k = i + 1; k < row; k++)
									if (map[k][j].id != SPACE) canAttack = false;
							}
							else if (j == col && i > row) // 向上
							{
								for (int k = i - 1; k > row; k--)
									if (map[k][j].id != SPACE) canAttack = false;
							}
							if (canAttack) return true;
						}
						break;

					// 馬：走"日"字攻击
					case piece::马:
					case piece::馬:
						if ((abs(j - col) == 2 && abs(i - row) == 1) || (abs(j - col) == 1 && abs(i - row) == 2))
						{
							// 检查马腿
							bool blocked = false;
							if (abs(j - col) == 2) // 横向跳跃
								blocked = (map[i][j + (col > j ? 1 : -1)].id != SPACE);
							else // 纵向跳跃
								blocked = (map[i + (row > i ? 1 : -1)][j].id != SPACE);

							if (!blocked) return true;
						}
						break;

					// 炮/砲：隔子攻击
					case piece::炮:
					case piece::砲:
						if ((i == row || j == col) && !(i == row && j == col))
						{
							int h = 0; // 路径上的棋子数
							if (i == row) // 同一行
							{
								int start = min(j, col);
								int end = max(j, col);
								for (int k = start + 1; k < end; k++)
									if (map[i][k].id != SPACE) h++;
							}
							else // 同一列
							{
								int start = min(i, row);
								int end = max(i, row);
								for (int k = start + 1; k < end; k++)
									if (map[k][j].id != SPACE) h++;
							}

							// 炮需要恰好一个炮架才能攻击
							if (h == 1) return true;
						}
						break;

					// 兵/卒：向前或横向攻击
					case piece::兵:
						// 红方兵
						if (i >= 5)
						{
							if ((row == i - 1 && col == j) || (row == i && abs(col - j) == 1))
								return true;
						}
						break;

					case piece::卒:
						// 黑方卒
						if (i <= 4)
						{
							if ((row == i + 1 && col == j) || (row == i && abs(col - j) == 1))
								return true;
						}
						break;

					// 将/帅：九宫格内攻击
					case piece::将:
					case piece::帥:
						if (attackType == RED) // 红帅
						{
							if (row >= 7 && row <= 9 && col >= 3 && col <= 5)
							{
								if ((abs(i - row) == 1 && j == col) || (abs(j - col) == 1 && i == row))
									return true;
							}
						}
						else // 黑将
						{
							if (row <= 4 && row >= 0 && col >= 3 && col <= 5)
							{
								if ((abs(i - row) == 1 && j == col) || (abs(j - col) == 1 && i == row))
									return true;
							}
						}
						break;
				}
			}
		}
	}
	return false;
}

/**
 * 检查是否将军
 * @brief 判断当前行动方是否将了对方的军
 * @return true=将军，false=未将军
 */
bool IsInCheck()
{
	// 找到被将军方的将/帅位置
	int kingRow = -1, kingCol = -1;
	int kingType = (type == RED) ? BLACK : RED;
	int kingId = (type == RED) ? BLACK_GENERAL_ID : RED_GENERAL_ID;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j].id == kingId && map[i][j].type == kingType)
			{
				kingRow = i;
				kingCol = j;
				break;
			}
		}
		if (kingRow != -1) break;
	}

	if (kingRow == -1) return false; // 将/帅已被吃

	// 检查将/帅是否被攻击
	return IsUnderAttack(kingRow, kingCol, type);
}

/**
 * 检查移动后是否会导致送将
 * @brief 验证移动是否合法（不能导致自己的将/帅被攻击）
 * @param fromRow 起始行
 * @param fromCol 起始列
 * @param toRow 目标行
 * @param toCol 目标列
 * @return true=会导致送将，false=合法移动
 */
bool WouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol)
{
	// 保存当前状态
	struct chess savedPiece = map[toRow][toCol];
	struct chess savedFrom = map[fromRow][fromCol];

	// 模拟移动
	map[toRow][toCol] = savedFrom;
	map[fromRow][fromCol].id = SPACE;

	// 临时切换回合（因为现在是攻击方在检查）
	int originalType = type;
	type = (type == RED) ? BLACK : RED; // 切换到防守方

	// 检查自己的将/帅是否被攻击
	bool inCheck = IsInCheck();

	// 恢复状态
	type = originalType;
	map[fromRow][fromCol] = savedFrom;
	map[toRow][toCol] = savedPiece;

	return inCheck;
}

/**
 * 检查是否绝杀
 * @brief 判断被将军方是否无路可走（绝杀）
 * @return true=绝杀，false=未绝杀
 */
bool IsCheckmate()
{
	if (!IsInCheck()) return false; // 未将军，不可能是绝杀

	int defenderType = (type == RED) ? BLACK : RED; // 被将军方
	int attackerType = type; // 攻击方

	// 找到被将军方的将/帅
	int kingId = (defenderType == RED) ? RED_GENERAL_ID : BLACK_GENERAL_ID;
	int kingRow = -1, kingCol = -1;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j].id == kingId && map[i][j].type == defenderType)
			{
				kingRow = i;
				kingCol = j;
				break;
			}
		}
		if (kingRow != -1) break;
	}

	if (kingRow == -1) return true; // 将/帅已被吃，绝杀

	// 1. 首先检查将/帅本身是否能移动到安全位置
	// 将/帅只能在九宫格内移动
	int palaceStart = (defenderType == RED) ? 7 : 0;
	int palaceEnd = (defenderType == RED) ? 9 : 2;

	for (int i = palaceStart; i <= palaceEnd; i++)
	{
		for (int j = 3; j <= 5; j++)
		{
			// 跳过当前位置
			if (i == kingRow && j == kingCol) continue;

			// 只能走一步直线
			if ((abs(i - kingRow) == 1 && j == kingCol) || (abs(j - kingCol) == 1 && i == kingRow))
			{
				// 检查目标位置是否为空或敌方棋子
				if (map[i][j].id == SPACE || map[i][j].type != defenderType)
				{
					// 模拟将/帅移动
					chess savedPiece = map[i][j];
					map[i][j] = map[kingRow][kingCol];
					map[kingRow][kingCol].id = SPACE;

					// 检查移动后是否还被攻击
					bool stillInCheck = IsUnderAttack(i, j, attackerType);

					// 恢复
					map[kingRow][kingCol] = map[i][j];
					map[i][j] = savedPiece;

					if (!stillInCheck) return false; // 将/帅可以逃到安全位置
				}
			}
		}
	}

	// 2. 检查其他棋子是否能吃掉攻击方或将/帅移出攻击线
	// 遍历被将军方的所有棋子（除了将/帅）
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map[i][j].id != SPACE && map[i][j].type == defenderType)
			{
				if (map[i][j].id == kingId) continue; // 跳过将/帅

				// 尝试所有可能的目标位置
				for (int ti = 0; ti < ROW; ti++)
				{
					for (int tj = 0; tj < COL; tj++)
					{
						if (ti == i && tj == j) continue;
						if (map[ti][tj].id != SPACE && map[ti][tj].type == defenderType) continue;

						// 模拟移动
						chess savedFrom = map[i][j];
						chess savedTo = map[ti][tj];
						map[ti][tj] = savedFrom;
						map[i][j].id = SPACE;

						// 检查将/帅是否还被攻击
						bool stillInCheck = IsUnderAttack(kingRow, kingCol, attackerType);

						// 恢复
						map[i][j] = savedFrom;
						map[ti][tj] = savedTo;

						if (!stillInCheck) return false; // 这个移动可以解除将军
					}
				}
			}
		}
	}

	return true; // 无路可走，绝杀
}
