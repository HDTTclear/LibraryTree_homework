#include<stdio.h>
#define MAX 9//操作穷举
//#define LENGTH 45
#include <ctype.h>
#include<ctype.h>


/*
26 lowercase letters. In this program,both capital and lowercase are the same.
so,we need to make the input string all into lowercase.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Coord {
	int x, y;
	//Coord(int a, int b) :x(a), y(b) {}
	Coord operator+(const Coord& c) const { return { x + c.x,y + c.y }; }
	void operator+=(const Coord& c) { x += c.x, y += c.y; }
	bool operator==(const Coord& c) const { return x == c.x && y == c.y; }
	bool operator!=(const Coord& c) const { return x != c.x || y != c.y; }
	int dis(const Coord& c) const { return abs(x - c.x) + abs(y - c.y); }
	bool online(const Coord& c) const { return (x == c.x) ^ (y == c.y); } //notice: here same coord is not online
	int getdir(const Coord& v) const {
		//assert(online(v));
		if (v.x > x) return DOWN;
		else if (v.x < x) return UP;
		else if (v.y > y) return RIGHT;
		else return LEFT;
	}
};
inline bool inborder(int x, int y) { return x >= 0 && y >= 0 && x < 9 && y < 9; }
inline bool inborder(Coord c) { return c.x >= 0 && c.y >= 0 && c.x < 9 && c.y < 9; }

const Coord dir[4] = { {-1,0},{0,1},{1,0},{0,-1} };
const Coord basepos[2] = { {0,4},{8,4} }struct Board {
	//0: empty, 1: brick & base, 2:steel, 3x: x tanks ,-2:water
	int b[9][9];
	static const int b0[9][9];
	int& operator[](const Coord& c) { return b[c.x][c.y]; }
	void setBoard(int hasBrick[3], int hasSteel[3], int hasWater[3]) {
		memcpy(b, b0, sizeof b);
		for (int i = 0; i < 3; i++) {
			int mask = 1;
			for (int y = i * 3; y < (i + 1) * 3; y++)
				for (int x = 0; x < 9; x++)
				{
					if (hasBrick[i] & mask) b[y][x] = 1; //brick
					else if (hasSteel[i] & mask) b[y][x] = 2; //steel
					else if (hasWater[i] & mask) b[y][x] = -2; //water
					mask <<= 1;
				}
		}
	}
};;
const int Board::b0[9][9] = {
{0,0,3,1,1,1,3,0,0},
{0,0,0,1,1,1,0,0,0},
{0,0,0,0,1,0,0,0,0},
{0,0,0,0,1,0,0,0,0},
{1,1,2,1,2,1,2,1,1},
{0,0,0,0,1,0,0,0,0},
{0,0,0,0,1,0,0,0,0},
{0,0,0,1,1,1,0,0,0},
{0,0,3,1,1,1,3,0,0}
};

//min step of CTF
/* kb[i][j]表示从坐标(i,j)位置到打碎对方基地所需要的回合数 */
/* vis[i][j]存广搜时是否已经搜过坐标为(i,j)的点 */
/* is22b[i][j]存坐标(i,j)位置是否是窄路(被夹逼的状态)，is22[i]存编号为i的坦克是否处在窄路上(被夹逼的状态) */
int kb[2][9][9], ekb[2][9][9];
bool vis[9][9], is22b[9][9], is22[4];

