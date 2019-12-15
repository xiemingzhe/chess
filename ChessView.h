// ChessView.h : interface of the CChessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHESSVIEW_H__09C6E7B6_DCA6_4E44_877E_A67FEB4739B3__INCLUDED_)
#define AFX_CHESSVIEW_H__09C6E7B6_DCA6_4E44_877E_A67FEB4739B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "extern.h"
#include "define1.h"
#include "define4.h"
#include "define5.h"
#include <STDIO.H>
//必须包含,迭代加深时需要时间控制
#include <TIME.H>
#include <WINDOWS.H>
#include <STDLIB.H>
#include <Mmsystem.h>
//水平选择对话框
#include "SetLevel.h"
//////////////////////////////////////////////////////////////////////////
#include "GeneratorMove.h"  //导入走法生成器
#include "SearchEngine.h"
#include "MainFrm.h"

//////////////////////////////////////////////////////////////////////////
//悔棋功能
static struct HistoryMovsStruct{
	int mv ;    //走法
	int pcsrc;  //起点棋子类型
	int pcdest; //终点棋子类型
}HistoryMovs[1024];
static int HistoryLen = 0;

// 置换表项结构
struct HashItem {
	BYTE ucDepth, ucFlag; //该表项求知时搜索的深度、标志（该表项值的类型）
	short svl;    //分值
	WORD wmv, wReserved; //最佳走法
	DWORD dwLock0, dwLock1;//校验码
};

// 开局库项结构
struct BookItem {
	DWORD dwLock;  //置换表中的校验码
	WORD wmv, wvl; //最佳走法和权重
};


// 与搜索有关的全局变量
static struct {
  int mvResult;                  // 电脑走的棋
  int nHistoryTable[65536];      // 历史表
  int mvKillers[LIMIT_DEPTH][2]; // 杀手走法表
  HashItem HashTable[HASH_SIZE]; // 置换表
  int nBookSize;                 // 开局库大小
  BookItem BookTable[BOOK_SIZE]; // 开局库
} Search;

////////临时局面结构//////////////////////////////////////////////////////////////////
// 局面结构
struct PositionStruct{
	BYTE ucpcSquares[256];   // 棋盘上的棋子
	int sdPlayer;
	int vlWhite, vlBlack;           // 红、黑双方的子力价值
	int nDistance1, nMoveNum;        // 距离根节点的步数，历史走法数
	ZobristStruct zobr;             // Zobrist
	MoveStruct mvsList[MAX_MOVES];  // 历史走法信息列表

	// 清空棋盘
	void ClearBoard(void) {         
		sdPlayer = vlWhite = vlBlack = nDistance1 = 0;
		memset(ucpcSquares, 0, 256);
	    zobr.InitZero();
	}

	//3.在棋盘上放一枚棋子
	void AddPiece(int sq, int pc) { 
		ucpcSquares[sq] = pc;
		// 红方加分，黑方(注意"cucvlPiecePos"取值要颠倒)减分
		if (pc < 16) {
			vlWhite += cucvlPiecePos[pc - 8][sq];
			zobr.Xor(Zobrist.Table[pc - 8][sq]);
		} else {
			vlBlack += cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
			zobr.Xor(Zobrist.Table[pc - 9][sq]);
		}
	}

	//2.交换走子方
	void ChangeSide(void) {        
		sdPlayer = 1 - sdPlayer ;
		//走棋方的随机数：即使是同一局面，但是对于不同走棋方意义完全不同
		zobr.Xor(Zobrist.Player); //走子方，走过棋子后就异或运算，然后就是对应对手的局面了。       
	}
	// 清空(初始化)历史走法信息
	void SetIrrev(void) {           
		mvsList[0].Set(0, 0, Checked(), zobr.dwKey);
		nMoveNum = 1;  //历史走法置为1
	}

