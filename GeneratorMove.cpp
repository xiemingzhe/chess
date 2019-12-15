// GeneratorMove.cpp: implementation of the GeneratorMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chess.h"
#include "GeneratorMove.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GeneratorMove::GeneratorMove()
{

}
GeneratorMove::~GeneratorMove()
{

}
//==================================================================
//函数名: GenerateMoves
//作  者: 谢明哲
//日  期: 2013年1月1日
//功  能: 生成所有合法走法
//输入参数: ucpcSquares输入的局面
//          sdPlayer轮到谁走棋 0:红 1:黑
//          mvs用来存放走法的数组
//          bCapture是否只产生吃子标记 
//返回值:   走法总数nGenMoves  走法mvs 
//说  明:   走法产生对轮到谁走棋很敏感
//==================================================================
int GeneratorMove::GenerateMoves(byte ucpcSquares[256], int sdPlayer, int *mvs, BOOL bCapture){
	int i, j, nGenMoves, nDelta, sqSrc, sqDst;
	int pcSelfSide, pcOppSide, pcSrc, pcDst;
	// 生成所有走法，需要经过以下几个步骤：

	nGenMoves = 0;                          //初始化走法总数
	pcSelfSide = SIDE_TAG(sdPlayer);        //获得本方标记 红:8 黑:16 
	pcOppSide  = OPP_SIDE_TAG(sdPlayer);    //获得对方标记 红:8 黑:16 
	for (sqSrc = 0; sqSrc < 256; sqSrc++) {
		
		// 1. 找到一个本方棋子:棋盘编号和本方标记进行与运算,(pcSrc&pcSelfSide!=0)说明是本方棋子
		pcSrc = ucpcSquares[sqSrc];
		if ((pcSrc & pcSelfSide) == 0) {
			continue;
		}
		
		// 2. 根据棋子类型确定走法
		switch (pcSrc - pcSelfSide) {     //棋子编号 0:将 1:士 2:象 3:马 4:车 5:炮 6:卒
		case PIECE_KING:  //将/帅
			for (i = 0; i < 4; i ++) {    //有4个可以移动到方向
				sqDst = sqSrc + ccKingDelta[i]; //原位置上加指定步长
				if (!IN_FORT(sqDst)) {          //目标位置不在九宫格内，说明越界
					continue;
				}
				//到这里说明目标位置合法
				pcDst = ucpcSquares[sqDst];  //记录目标位置的棋盘编号
				//bCapture为true时，pcDst & pcOppSide)!=0说明目标位置是对手的棋子,吃子走法
				//bCapture为false时,pcDst & pcSelfSide即只要目标位置不是自己的棋子都可以走
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
					mvs[nGenMoves] = MOVE(sqSrc, sqDst); //走法起点和终点加入走法数组中
					nGenMoves ++;    //走法数加一
				}
			}
			break;
		case PIECE_ADVISOR:  //  士/仕
			for (i = 0; i < 4; i ++) { //移动方向最多有4种
				sqDst = sqSrc + ccAdvisorDelta[i]; // 原位置加指定步长
				if (!IN_FORT(sqDst)) {  //目标位置如果是否在九宫格中
					continue;
				}
				pcDst = ucpcSquares[sqDst]; //目标位置棋盘编号
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
					mvs[nGenMoves] = MOVE(sqSrc, sqDst);
					nGenMoves ++;
				}
			}
			break;
		case PIECE_BISHOP: // 象/相
			for (i = 0; i < 4; i ++) { // 移动方向最多有4种
				sqDst = sqSrc + ccAdvisorDelta[i]; //技巧: 象的一次移动可以看成是士的两次同方向移动
				//目标位置在棋盘中&目标位置未过河&目标位置没有棋子(象眼)
				if (!(IN_BOARD(sqDst) && HOME_HALF(sqDst, sdPlayer) && ucpcSquares[sqDst] == 0)) {
					continue;
				}
				sqDst += ccAdvisorDelta[i];  //再增加一次步长(象的真正目标位置)
				pcDst = ucpcSquares[sqDst];  //目标位置棋盘编号
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
					mvs[nGenMoves] = MOVE(sqSrc, sqDst);
					nGenMoves ++;
				}
			}
			break;
		case PIECE_KNIGHT:  //  马
			//马有4个马腿8种移动方向
			for (i = 0; i < 4; i ++) {  //循环每个马马腿
				sqDst = sqSrc + ccKingDelta[i]; //马腿的步长和将/帅步长相同
				if (ucpcSquares[sqDst] != 0) {  //马腿位置有棋子则直接返回
					continue;
				}
				for (j = 0; j < 2; j ++) {  //每个马腿对应两种走法
					sqDst = sqSrc + ccKnightDelta[i][j]; //原位置加上马腿对应的移动方向的走法步长
					if (!IN_BOARD(sqDst)) {  //目标位置不在棋盘中，直接返回
						continue;
					}
					pcDst = ucpcSquares[sqDst]; //获得目标位置棋子标识
				   //bCapture为true时，pcDst & pcOppSide)!=0说明目标位置是对手的棋子,吃子走法
				   //bCapture为false时,pcDst & pcSelfSide即只要目标位置不是自己的棋子都可以走
					if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
						mvs[nGenMoves] = MOVE(sqSrc, sqDst);  //把走法添加到走法队列中
						nGenMoves ++;
					}
				}
			}
			break;
		case PIECE_ROOK:  //  车 
			for (i = 0; i < 4; i ++) {    //有4种可以移动的方向
				nDelta = ccKingDelta[i];  //用将对步长作为车每个方向的一个步长
				sqDst = sqSrc + nDelta;   //在i方向移动一个步长
				while (IN_BOARD(sqDst)) { //步长在棋盘中
					pcDst = ucpcSquares[sqDst]; //目标位置棋盘编号
					if (pcDst == 0) {    //目标位置没有棋子，不可能是吃子走法
						if (!bCapture) { //参数要求不需要只产生吃子走法
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					} else {  //目标位置有棋子
						if ((pcDst & pcOppSide) != 0) { //是对方的棋子，吃子走法(无论参数是否要求这都是要记录的走法)
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
						break;
					}
					sqDst += nDelta;   //再移动一个步长，进行同样操作
				}
			}
			break;
		case PIECE_CANNON:  // 炮/砲
			for (i = 0; i < 4; i ++) {   //有4个可以移动方向
				nDelta = ccKingDelta[i];
				sqDst = sqSrc + nDelta;  //同车一样，移动一个步长
				while (IN_BOARD(sqDst)) {
					pcDst = ucpcSquares[sqDst];
					if (pcDst == 0) { //目标位置没有棋子
						if (!bCapture) { //正好参数不要求只产生吃子走法，记录走法
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					} else {  //如果目标位置有棋子(炮台)，跳出循环
						break;
					}
					sqDst += nDelta;
				}
				//已经有炮台了，现在继续移动
				sqDst += nDelta;  //移动一个步长
				while (IN_BOARD(sqDst)) { 
					pcDst = ucpcSquares[sqDst];
					if (pcDst != 0) {  //又遇到一个棋子
						if ((pcDst & pcOppSide) != 0) { //棋子是对方的，记录走法
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
						break;   //直接跳出
					}
					sqDst += nDelta;
				}
			}
			break;
		case PIECE_PAWN:  // 卒/兵
			sqDst = SQUARE_FORWARD(sqSrc, sdPlayer); // 上来就前进一步(无论是否过河都可以前进)
			if (IN_BOARD(sqDst)) {  //目标位置在棋盘中
				pcDst = ucpcSquares[sqDst]; //取出目标位置棋盘编号
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) { //根据是否要求只产生吃子走法，进行记录
					mvs[nGenMoves] = MOVE(sqSrc, sqDst);
					nGenMoves ++;
				}
			}
			// 过河
			if (AWAY_HALF(sqSrc, sdPlayer)) {
				for (nDelta = -1; nDelta <= 1; nDelta += 2) { //过河后多出2种走法，左、右
					sqDst = sqSrc + nDelta;   //加上指定步长
					if (IN_BOARD(sqDst)) {    //目标位置在棋盘中
						pcDst = ucpcSquares[sqDst];  //目标位置棋盘编号
						if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					}
				}
			}
			break;
    }
  }
  return nGenMoves;  //返回走法总数
}