//new state
class GameState 
{
public:
	//B1, B2, R1, R2
	Coord tank[4];
	bool lastshoot[4];
	Act lastmove[4];
	bool dead[4];
	/**< imaging a virtual board */
	Board board;
	int step;
	GameState* pre;
	void init(int hasBrick[3], int hasSteel[3], int hasWater[3]) {
		step = 0;
		tank[0] = { 0,2 }; tank[1] = { 0,6 };
		tank[2] = { 8,2 }; tank[3] = { 8,6 };
		for (int i = 0; i < 4; i++) lastshoot[i] = dead[i] = 0;
		board.setBoard(hasBrick, hasSteel, hasWater);
		pre = this;
	}
	int judgeend() {
		if ((dead[0] && dead[1]) || (dead[2] && dead[3]))
			return 2 * (int)(dead[0] && dead[1]) + (int)(dead[2] && dead[3]);
		return 2 * !board.b[0][4] + !board.b[8][4]; //win/loss/draw
	}
	//!-- this process won't check available
	//Return game ended? 1: B win, 2:R win, 3: Draw
	/**< changed */
	int forward(Act move[4], GameState* pre1) {
		step++;
		pre = pre1;
		Coord destroyed[4]; int dc = 0;
		for (int i = 0; i < 4; i++) lastmove[i] = move[i];
		for (int i = 0; i < 4; i++) {
			if (dead[i]) move[i] = -1;
			if (move[i] < 0) continue;
			if (move[i] < 4)
				board[tank[i]] -= 3,
				tank[i] += dir[move[i]],
				board[tank[i]] += 3;
		}
		for (int i = 0; i < 4; i++)
			if (move[i] >= 4) {
				lastshoot[i] = 1;
				int d = move[i] - 4;
				auto c = tank[i] + dir[d];
				for (; (board[c] <= 0) && inborder(c); c += dir[d]);
				if (!inborder(c)) continue;
				int st = board[c];
				//assert(st);
				if (st == 1) destroyed[dc++] = c;
				else if (st == 2) continue;
				else {
					destroyed[dc++] = c;
					if (board[tank[i]] == 3 && st == 3) //spj oppisite shot
						for (int j = 0; j < 4; j++)
							if (move[j] >= 4 && move[i] % 4 == (move[j] + 2) % 4 && tank[j] == c)
							{
								dc--; break;
							}
				}
			}
			else lastshoot[i] = 0;
		for (int i = 0; i < dc; i++) {
			const auto& c = destroyed[i];
			if (board[c] == 2) continue;
			if (board[c] > 2) {
				for (int j = 0; j < 4; j++)
					if (tank[j] == c) {
						tank[j] = { -1,-1 };
						dead[j] = 1;
					}
			}
			board[c] = 0;
		}
		return judgeend();
	}
	bool available(int tk, Act move) {
		if (move == -1) return 1;
		if (move < 4) return inborder(tank[tk] + dir[move]) && board[tank[tk] + dir[move]] == 0;
		return !lastshoot[tk];
	}
	bool availableforrush(int tk, Act move) {
		if (move == -1) return 1;
		else if (move < 4 && (board[tank[tk] + dir[move]] == 3))
			return kb[tk > 1][tank[tk].x][tank[tk].y] + 5 >= kb[tk < 2][tank[tk].x][tank[tk].y];	//5只是一个测试的值，可能要改
		else if(move < 4) return inborder(tank[tk] + dir[move]) 
			&& (board[tank[tk] + dir[move]] == 1 || board[tank[tk] + dir[move]] == 0);
		return !lastshoot[tk];
	}
	void debugPrint() {
		for (int i = 0; i < 9; i++, cout << '\n')
			for (int j = 0; j < 9; j++)
				switch (board.b[i][j]) {
				case 0: cout << "Â·"; break;
				case 1: cout << "ï¼ƒ"; break;
				case 2: cout << "ï¼Š"; break;
				case -2:cout << "水"; break;
				case 3: case 6: case 9: case 12:
					cout << "ï¼´"; break;
				default: assert(false);
				}
		cout << '\n';
	}