	// 11.判断是否被对方将军
	BOOL Checked() const {
		int i, j, sqSrc, sqDst;
		int pcSelfSide, pcOppSide, pcDst, nDelta;
		pcSelfSide = SIDE_TAG(sdPlayer);
		pcOppSide = OPP_SIDE_TAG(sdPlayer);
		// 找到棋盘上的帅(将)，再做以下判断：
		
		for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
			if (ucpcSquares[sqSrc] != pcSelfSide + PIECE_KING) {
				continue;
			}
			
			// 1. 判断是否被对方的兵(卒)将军
			if (ucpcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) {
				return TRUE;
			}
			for (nDelta = -1; nDelta <= 1; nDelta += 2) {
				if (ucpcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) {
					return TRUE;
				}
			}
			
			// 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
			for (i = 0; i < 4; i ++) {
				if (ucpcSquares[sqSrc + ccAdvisorDelta[i]] != 0) {
					continue;
				}
				for (j = 0; j < 2; j ++) {
					pcDst = ucpcSquares[sqSrc + ccKnightCheckDelta[i][j]];
					if (pcDst == pcOppSide + PIECE_KNIGHT) {
						return TRUE;
					}
				}
			}
			
			// 3. 判断是否被对方的车或炮将军(包括将帅对脸)
			for (i = 0; i < 4; i ++) {
				nDelta = ccKingDelta[i];
				sqDst = sqSrc + nDelta;
				while (IN_BOARD(sqDst)) {
					pcDst = ucpcSquares[sqDst];
					if (pcDst != 0) {
						if (pcDst == pcOppSide + PIECE_ROOK || pcDst == pcOppSide + PIECE_KING) {
							return TRUE;
						}
						break;
					}
					sqDst += nDelta;
				}
				sqDst += nDelta;
				while (IN_BOARD(sqDst)) {
					int pcDst = ucpcSquares[sqDst];
					if (pcDst != 0) {
						if (pcDst == pcOppSide + PIECE_CANNON) {
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
};

//////////////////////////////////////////////////////////////////////////
class CChessView : public CView
{
public:
	GeneratorMove gen ;
    int setTime ;  //电脑搜索的时间

protected: // create from serialization only
	CChessView();
	DECLARE_DYNCREATE(CChessView)

// Attributes
public:
	CChessDoc* GetDocument();
	//MoveGenerator mg ; 
	CDC     MemDC ;        //设备环境类
    CBitmap bmpBoard;      //象棋背景
    CBitmap bmpPieces[24]; //棋子位图集合
	CBitmap bmpSelected ;  //选中棋子标志位图

	bool    bFlipped ;   //是否翻转
	static int  sdPlayer ;   //轮到谁0 红 1 黑
	int     sqSelected, mvLast; //选中的格子,上一步棋
	static  byte ucpcSquares[256];

	//3.////////////////////////////////////////////////////////////////////////
	int vlWhite, vlBlack;           // 红、黑双方的子力价值
    static int nDistance;           // 距离根节点的步数

// Operations
public:
	HINSTANCE hInst; 
	// 初始化棋盘信息
	void  Startup(void);
	// 交换走子方
	void  ChangeSide(void);      
	// 在棋盘上放一枚棋子
	void  AddPiece(int sq, int pc) ; 
	// 从棋盘上拿走一枚棋子
	void  DelPiece(int sq , int pc) ;       
	// 搬一步棋的棋子
	int  MovePiece(int mv) ;       
	// 走一步棋
	BOOL MakeMove(int mv); 
	
	void UndoMovePiece(int mv, int pcCaptured); // 撤消搬一步棋的棋子

    BOOL IsMate(void);                          // 判断是否被杀


	//////////////////////////
    void DrawSquare(int,BOOL); 
	void ClickSquare(int);
	//3////////////////////////////////////////////////////////////////////////
	int Evaluate(void) const ;
    void UndoMakeMove();  // 撤消走一步棋

    void SearchMain(void) ;  //迭代加深搜索过程
    int SearchFull(int vlAlpha, int vlBeta, int nDepth,BOOL bNoNull);// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
	static int CompareHistory(const void *lpmv1, const void *lpmv2);// "qsort"按历史表排序的比较函数
	void ResponseMove(void); //电脑回应一步棋
//4////////////////////////////////////////////////////////////////////////
public:
	 int  nMoveNum;                  // 历史走法数
	 MoveStruct mvsList[MAX_MOVES];  // 历史走法信息列表
     ZobristStruct zobr;             // Zobrist 局面校验码，可以理解为当前局面校验码
	 BOOL bGameOver;                  //是否游戏结束(不让继续玩下去)

public:
	// 清空(初始化)历史走法信息
	void SetIrrev(void) {           
		mvsList[0].Set(0, 0, gen.Checked(ucpcSquares,sdPlayer), zobr.dwKey);
		nMoveNum = 1;  //历史走法置为1
	}
	BOOL InCheck(void) const {      // 是否被将军
		return mvsList[nMoveNum - 1].ucbCheck;
	}
	BOOL Captured(void) const {     // 上一步是否吃子
		return mvsList[nMoveNum - 1].ucpcCaptured != 0;
	}

	void NullMove(void) {                       // 走一步空步
		DWORD dwKey;
		dwKey = zobr.dwKey;
		ChangeSide();
		mvsList[nMoveNum].Set(0, 0, FALSE, dwKey);
		nMoveNum ++;
		nDistance ++;
	}

	void UndoNullMove(void) {                   // 撤消走一步空步
		nDistance --;
		nMoveNum --;
		ChangeSide();
	}

	int RepStatus(int nRecur = 1) const ;       // 检测重复局面
	int RepValue(int nRepStatus) const {        // 重复局面分值
		int vlReturn;
		vlReturn = ((nRepStatus & 2) == 0 ? 0 : nDistance - MATE_VALUE) +
			((nRepStatus & 4) == 0 ? 0 : MATE_VALUE - nDistance);
		return vlReturn == 0 ? -DRAW_VALUE : vlReturn;
	}
	BOOL NullOkay(void) const {                 // 判断是否允许空步裁剪
		return (sdPlayer == 0 ? vlWhite : vlBlack) > NULL_MARGIN;
	}
    
	// 播放资源声音
	 void PlayResWav(int nResId) {
		HINSTANCE hins = AfxGetInstanceHandle();
		PlaySound(MAKEINTRESOURCE(nResId), NULL, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
	}

	// 求MVV/LVA值
	 static int MvvLva(int mv) {
		return (cucMvvLva[ucpcSquares[DST(mv)]] << 3) - cucMvvLva[ucpcSquares[SRC(mv)]];
	}
	// "qsort"按MVV/LVA值排序的比较函数
	 static int CompareMvvLva(const void *lpmv1, const void *lpmv2) {
		 return MvvLva(*(int *) lpmv2) - MvvLva(*(int *) lpmv1);
	}


	// 静态(Quiescence)搜索过程
   int SearchQuiesc(int vlAlpha, int vlBeta);
   //////////////////////////////////////////////////////////////////////////


   // 和棋分值
   int DrawValue(void) const {                 
	   return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
   }
   // 提取置换表项
   int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv) ;
   // 保存置换表项
   void RecordHash(int nFlag, int vl, int nDepth, int mv) ;

   // 走法排序结构
   struct SortStruct{
	   int mvHash, mvKiller1, mvKiller2; // 置换表走法和两个杀手走法
	   int nPhase, nIndex, nGenMoves;    // 当前阶段，当前采用第几个走法，总共有几个走法
	   int mvs[MAX_GEN_MOVES];           // 所有的走法
	   GeneratorMove g ;
	   void Init(int mvHash_){ // 初始化，设定置换表走法和两个杀手走法
		   mvHash = mvHash_;
		   mvKiller1 = Search.mvKillers[nDistance][0];
		   mvKiller2 = Search.mvKillers[nDistance][1];
           nPhase = PHASE_HASH;
	   }
	   // 得到下一个走法
	   int Next(){
		   int mv;
		   switch (nPhase) {
			   // "nPhase"表示着法启发的若干阶段，依次为：
			   
			   // 0. 置换表着法启发，完成后立即进入下一阶段；
		   case PHASE_HASH:
			   nPhase = PHASE_KILLER_1;
			   if (mvHash != 0) {
				   return mvHash;
			   }
			   // 技巧：这里没有"break"，表示"switch"的上一个"case"执行完后紧接着做下一个"case"，下同
			   
			   // 1. 杀手着法启发(第一个杀手着法)，完成后立即进入下一阶段；
		   case PHASE_KILLER_1:
			   nPhase = PHASE_KILLER_2;
			   if (mvKiller1 != mvHash && mvKiller1 != 0 && g.LegalMove(ucpcSquares,sdPlayer,mvKiller1)) {
				   return mvKiller1;
			   }
			   
			   // 2. 杀手着法启发(第二个杀手着法)，完成后立即进入下一阶段；
		   case PHASE_KILLER_2:
			   nPhase = PHASE_GEN_MOVES;
			   if (mvKiller2 != mvHash && mvKiller2 != 0 && g.LegalMove(ucpcSquares,sdPlayer,mvKiller2)) {
				   return mvKiller2;
			   }
			   
			   // 3. 生成所有着法，完成后立即进入下一阶段；
		   case PHASE_GEN_MOVES:
			   nPhase = PHASE_REST;
			   nGenMoves = g.GenerateMoves(ucpcSquares,sdPlayer,mvs);
			   qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
			   nIndex = 0;
			   
			   // 4. 对剩余着法做历史表启发；
		   case PHASE_REST:
			   while (nIndex < nGenMoves) {
				   mv = mvs[nIndex];
				   nIndex ++;
				   if (mv != mvHash && mv != mvKiller1 && mv != mvKiller2) {
					   return mv;
				   }
			   }	
			   // 5. 没有着法了，返回零。
		   default:
			   return 0;
		   }
	   }
	};

	   // 对最佳走法的处理
	   static void SetBestMove(int mv, int nDepth) {
		   int *lpmvKillers;
		   Search.nHistoryTable[mv] += nDepth * nDepth;
		   lpmvKillers = Search.mvKillers[nDistance];
		   if (lpmvKillers[0] != mv) {
			   lpmvKillers[1] = lpmvKillers[0];
			   lpmvKillers[0] = mv;
		   }
	   }
//////////////////////////////////////////////////////////////////////////
	   void ClearBoard(void) {         // 清空棋盘
		   sdPlayer = vlWhite = vlBlack = nDistance = 0;
		   memset(ucpcSquares, 0, 256);
		   zobr.InitZero();
	   }

	   void Mirror(PositionStruct &posMirror) ; // 对局面镜像
	   // 装入开局库
	    void LoadBook(void) {
		   HINSTANCE hins = AfxGetInstanceHandle();
		   HRSRC hrsrc;

		   hrsrc = FindResource(hins, MAKEINTRESOURCE(IDR_DATA1), RT_RCDATA);
		   Search.nBookSize = SizeofResource(hins, hrsrc) / sizeof(BookItem);
		   /*
		   CString s ;
		   s.Format("%d",Search.nBookSize);
		   //MessageBox(s);
		   */
		   if (Search.nBookSize > BOOK_SIZE) {
			   Search.nBookSize = BOOK_SIZE;
		   }
		   memcpy(Search.BookTable, LockResource(LoadResource(hins, hrsrc)),
			   Search.nBookSize * sizeof(BookItem));
	   }

		static int CompareBook(const void *lpbk1, const void *lpbk2) {
			DWORD dw1, dw2;
			dw1 = ((BookItem *) lpbk1)->dwLock;
			dw2 = ((BookItem *) lpbk2)->dwLock;
			return dw1 > dw2 ? 1 : dw1 < dw2 ? -1 : 0;
		}


		// 搜索开局库
		 int SearchBook(void) {
			int i, vl, nBookMoves, mv;
			int mvs[MAX_GEN_MOVES], vls[MAX_GEN_MOVES];
			BOOL bMirror;
			BookItem bkToSearch, *lpbk;
			PositionStruct posMirror;
			// 搜索开局库的过程有以下几个步骤
			
			// 1. 如果没有开局库，则立即返回
			if (Search.nBookSize == 0) {
				return 0;
			}
			// 2. 搜索当前局面
			bMirror = FALSE;
			bkToSearch.dwLock = zobr.dwLock1;
			lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
			// 3. 如果没有找到，那么搜索当前局面的镜像局面
			if (lpbk == NULL) {
				bMirror = TRUE;
				Mirror(posMirror);
				bkToSearch.dwLock = posMirror.zobr.dwLock1;
				lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
			}
			// 4. 如果镜像局面也没找到，则立即返回
			if (lpbk == NULL) {
				return 0;
			}
			// 5. 如果找到，则向前查第一个开局库项
			while (lpbk >= Search.BookTable && lpbk->dwLock == bkToSearch.dwLock) {
				lpbk --;
			}
			lpbk ++;
			// 6. 把走法和分值写入到"mvs"和"vls"数组中
			vl = nBookMoves = 0;
			while (lpbk < Search.BookTable + Search.nBookSize && lpbk->dwLock == bkToSearch.dwLock) {
				mv = (bMirror ? MIRROR_MOVE(lpbk->wmv) : lpbk->wmv);
				if (gen.LegalMove(ucpcSquares,sdPlayer,mv)) {
					mvs[nBookMoves] = mv;
					vls[nBookMoves] = lpbk->wvl;
					vl += vls[nBookMoves];
					nBookMoves ++;
					if (nBookMoves == MAX_GEN_MOVES) {
						break; // 防止"BOOK.DAT"中含有异常数据
					}
				}
				lpbk ++;
			}
			if (vl == 0) {
				return 0; // 防止"BOOK.DAT"中含有异常数据
			}
			// 7. 根据权重随机选择一个走法
			vl = rand() % vl;
			for (i = 0; i < nBookMoves; i ++) {
				vl -= vls[i];
				if (vl < 0) {
					break;
				}
			}
			return mvs[i];
		}

		 // 根节点的Alpha-Beta搜索过程
     int CChessView::SearchRoot(int nDepth) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CChessView)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNewGame();
	afx_msg void OnComputerRed();
	afx_msg void OnComputerBlack();
	afx_msg void OnFilp();
	afx_msg void OnBack();
	afx_msg void OnSetLevel();
	afx_msg void OnLight();
	afx_msg void OnModerate();
	afx_msg void OnStrong();
	afx_msg void OnUpdateSetlevel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStrong(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModerate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLight(CCmdUI* pCmdUI);
	afx_msg void OnOneHorse();
	afx_msg void OnNinePiece();
	afx_msg void OnTowHorse();
	afx_msg void OnUpdateOnehorse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNinepiece(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTowhorse(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ChessView.cpp
inline CChessDoc* CChessView::GetDocument()
   { return (CChessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSVIEW_H__09C6E7B6_DCA6_4E44_877E_A67FEB4739B3__INCLUDED_)