//==================================================================
//函数名: LegalMove
//作  者: 谢明哲
//日  期: 2013年1月1日
//功  能: 判断走法是否合理
//输入参数: ucpcSquares 判断的局面
//          sdPlayer轮到谁走棋 0:红 1:黑
//          mvs用来存放走法的数组
//返回值:   BOOL类型 TRUE合法 FALSE不合法 
//说  明:   走法产生对轮到谁走棋很敏感
//==================================================================
BOOL GeneratorMove::LegalMove(byte ucpcSquares[256],int sdPlayer, int mv)  {
	int sqSrc, sqDst, sqPin;
	int pcSelfSide, pcSrc, pcDst, nDelta;
	// 判断走法是否合法，需要经过以下的判断过程：
	
	// 1. 判断起始格是否有自己的棋子


	sqSrc = SRC(mv);
	//获得起始位置棋盘编号
	pcSrc = ucpcSquares[sqSrc];  
	//得到本方标记 红:8 黑:16  
	pcSelfSide = SIDE_TAG(sdPlayer); 
	//如果起点位置不是自己的棋子,返回FALSE
	if ((pcSrc & pcSelfSide) == 0) { 
		return FALSE;
	}
	
	// 2. 判断目标位置是否有自己的棋子
	sqDst = DST(mv);
	pcDst = ucpcSquares[sqDst];
	if ((pcDst & pcSelfSide) != 0) { //目标位置是自己的棋子，返回FALSE
		return FALSE;
	}
	
	// 3. 根据棋子的类型判断走法是否合法
	switch (pcSrc - pcSelfSide) {  //获得棋子编号
	case PIECE_KING:   // 将
		return IN_FORT(sqDst) && KING_SPAN(sqSrc, sqDst);   //在九宫格中&符合将步长
	case PIECE_ADVISOR:// 士
		return IN_FORT(sqDst) && ADVISOR_SPAN(sqSrc, sqDst);//在九宫格中&符合士步长
	case PIECE_BISHOP: // 像
		//在河同一边&符合象的步长&象眼处没有棋子
		return SAME_HALF(sqSrc, sqDst) && BISHOP_SPAN(sqSrc, sqDst) &&
			ucpcSquares[BISHOP_PIN(sqSrc, sqDst)] == 0;
	case PIECE_KNIGHT: // 马
		sqPin = KNIGHT_PIN(sqSrc, sqDst); //如果走法合法，返回马腿位置，否则返回起点位置
		return sqPin != sqSrc && ucpcSquares[sqPin] == 0; //未返回起点位置，则返回马腿位置&马腿位置没有棋子
	case PIECE_ROOK:   // 车
	case PIECE_CANNON: // 炮
		if (SAME_RANK(sqSrc, sqDst)) { //如果是同一行，确定步长
			nDelta = (sqDst < sqSrc ? -1 : 1);
		} else if (SAME_FILE(sqSrc, sqDst)) {  //如果是同一列，确定步长
			nDelta = (sqDst < sqSrc ? -16 : 16);
		} else {  //既不是同一行也不是用一列，非法走法
			return FALSE;
		}
		//车和炮在不吃子的情况下走法是一样的
		sqPin = sqSrc + nDelta;  //加上指定步长
		while (sqPin != sqDst && ucpcSquares[sqPin] == 0) { //未到目标位置&未遇到其它棋子
			sqPin += nDelta;
		}
		if (sqPin == sqDst) {   //while循环因到达终点位置而结束
			//第2步已经判断了，目标位置若是自己的棋子，程序根本走不到这里
			return pcDst == 0 || pcSrc - pcSelfSide == PIECE_ROOK; //终点位置没有棋子||有棋子(肯定是对手的棋子)，但起点棋子是车(吃子)。
		} else if (pcDst != 0 && pcSrc - pcSelfSide == PIECE_CANNON) { //未到终点位置的前提下(肯定是因为遇到棋子了while结束了)，终点位置有棋子(对方的)&起点棋子是炮
			sqPin += nDelta;  //加上指定步长
			while (sqPin != sqDst && ucpcSquares[sqPin] == 0) { //继续扫描，是否还有其它棋子
				sqPin += nDelta;
			}
			return sqPin == sqDst; //为TRUE说明到扫描到终点位置，也没有再发现棋子，也即是炮台合法
		} else {  //其它情况，都是非法的
			return FALSE;
		}
	case PIECE_PAWN:  //  卒/兵
		//过河&是向左或者右走了一步，合法。
		if (AWAY_HALF(sqDst, sdPlayer) && (sqDst == sqSrc - 1 || sqDst == sqSrc + 1)) {
			return TRUE;
		}
		return sqDst == SQUARE_FORWARD(sqSrc, sdPlayer);  //无论过河与否，只要是向前走一步，都是合法的。
	default: //否则返回FALSE
		return FALSE;
	}
}