	/**< 对于新规则没什么用 */
	//int killermoves(int tk) {
	//	if (dead[tk]) return 100;
	//	int oppdir = tk < 2 ? 1 : -1;   /**< 向对方前进的方向red -1 往上; blue +1 往下 */
	//	int dx = tank[tk].x, dy = tank[tk].y;
	//	int bc = 0;
	//	for (; dx < 9 && dx >= 0; dx += oppdir)    /**< 计算沿x方向前进到地图边界会碰到的砖块的数量 */
	//		if (board.b[dx][dy] != 0)
	//			bc++;
	//	dx -= oppdir;
	//	int centdir = dy > 4 ? -1 : 1;  /**< 向地图中间前进的方向 dy>4 -1往上; dy<=4 +1往下 */
	//	if (dy != 4) {
	//		for (dy += centdir; dy != 4; dy += centdir) /**< 计算沿y方向前进到地图中间会碰到的砖块数量 */
	//			if (board.b[dx][dy] != 0)
	//				bc++;
	//	}
	//	else bc++;
	//	return (tk < 2 ? 8 - tank[tk].x : tank[tk].x) + bc;
	//}

	/**< changed */
	bool hasObscale(const Coord& ta, const Coord& tb) {  /**< 判断tanka和tankb之间有没有东西挡子弹 */
		if (ta == tb) return 1;
		const Coord* a = &ta; const Coord* b = &tb;
		//if (a->x > b->x || a->y > b->y) std::swap(a, b);
		if (a->x == b->x) {
			if (a->y > b->y) std::swap(a, b);
			int dx = a->x, dy = a->y + 1;
			for (; dy != b->y; dy++)
				if (board.b[dx][dy] > 0)
					return 1;
		}
		else if (a->y == b->y) {
			if (a->x > b->x) std::swap(a, b);
			int dx = a->x + 1, dy = a->y;
			for (; dx != b->x; dx++)
				if (board.b[dx][dy] > 0)
					return 1;
		}
		return 0;	/*注意：不共线也会返回0*/
	}
	// added
	bool hasSteelObscale(const Coord& ta, const Coord& tb) {  /**< 判断tanka和tankb之间有没有钢块 */
		if (ta == tb) return 1;
		const Coord* a = &ta; const Coord* b = &tb;
		//if (a->x > b->x || a->y > b->y) std::swap(a, b);
		if (a->x == b->x) {
			if (a->y > b->y) std::swap(a, b);
			int dx = a->x, dy = a->y + 1;
			for (; dy != b->y; dy++)
				if (board.b[dx][dy] == 2)
					return 1;
		}
		else if (a->y == b->y) {
			if (a->x > b->x) std::swap(a, b);
			int dx = a->x + 1, dy = a->y;
			for (; dx != b->x; dx++)
				if (board.b[dx][dy] == 2)
					return 1;
		}
		return 0;	/*注意：不共线也会返回0*/
	}
	/**< changed */
	int countObscale(Coord& ta, Coord& tb) { /**< 计算tanka和tankb之间有多少东西能挡子弹，有什么鬼用？？ */
		if (ta == tb) return 9;	/*注意：重合也返回9*/
		Coord* a = &ta; Coord* b = &tb;
		if (a->x == b->x) {
			if (a->y > b->y) std::swap(a, b);
			int c = 0;
			int dx = a->x, dy = a->y + 1;
			for (; dy != b->y; dy++)
				if (board.b[dx][dy] > 0)
					c++;
			return c;
		}
		else if (a->y == b->y) {
			if (a->x > b->x) std::swap(a, b);
			int c = 0;
			int dx = a->x + 1, dy = a->y;
			for (; dx != b->x; dx++)
				if (board.b[dx][dy] > 0)
					c++;
			return c;
		}
		return 9;	/*注意：不共线会返回9*/
	}
	/**< changed */
	int firstObscale(int x, int y, int d) {    /**< 返回(x,y)坐标位置以dir方向第一个挡子弹的东西的类别 */
		if (d == 0) { //up
			for (x--; x >= 0 && board.b[x][y] <= 0; x--);
			if (x < 0) return -1;
			return board.b[x][y];
		}
		else if (d == 2) {
			for (x++; x < 9 && board.b[x][y] <= 0; x++);
			if (x == 9) return -1;
			return board.b[x][y];
		}
		else if (d == 3) {
			for (y--; y >= 0 && board.b[x][y] <= 0; y--);
			if (y < 0) return -1;
			return board.b[x][y];
		}
		else if (d == 1) {
			for (y++; y < 9 && board.b[x][y] <= 0; y++);
			if (y == 9) return -1;
			return board.b[x][y];
		}
		else {
			return -1;
		}
	}
	Coord firstObscalePos(const Coord& c, int d) { /**< 返回c位置在dir方向上第一个挡子弹的东西的位置，出界返回{-1,-1} */
		int x = c.x, y = c.y;
		if (d == 0) { //up
			for (x--; x >= 0 && board.b[x][y] <= 0; x--);
			if (x < 0) return { -1,-1 };
			return { x,y };
		}
		else if (d == 2) {
			for (x++; x < 9 && board.b[x][y] <= 0; x++);
			if (x == 9) return { -1,-1 };
			return { x,y };
		}
		else if (d == 3) {
			for (y--; y >= 0 && board.b[x][y] <= 0; y--);
			if (y < 0) return { -1,-1 };
			return { x,y };
		}
		else if (d == 1) {
			for (y++; y < 9 && board.b[x][y] <= 0; y++);
			if (y == 9) return { -1,-1 };
			return { x,y };
		}
		else {
			return { -1,-1 };
		}
	}

	//--- old help functions
	//float killereval(int tk) {   /**< 对killermove给出的步数评分 */
	//	if (dead[tk]) return 0;
	//	int dis = killerMoves(tk);
	//	switch (dis) {
	//	case 0: return 60;
	//	case 1: return 30;
	//	case 2: return 15;
	//	case 3: return 12;
	//	}
	//	if (dis < 8) return 16 - 2 * dis;
	//	return 15.0 / dis;
	//}

	//float roweval(int tk) {
	//	if (dead[tk]) return 0;
	//	return (tk < 2 ? tank[tk].x : 8 - tank[tk].x) * 0.5;
	//}

	//float shooteval(int tk) {    /**< 判断能不能被对方打到 */
	//	float w = 0;
	//	if (lastshoot[tk]) w = -2;
	//	if (tk < 2) {
	//		bool w1 = 0, w2 = 0;
	//		if (!lastshoot[2] && tank[tk].online(tank[2])
	//			&& !hasObscale(tank[tk], tank[2])) w1 = 1; /**< 能被对方0号打到 */
	//		if (!lastshoot[3] && tank[tk].online(tank[3])
	//			&& !hasObscale(tank[tk], tank[3])) w2 = 1; /**< 能被对方1号打到 */
	//		if (w1 && w2) return -25;
	//		if (w1 || w2) return w;
	//	}
	//	else {
	//		bool w1 = 0, w2 = 0;
	//		if (!lastshoot[0] && tank[tk].online(tank[0])
	//			&& !hasObscale(tank[tk], tank[0])) w1 = 1;
	//		if (!lastshoot[1] && tank[tk].online(tank[1])
	//			&& !hasObscale(tank[tk], tank[1])) w2 = 1;
	//		if (w1 && w2) return -25;
	//		if (w1 || w2) return w;
	//	}
	//	return 0;
	//}

	//float eval(int col) {    /**< 判断颜色为col的一方的得分 */
	//	float res1 = 0, res2 = 0, r0 = 0;   /**< res1蓝方，res2红方 */
	//	int end = judgeend();
	//	if (end == 1 || end == 2)   /**< Return game ended? 1: B win, 2:R win, 3: Draw */
	//		r0 = (end == col + 1 ? 1000 : -1000);
	//	if (dead[0] || dead[1]) res1 -= 50;
	//	if (dead[2] || dead[3]) res2 += 50;
	//	res1 += killereval(0);
	//	res1 += killereval(1);
	//	res2 -= killereval(2);
	//	res2 -= killereval(3);
	//	res1 += roweval(0);
	//	res1 += roweval(1);
	//	res2 -= roweval(2);
	//	res2 -= roweval(3);
	//	res1 += shooteval(0);
	//	res1 += shooteval(1);
	//	res2 -= shooteval(2);
	//	res2 -= shooteval(3);
	//	if (col == 0) return r0 + res1 + res2 * 0.9;
	//	return r0 + -res1 * 0.9 - res2;
	//}
	//