/*
  其实判断帅(将)是否被将军的过程并不复杂：
  (1) 假设帅(将)是车，判断它是否能吃到对方的车和将(帅)(中国象棋中有将帅不能对脸的规则)；
  (2) 假设帅(将)是炮，判断它是否能吃到对方的炮；
  (3) 假设帅(将)是马，判断它是否能吃到对方的马，需要注意的是，帅(将)的马腿用的数组是 ccAdvisorDelta，而不是 ccKingDelta；
  (4) 假设帅(将)是过河的兵(卒)，判断它是否能吃到对方的卒(兵)。
*/
//==================================================================
//函数名: Checked
//作  者: 谢明哲
//日  期: 2013年1月1日
//功  能: 判断是否处于被将军的状态
//输入参数: ucpcSquares 判断的局面
//          sdPlayer轮到谁走棋 0:红 1:黑
//          mvs用来存放走法的数组
//返回值:   BOOL类型 TRUE合法 FALSE不合法 
//说  明:   走法产生对轮到谁走棋很敏感
//==================================================================
BOOL GeneratorMove::Checked(byte ucpcSquares[256],int sdPlayer) {
	int i, j, sqSrc, sqDst;
	int pcSelfSide, pcOppSide, pcDst, nDelta;
	pcSelfSide = SIDE_TAG(sdPlayer);
	pcOppSide = OPP_SIDE_TAG(sdPlayer);
	// 找到棋盘上的帅(将)，再做以下判断：
	
	for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
		if (ucpcSquares[sqSrc] != pcSelfSide + PIECE_KING) {  //找到本方的将帅
			continue;
		}
		
		// 1. 判断是否被对方的兵(卒)将军
		if (ucpcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) { //如果将是卒，前进一步看是否可以到达对方卒的位置
			return TRUE;
		}
		for (nDelta = -1; nDelta <= 1; nDelta += 2) {
			if (ucpcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) { //如果将是卒，左、右走一步看是否可以到达对方卒的位置
				return TRUE;
			}
		}
		
		// 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
		for (i = 0; i < 4; i ++) {
			//如果将是对方的马，那马腿的位置就不能按对方的马腿找了，而是按士的步长作为马腿位置(技巧)。
			if (ucpcSquares[sqSrc + ccAdvisorDelta[i]] != 0) { 
				continue;
			}
			for (j = 0; j < 2; j ++) {
				pcDst = ucpcSquares[sqSrc + ccKnightCheckDelta[i][j]];
				if (pcDst == pcOppSide + PIECE_KNIGHT) {    //如果将是马，看是否可以到达对方马的位置
					return TRUE;
				}
			}
		}
		// 3. 判断是否被对方的车或炮将军(包括将帅对脸)
		for (i = 0; i < 4; i ++) {    //4个方向
			nDelta = ccKingDelta[i];  //步长
			sqDst = sqSrc + nDelta;
			while (IN_BOARD(sqDst)) {  //寻找对放的车
				pcDst = ucpcSquares[sqDst];
				if (pcDst != 0) {  //目标位置有棋子，是对方的将(对脸)或者是对方的车，都是处于将车状态
					if (pcDst == pcOppSide + PIECE_ROOK || pcDst == pcOppSide + PIECE_KING) {
						return TRUE;
					}
					break; //有其它棋子，我们不感兴趣，直接返回
				}
				sqDst += nDelta;
			}
			//while可能因为越界而终止，我们感兴趣的是因为遇到棋子而终止了(炮台)
			sqDst += nDelta;
			while (IN_BOARD(sqDst)) {
				int pcDst = ucpcSquares[sqDst];
				if (pcDst != 0) {
					if (pcDst == pcOppSide + PIECE_CANNON) {  //继续扫描时，找到对方炮了
						return TRUE;
					}
					break;
				}
				sqDst += nDelta;
			}
		}
		return FALSE;
	}
  return FALSE;
}