	bool __2shootdanger(int tk, Act d) {
		int op1, op2;    /**< 对方坦克编号 */
		if (tk < 2) op1 = 2, op2 = 3;
		else op1 = 0, op2 = 1;
		if (!dead[op1] && !dead[op2]) {
			if (!lastshoot[op1] && !lastshoot[op2]) {
				Coord c1 = ((d >= 0 && d < 4) ? (tank[tk] + dir[d]) : tank[tk]);
				if (c1.online(tank[op1]) && c1.online(tank[op2])) {
					board.b[tank[tk].x][tank[tk].y] -= 3;  board.b[c1.x][c1.y] += 3;	//这一步确实有必要
					bool ret = (!hasObscale(c1, tank[op1]) && !hasObscale(c1, tank[op2]));
					board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
					return ret;
				}
			}
		}
		return 0;
	}
	bool __1shootdanger(int tk, Act d) {
		int op1, op2;
		if (tk < 2) op1 = 2, op2 = 3;
		else op1 = 0, op2 = 1;
		Coord c1 = ((d >= 0 && d < 4) ? (tank[tk] + dir[d]) : tank[tk]);
		board.b[tank[tk].x][tank[tk].y] -= 3;  board.b[c1.x][c1.y] += 3;
		if (!dead[op1])
			if (!lastshoot[op1])
				if (c1.online(tank[op1]) && !hasObscale(c1, tank[op1])) {
					board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
					return 1;
				}
		if (!dead[op2])
			if (!lastshoot[op2])
				if (c1.online(tank[op2]) && !hasObscale(c1, tank[op2])) {
					board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
					return 1;
				}
		board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
		return 0;
	}
	bool __1shootdanger(int tk, Act d, int& dtk, int& ddir, bool noshoot = false) {	//noshoot怎么起作用？？ 似乎有了noshoot会更保守
		int op1, op2;
		if (tk < 2) op1 = 2, op2 = 3;
		else op1 = 0, op2 = 1;
		Coord c1 = ((d >= 0 && d < 4) ? (tank[tk] + dir[d]) : tank[tk]);
		board.b[tank[tk].x][tank[tk].y] -= 3;  board.b[c1.x][c1.y] += 3;
		if (!dead[op1] && (!lastshoot[op1] || noshoot))
			if (c1.online(tank[op1]) && !hasObscale(c1, tank[op1])) {
				board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
				dtk = op1; ddir = tank[op1].getdir(c1) + 4;
				return 1;
			}
		if (!dead[op2] && (!lastshoot[op2] || noshoot))
			if (c1.online(tank[op2]) && !hasObscale(c1, tank[op2])) {
				board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
				dtk = op2; ddir = tank[op2].getdir(c1) + 4;
				return 1;
			}
		board.b[tank[tk].x][tank[tk].y] += 3;  board.b[c1.x][c1.y] -= 3;
		return 0;
	}
	void lblKillerMove();
	void lblEnMove();
	void lbl22Pos();
	int __killdis(int col, int tx, int ty, bool isb[3], int r);
	int __killdis(int col, int tx, int ty);
	bool startJudge();
	Act startpolicy(int col, int lr);
	Act rushonly(int tk);
	Act rushwait(int tk);
	Act rushwait2(int tk);
	Act directShootJudge(int tk);
	Act lineShootJudge(int tk);
	Act staticPolicy(int tk);
	Act nearPolicy(int tk);
	Act crossrushPolicy(int col, int side, int& mstep, int& opstep);
	void crossrushDis(int col, int side, int& mstep, int& opstep);
	Act2 s4Policy(int col, int lstate, int rstate);
	Act2 mainPolicy(int col);
	Act2 mainPolicy_safe(int col);
	Act trymove(int tk, Act dir, bool needturn = 0);
	Act defendPolicy(int tk, bool noshootcheck = 0);
};

/*
the data structure of TrieTree
each TrieTree has a pointer which has 26 positions to place abcdefg……xyz.

and a count int tyoe to store the the number of strings with the common prefix,
from the root node to the current node's string.
*/

typedef struct Node
{
	struct Node *next[MAX][MAX];//一共九种情况的遍历，第一个是-1，也就是说索引是dir+1。第二个索引是敌方坦克的动作
	//There may be MAX characters below each node.
	int step;//初始应该是0
    int tk;
    int col;
	//The number of strings with the common prefix from the root node to the current node's string
	
    int score;//得分
	
    Coord tank_this[4];
    Coord kb_this[9][9]
	bool lastshoot_this[4];
	Act lastmove_this[4];
	bool dead_this[4];

    
}TrieNode,*TrieTree;

/*
create a TrieTree,which has only root node.
and we assign values to each child node.

return the root node.
*/

TrieTree create_rootTree(int step,int tk,int col,Coord tank_new[4],Coord kb_new[9][9],Coord lastshoot_new[4],Coord lastmove_new[4],bool dead_new[4])//输入正演的本回合信息
{
	TrieTree rootTree = (TrieTree)malloc(sizeof(TrieNode));
	rootTree->step=step;
    rootTree->tk=tk;
    rootTree->col=col;
    rootTree->count = 0;
	rootTree->score = 0;
	
	rootTree->tank_this=tank_new[4];
    rootTree->kb_this=kb_new;//本回合决策前的棋盘
    rootTree->lastshoot_this=lastshoot_new;//本回合决策前的上回合开枪状态
    rootTree->lastmove_this=lastmove_new;//本回合决策前的上回合移动状态；
    int i;int j;
	for(i=0;i<MAX;i++){
        for(j=0;j<MAX;j++){
            rootTree->next[i][j] = NULL;
        }
    }		
	return rootTree;
}

TrieTree can_do(TrieTree rootTree,i,j){//判断是否合法

    
}
TrieTree forward_Simulation(TrieTree rootTree)
{
    int step=(rootTree->step)+1;
    int tk=rootTree->tk;
    int col=rootTree->col;
    int i;int j;
	for(i=0;i<MAX;i++){
        for(j=0;j<MAX;j++){
            if can_do(rootTree,i,j){
                
                rootTree->next[i][j] = create_rootTree(step+1,);
            }
            
        }
    }		
}

/*
销毁穷举树
*/
void destroy_TrieTree(TrieTree rootTree)
{
	int i;int j;
	//递归释放每个节点的内存
	for(i=0;i<MAX;i++){
        for(j=0;j<MAx,j++){
		if(rootTree->next[i])
			destroy_TrieTree(rootTree->next[i]);
        }
    }
	free(rootTree);
}

//创建一个只有根节点的字典树
TrieTree create_rootTree();


//销毁字典树
void destroy_TrieTree(TrieTree);


int itssize(FILE *fp)
{
    // TODO
    char c;//用来循环的变量
    int h = 0;//用来计数的变量
    while((c = fgetc(fp)) != EOF)//文件终结是EOF

     {
        if(c == '\n')//如果检测到换行
        h++;//就计数+1

        else
        {
            c = fgetc(fp);//这是处理最后一行可能没有换行标志，但是文件却已经结束。

            if(c == EOF)
            {
                h++;//最后一行没有换行的话也可以检验一下
                break;
            }       //if的括号

        }          //else的括号

    }               //while的括号

    return h;

}                   //函数的括号

