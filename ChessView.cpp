// ChessView.cpp : implementation of the CChessView class
//左视图实现

#include "stdafx.h"
#include "Chess.h"

#include "ChessDoc.h"
#include "ChessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


byte CChessView::ucpcSquares[256] ={0}; //静态数组初始化
int CChessView::nDistance =  0 ;
int CChessView::sdPlayer =   0 ;
/////////////////////////////////////////////////////////////////////////////
// CChessView

IMPLEMENT_DYNCREATE(CChessView, CView)

BEGIN_MESSAGE_MAP(CChessView, CView)
	//{{AFX_MSG_MAP(CChessView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FILE_NEW, OnNewGame)
	ON_COMMAND(ID_COM_RED, OnComputerRed)
	ON_COMMAND(ID_COM_BLACK, OnComputerBlack)
	ON_COMMAND(ID_FILP, OnFilp)
	ON_COMMAND(ID_BACK, OnBack)
	ON_COMMAND(ID_SETLEVEL, OnSetLevel)
	ON_COMMAND(ID_LIGHT, OnLight)
	ON_COMMAND(ID_MODERATE, OnModerate)
	ON_COMMAND(ID_STRONG, OnStrong)
	ON_UPDATE_COMMAND_UI(ID_SETLEVEL, OnUpdateSetlevel)
	ON_UPDATE_COMMAND_UI(ID_STRONG, OnUpdateStrong)
	ON_UPDATE_COMMAND_UI(ID_MODERATE, OnUpdateModerate)
	ON_UPDATE_COMMAND_UI(ID_LIGHT, OnUpdateLight)
	ON_COMMAND(ID_ONEHORSE, OnOneHorse)
	ON_COMMAND(ID_NINEPIECE, OnNinePiece)
	ON_COMMAND(ID_TOWHORSE, OnTowHorse)
	ON_UPDATE_COMMAND_UI(ID_ONEHORSE, OnUpdateOnehorse)
	ON_UPDATE_COMMAND_UI(ID_NINEPIECE, OnUpdateNinepiece)
	ON_UPDATE_COMMAND_UI(ID_TOWHORSE, OnUpdateTowhorse)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChessView construction/destruction

CChessView::CChessView()
{
	// TODO: add construction code here

}

CChessView::~CChessView()
{
}

BOOL CChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx=520;
	cs.cy=680;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChessView drawing

void CChessView::OnDraw(CDC* pDC)
{	
	CChessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CChessView printing

BOOL CChessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CChessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CChessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CChessView diagnostics

#ifdef _DEBUG
void CChessView::AssertValid() const
{
	CView::AssertValid();
}

void CChessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CChessDoc* CChessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChessDoc)));
	return (CChessDoc*)m_pDocument;
}
#endif

void CChessView::OnPaint() 
{
	CPaintDC pDC(this); // device context for painting
    OnPrepareDC(&pDC);
	// TODO: Add your message handler code here

	//绘制棋盘
	MemDC.CreateCompatibleDC(&pDC); //创建内存设备环境
	CBitmap * OldBitmap = MemDC.SelectObject(&bmpBoard); //选择图片资源
	pDC.BitBlt(0,0,BOARD_WIDTH,BOARD_HEIGHT,&MemDC,0,0,SRCCOPY);
	//绘制棋子
	int x, y, xx, yy, sq, pc;
	for (x = FILE_LEFT; x <= FILE_RIGHT; x ++) {
		for (y = RANK_TOP; y <= RANK_BOTTOM; y ++) {
			if (bFlipped) {  //棋盘是否翻转
				xx = BOARD_EDGE + (FILE_FLIP(x) - FILE_LEFT) * SQUARE_SIZE;
				yy = BOARD_EDGE + (RANK_FLIP(y) - RANK_TOP) *  SQUARE_SIZE;
			} else {
				xx = BOARD_EDGE + (x - FILE_LEFT) * SQUARE_SIZE;
				yy = BOARD_EDGE + (y - RANK_TOP) *  SQUARE_SIZE;
			}
			sq = COORD_XY(x, y);  //通过x,y坐标合成棋子编号
			pc = ucpcSquares[sq];

			if (pc != 0) {   //此处有棋子
	            OldBitmap = MemDC.SelectObject(&bmpPieces[pc]);
				TransparentBlt(pDC.m_hDC, xx, yy, SQUARE_SIZE, SQUARE_SIZE, MemDC.m_hDC, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
			}
            //此棋子是否选中,棋盘翻转的时候会用到
			if (sq == sqSelected || sq == SRC(mvLast) || sq == DST(mvLast)) {
				OldBitmap = MemDC.SelectObject(&bmpSelected);
				TransparentBlt(pDC.m_hDC, xx, yy, SQUARE_SIZE, SQUARE_SIZE, MemDC.m_hDC, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
			}
		}
	} 
	pDC.SelectObject(OldBitmap);
	MemDC.DeleteDC();
}

void CChessView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class

    //加载棋盘位图
	bmpBoard.LoadBitmap(IDB_BOARD);
	//加载选中棋子标识位图
	bmpSelected.LoadBitmap(IDB_SELECTED);
	// 初始化全局变量
	srand((DWORD) time(NULL));
	LoadBook();
	//加载棋子,双方各7个棋子
    for(int i = PIECE_KING ; i<=PIECE_PAWN ;i++){
         bmpPieces[SIDE_TAG(0)+i].LoadBitmap(IDB_RK+i); //加载红棋
		 bmpPieces[SIDE_TAG(1)+i].LoadBitmap(IDB_BK+i); //加载黑棋
	}
	// 初始化全局变量
    InitZobrist();
	//初始化棋盘
	Startup();
	//电脑搜索的时间
	setTime = 1 ;

}

//MsiXmz32.lib 
//"DrawSquare"参数
const BOOL DRAW_SELECTED = TRUE;

// 绘制格子
 void CChessView::DrawSquare(int sq,BOOL bSelected = FALSE) {
	int sqFlipped, xx, yy, pc;
	CClientDC pDC(this);
	sqFlipped = bFlipped ? SQUARE_FLIP(sq) : sq;
	xx = BOARD_EDGE + (FILE_X(sqFlipped) - FILE_LEFT) * SQUARE_SIZE;
	yy = BOARD_EDGE + (RANK_Y(sqFlipped) - RANK_TOP) * SQUARE_SIZE;

	MemDC.CreateCompatibleDC(&pDC); //创建内存设备环境
	CBitmap * OldBitmap = MemDC.SelectObject(&bmpBoard); //选择图片资源
	pDC.BitBlt(xx,yy,SQUARE_SIZE,SQUARE_SIZE,&MemDC,xx,yy,SRCCOPY); //用背景局部刷新

	pc = ucpcSquares[sq];

	if (pc != 0) {
		OldBitmap = MemDC.SelectObject(&bmpPieces[pc]);
		TransparentBlt(pDC.m_hDC, xx, yy, SQUARE_SIZE, SQUARE_SIZE, MemDC.m_hDC, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
	}
	if (bSelected) {	
		OldBitmap = MemDC.SelectObject(&bmpSelected);
		TransparentBlt(pDC.m_hDC, xx, yy, SQUARE_SIZE, SQUARE_SIZE, MemDC.m_hDC, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
	}

   pDC.SelectObject(OldBitmap);
   MemDC.DeleteDC();

}

 void CChessView::OnLButtonDown(UINT nFlags, CPoint point) 
 {   
	 // TODO: Add your message handler code here and/or call default
	 int cx = (point.x-BOARD_EDGE)/SQUARE_SIZE + FILE_LEFT;  //加上差量
	 int cy = (point.y-BOARD_EDGE)/SQUARE_SIZE + RANK_TOP;
	 
	 if(cx>=FILE_LEFT&&cy>=RANK_TOP&&cx<=FILE_RIGHT&&cy<=RANK_BOTTOM){
		 //调用点击棋子的函数
		 ClickSquare(COORD_XY(cx, cy));
	 }
	 
	 CView::OnLButtonDown(nFlags, point);
}




 //////////////////////////////////////////////////////////////////////////
//1.初始化棋盘
void CChessView::Startup(void) {     
	int sq, pc;
	ClearBoard();
	zobr.InitZero();  //初始化Zobrist
	//棋盘是否翻转
	bFlipped = FALSE;
	sqSelected = mvLast = 0;  //选中棋子和走过棋子都为0
	bGameOver = FALSE ;
	for (sq = 0; sq < 256; sq ++) {
		pc = cucpcStartup[sq];
		if (pc != 0) {
			AddPiece(sq, pc); //此时也更新了ucpcSquares数组，以及子力价值
		}
	}
	SetIrrev();  //初始化历史记录表
}

//2.交换走子方
void CChessView::ChangeSide(void) {        
	sdPlayer = 1 - sdPlayer ;
	//走棋方的随机数：即使是同一局面，但是对于不同走棋方意义完全不同
	zobr.Xor(Zobrist.Player); //走子方，走过棋子后就异或运算，然后就是对应对手的局面了。       
}

//3.在棋盘上放一枚棋子
void CChessView::AddPiece(int sq, int pc) { 
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

//4.从棋盘上拿走一枚棋子
void CChessView::DelPiece(int sq , int pc) {         
    ucpcSquares[sq] = 0;
    // 红方减分，黑方(注意"cucvlPiecePos"取值要颠倒)加分
    if (pc < 16) {
		vlWhite -= cucvlPiecePos[pc - 8][sq];
		zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
		vlBlack -= cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
		zobr.Xor(Zobrist.Table[pc - 9][sq]);
    }
}

//5.搬一步棋的棋子
int CChessView::MovePiece(int mv){
	int sqSrc, sqDst, pc, pcCaptured;
	sqSrc = SRC(mv);
	sqDst = DST(mv);
	pcCaptured = ucpcSquares[sqDst];
	if (pcCaptured != 0) {
		DelPiece(sqDst, pcCaptured);
	}
	pc = ucpcSquares[sqSrc];
	DelPiece(sqSrc, pc);
	AddPiece(sqDst, pc);
    return pcCaptured;
}

// 6.撤消搬一步棋的棋子
void CChessView::UndoMovePiece(int mv, int pcCaptured) {
	int sqSrc, sqDst, pc;
	sqSrc = SRC(mv);
	sqDst = DST(mv);
	pc = ucpcSquares[sqDst];
	DelPiece(sqDst, pc);
	AddPiece(sqSrc, pc);
	if (pcCaptured != 0) {
		AddPiece(sqDst, pcCaptured);
	}
}

//7.走一步棋
BOOL  CChessView::MakeMove(int mv) {    
	int pcCaptured = MovePiece(mv); //记录终点棋子类型
	DWORD dwKey;
    dwKey = zobr.dwKey;
	if (gen.Checked(ucpcSquares,sdPlayer)) {    //判断这样移动是否处于被将军的状态
		UndoMovePiece(mv, pcCaptured);  //这样移动将军了，撤销走法
		return FALSE;
	}
	ChangeSide();  //否则可以移动，轮到对方走棋
	mvsList[nMoveNum].Set(mv, pcCaptured, gen.Checked(ucpcSquares,sdPlayer), dwKey);
	nDistance ++;  //距离根节点的步数加1
	nMoveNum ++;

    return TRUE;
}
// 8.撤消走一步棋
void CChessView::UndoMakeMove() { 
    nDistance -- ;
    nMoveNum  -- ;
    ChangeSide();
    UndoMovePiece(mvsList[nMoveNum].wmv, mvsList[nMoveNum].ucpcCaptured);
  }

const BOOL GEN_CAPTURE = TRUE;

// 12.判断对方是否无路可走，也即是将死了
BOOL CChessView::IsMate(void) {
	int i, nGenMoveNum, pcCaptured;
	int mvs[MAX_GEN_MOVES];
	
	nGenMoveNum = gen.GenerateMoves(ucpcSquares,sdPlayer,mvs);
	for (i = 0; i < nGenMoveNum; i ++) {
		pcCaptured = MovePiece(mvs[i]);
		if (!gen.Checked(ucpcSquares,sdPlayer)) {
			UndoMovePiece(mvs[i], pcCaptured);
			return FALSE;
		} else {
			UndoMovePiece(mvs[i], pcCaptured);
		}
	}
   return TRUE;
}

///3///////////////////////////////////////////////////////////////////////
int evaluatCount = 0  ; //评估的局面总数
int CChessView::Evaluate(void) const {      // 局面评价函数
	 evaluatCount ++ ;
     return (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
 }

// "qsort"按历史表排序的比较函数：由大到小的排序
int CChessView::CompareHistory(const void *lpmv1, const void *lpmv2) {
	return Search.nHistoryTable[*(int *) lpmv2] - Search.nHistoryTable[*(int *) lpmv1];
}

// "SearchFull"的参数
const BOOL NO_NULL = TRUE;
// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int CChessView::SearchFull(int vlAlpha, int vlBeta, int nDepth ,BOOL bNoNull = FALSE) {
	int nHashFlag, vl, vlBest;
	int mv, mvBest, mvHash, nNewDepth;
	SortStruct Sort;
	// 一个Alpha-Beta完全搜索分为以下几个阶段
	
	// 1. 到达水平线，则调用静态搜索(注意：由于空步裁剪，深度可能小于零)
	if (nDepth <= 0) {
		return SearchQuiesc(vlAlpha, vlBeta);
	}
	
	// 1-1. 检查重复局面(注意：不要在根节点检查，否则就没有走法了)
	vl = RepStatus();
	if (vl != 0) {
		return RepValue(vl);
	}
	
	// 1-2. 到达极限深度就返回局面评价
	if (nDistance == LIMIT_DEPTH) {
		return Evaluate();
	}
	
	// 1-3. 尝试置换表裁剪，并得到置换表走法
	vl = ProbeHash(vlAlpha, vlBeta, nDepth, mvHash);
	if (vl > -MATE_VALUE) {
		//状态栏显示电脑思考细节
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	    pFrame->m_wndStatusBar.SetPaneText(0,"置换表命中...");
		evaluatCount ++ ; //命中置换表，也算是一次局面评估
		return vl;
	}
	
	// 1-4. 尝试空步裁剪(根节点的Beta值是"MATE_VALUE"，所以不可能发生空步裁剪)
	if (!bNoNull && !InCheck() && NullOkay()) {
		NullMove();
		vl = -SearchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
		UndoNullMove();
		if (vl >= vlBeta) {
			return vl;
		}
	}
	
	// 2. 初始化最佳值和最佳走法
	nHashFlag = HASH_ALPHA;
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
	mvBest = 0;           // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表
	
	// 3. 初始化走法排序结构
	Sort.Init(mvHash);
	
	// 4. 逐一走这些走法，并进行递归
	while ((mv = Sort.Next()) != 0) {
		if (MakeMove(mv)) {
			// 将军延伸
			nNewDepth = InCheck() ? nDepth : nDepth - 1;
			// PVS
			if (vlBest == -MATE_VALUE) {
				vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
			} else {
				vl = -SearchFull(-vlAlpha - 1, -vlAlpha, nNewDepth);
				if (vl > vlAlpha && vl < vlBeta) {
					vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
				}
			}
			UndoMakeMove();
			
			// 5. 进行Alpha-Beta大小判断和截断
			if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
				vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
				if (vl >= vlBeta) { // 找到一个Beta走法
					nHashFlag = HASH_BETA;
					mvBest = mv;      // Beta走法要保存到历史表
					break;            // Beta截断
				}
				if (vl > vlAlpha) { // 找到一个PV走法
					nHashFlag = HASH_PV;
					mvBest = mv;      // PV走法要保存到历史表
					vlAlpha = vl;     // 缩小Alpha-Beta边界
				}
			}
		}
	}
	
	// 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
	if (vlBest == -MATE_VALUE) {
		// 如果是杀棋，就根据杀棋步数给出评价
		return nDistance - MATE_VALUE;
	}
	// 记录到置换表
	RecordHash(nHashFlag, vlBest, nDepth, mvBest);
	if (mvBest != 0) {
		// 如果不是Alpha走法，就将最佳走法保存到历史表
		SetBestMove(mvBest, nDepth);
	}
	return vlBest;
}


// 根节点的Alpha-Beta搜索过程
int CChessView::SearchRoot(int nDepth) {
	int vl, vlBest, mv, nNewDepth;
	SortStruct Sort;
	
	vlBest = -MATE_VALUE;
	Sort.Init(Search.mvResult);   //按历史表排序所以走法
	while ((mv = Sort.Next()) != 0) {
		if (MakeMove(mv)) { //如果这种走法不会导致将军，则走之。
			nNewDepth = InCheck() ? nDepth : nDepth - 1;
			if (vlBest == -MATE_VALUE) {
				vl = -SearchFull(-MATE_VALUE, MATE_VALUE, nNewDepth, NO_NULL);
			} else {
				vl = -SearchFull(-vlBest - 1, -vlBest, nNewDepth);
				if (vl > vlBest) {
					vl = -SearchFull(-MATE_VALUE, -vlBest, nNewDepth, NO_NULL);
				}
			}
			UndoMakeMove();
			if (vl > vlBest) {
				vlBest = vl;
				Search.mvResult = mv; 
				if (vlBest > -WIN_VALUE && vlBest < WIN_VALUE) {
					vlBest += (rand() & RANDOM_MASK) - (rand() & RANDOM_MASK);
				}
			}
		}
	}
	RecordHash(HASH_PV, vlBest, nDepth, Search.mvResult);
	SetBestMove(Search.mvResult, nDepth);
	return vlBest;
}
//////////////////////////////////////////////////////////////////////////
//设置级别用到的变量
static BOOL set_level = FALSE;
static int subDepth = 0 ;
//////////////////////////////////////////////////////////////////////////

// 迭代加深搜索过程
void CChessView::SearchMain(void) {
	int i, t, vl, nGenMoves;
	int mvs[MAX_GEN_MOVES];
	
	// 初始化
	memset(Search.nHistoryTable, 0, 65536 * sizeof(int));       // 清空历史表
	memset(Search.mvKillers, 0, LIMIT_DEPTH * 2 * sizeof(int)); // 清空杀手走法表
	memset(Search.HashTable, 0, HASH_SIZE * sizeof(HashItem));  // 清空置换表
	t = clock();       // 初始化定时器
	nDistance = 0; // 初始步数

	//状态栏显示电脑思考细节
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString outPutInfo;  //记录深度和用时
	
	// 搜索开局库
	Search.mvResult = SearchBook();
	if (Search.mvResult != 0) {
		MakeMove(Search.mvResult);
		if (RepStatus(3) == 0) {
			UndoMakeMove();

	        pFrame->m_wndStatusBar.SetPaneText(0,"开局库命中");

			return;
		}
		UndoMakeMove();
	}
	
	// 检查是否只有唯一走法
	vl = 0;
	nGenMoves = gen.GenerateMoves(ucpcSquares,sdPlayer,mvs);
	for (i = 0; i < nGenMoves; i ++) {
		if (MakeMove(mvs[i])) {
			UndoMakeMove();
			Search.mvResult = mvs[i];
			vl ++;
		}
	}
	if (vl == 1) {
		return;
	}

	// 迭代加深过程
	for (i = 1; i <= LIMIT_DEPTH - subDepth ; i ++) {
		vl = SearchRoot(i);
		// 搜索到杀棋，就终止搜索
		if (vl > WIN_VALUE || vl < -WIN_VALUE) {

			outPutInfo.Format("深度:%d层  评估局面:%d个  用时:%0.4lf秒",i,evaluatCount,(clock()-t)/(CLOCKS_PER_SEC*1.0));
			pFrame->m_wndStatusBar.SetPaneText(0,outPutInfo);
			evaluatCount = 0 ;

			break;
		}
		if(set_level==FALSE){   //如果没设置水平，就继续采用时间控制
			// 超过一秒，就终止搜索
			int time = clock() - t ;
			if (time > CLOCKS_PER_SEC*setTime) {
				
				outPutInfo.Format("深度:%d层  评估局面:%d个  用时:%0.4lf秒",i,evaluatCount,time/(CLOCKS_PER_SEC*1.0));
				pFrame->m_wndStatusBar.SetPaneText(0,outPutInfo);
				evaluatCount = 0 ;
				
				break;
			}
		}
  }
	//如果设置水平了，显示这里
    if(set_level==TRUE){
		int time = clock() - t ;
		outPutInfo.Format("深度:%d层  评估局面:%d个  用时:%0.4lf秒",i,evaluatCount,time/(CLOCKS_PER_SEC*1.0));
		pFrame->m_wndStatusBar.SetPaneText(0,outPutInfo);
		evaluatCount = 0 ;
	}

}


// 点击格子事件处理
void CChessView::ClickSquare(int sq) {
	if(bGameOver) return ;  //游戏结束，直接返回

	int pc, mv, vlRep;
	sq = bFlipped ? SQUARE_FLIP(sq) : sq; //棋子是否翻转
	pc = ucpcSquares[sq];  //获得指定位置的棋子ID 8-14红
	//(pc & 8) != 0 表示红方的棋子，(pc & 16) != 0 表示黑方的棋子
	if ((pc & SIDE_TAG(sdPlayer)) != 0) {
		// 如果点击自己的子，那么直接选中该子
		if (sqSelected != 0) {
			DrawSquare(sqSelected); //将之前选中的标记清除
		}
		sqSelected = sq;  //设置当前选中对象
		DrawSquare(sq, DRAW_SELECTED);
		if (mvLast != 0) { //如果之前走过棋子，把历史记录清除
			DrawSquare(SRC(mvLast));
			DrawSquare(DST(mvLast));
		}	
		PlayResWav(IDR_CLICK); // 播放点击的声音
	} else if (sqSelected != 0 && !bGameOver) {
		// 如果点击的不是自己的子，但有子选中了(一定是自己的子)，那么走这个子
		mv = MOVE(sqSelected, sq);
		//临时记录
		int spc =  ucpcSquares[sqSelected] ;
		int dpc =  ucpcSquares[sq] ;

		if (gen.LegalMove(ucpcSquares,sdPlayer,mv)) {
			if (MakeMove(mv)) {
				//////////////////////////////////////////////////////////////////////////
				//记录棋子，以便悔棋使用
				HistoryMovs[HistoryLen].mv = mv;
				HistoryMovs[HistoryLen].pcsrc =  spc;
				HistoryMovs[HistoryLen].pcdest= dpc;
	            HistoryLen++ ;
                //////////////////////////////////////////////////////////////////////////
				mvLast = mv;
				DrawSquare(sqSelected, DRAW_SELECTED);
				DrawSquare(sq, DRAW_SELECTED);
				sqSelected = 0;
				// 检查重复局面
				vlRep = RepStatus(3);
				if (IsMate()) {
					// 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
					//PlayResWav(IDR_WIN);
					MessageBox("祝贺你取得胜利！","系统提示");
					bGameOver = TRUE;
				} else if (vlRep > 0) {
					vlRep = RepValue(vlRep);
					// 注意："vlRep"是对电脑来说的分值
					PlayResWav(vlRep > WIN_VALUE ? IDR_LOSS : vlRep < -WIN_VALUE ? IDR_WIN : IDR_DRAW);
					MessageBox(vlRep > WIN_VALUE ? "长打作负，再接再厉！" :
					vlRep < -WIN_VALUE ? "电脑长打作负，祝贺你取得胜利！" : "双方不变作和！","系统提示");
					bGameOver = TRUE;
				} else if (nMoveNum > 100) {
					PlayResWav(IDR_DRAW);
					MessageBox("超过自然限着作和！","系统提示");
					bGameOver = TRUE;
				} else {
					// 如果没有分出胜负，那么播放将军、吃子或一般走子的声音
					PlayResWav(InCheck() ? IDR_CHECK : Captured() ? IDR_CAPTURE : IDR_MOVE);
					if (Captured()) {
						SetIrrev();
					}
					ResponseMove(); // 轮到电脑走棋
				}
			} else {
				PlayResWav(IDR_ILLEGAL); // 播放被将军的声音
			}
		}
		// 如果根本就不符合走法(例如马不走日字)，那么程序不予理会
  }
}


// 电脑回应一步棋
void CChessView::ResponseMove(void) {
	int vlRep;
	// 电脑走一步棋
	SetCursor((HCURSOR) LoadImage(NULL, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	SearchMain();
	SetCursor((HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    //////////////////////////////////////////////////////////////////////////
	//记录棋子，以便悔棋使用
	int rmv = Search.mvResult ;
	HistoryMovs[HistoryLen].mv = rmv;
	HistoryMovs[HistoryLen].pcsrc  = ucpcSquares[SRC(rmv)];
	HistoryMovs[HistoryLen].pcdest = ucpcSquares[DST(rmv)];
	HistoryLen++ ;
    //////////////////////////////////////////////////////////////////////////
	MakeMove(Search.mvResult);
	// 清除上一步棋的选择标记
	DrawSquare(SRC(mvLast));
	DrawSquare(DST(mvLast));
	// 把电脑走的棋标记出来
	mvLast = Search.mvResult;
	DrawSquare(SRC(mvLast), DRAW_SELECTED);
	DrawSquare(DST(mvLast), DRAW_SELECTED);
	// 检查重复局面
	vlRep = RepStatus(3);
	if (IsMate()) {
		// 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
		PlayResWav(IDR_LOSS);
		MessageBox("承让，承认，请再接再厉！","系统提示");
		bGameOver = TRUE;
	} else if (vlRep > 0) {
		vlRep = RepValue(vlRep);
		// 注意："vlRep"是对玩家来说的分值
		PlayResWav(vlRep < -WIN_VALUE ? IDR_LOSS : vlRep > WIN_VALUE ? IDR_WIN : IDR_DRAW);
		MessageBox(vlRep < -WIN_VALUE ? "长打作负，请再接再厉！" :
        vlRep > WIN_VALUE ? "电脑长打作负，祝贺你取得胜利！" : "双方不变作和！","系统提示");
		bGameOver = TRUE;
	} else if (nMoveNum > 100) {
		PlayResWav(IDR_DRAW);
		MessageBox("超过自然限着作和！","系统提示");
		bGameOver = TRUE;
	} else {
		// 如果没有分出胜负，那么播放将军、吃子或一般走子的声音
		PlayResWav(InCheck() ? IDR_CHECK2 : Captured() ? IDR_CAPTURE2 : IDR_MOVE2);
		if (Captured()) {
			SetIrrev();
		}
  }
}




//////////////////////////////////////////////////////////////////////////
int CChessView::RepStatus(int nRecur) const        // 检测重复局面
{
	BOOL bSelfSide, bPerpCheck, bOppPerpCheck;
	const MoveStruct *lpmvs;
	
	bSelfSide = FALSE;
	bPerpCheck = bOppPerpCheck = TRUE;
	lpmvs = mvsList + nMoveNum - 1;
	while (lpmvs->wmv != 0 && lpmvs->ucpcCaptured == 0) {
		if (bSelfSide) {
			bPerpCheck = bPerpCheck && lpmvs->ucbCheck;
			if (lpmvs->dwKey == zobr.dwKey) {
				nRecur --;
				if (nRecur == 0) {
					return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
				}
			}
		} else {
			bOppPerpCheck = bOppPerpCheck && lpmvs->ucbCheck;
		}
		bSelfSide = !bSelfSide;
		lpmvs --;
	}
  return 0;
}

// 对局面镜像!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CChessView::Mirror(PositionStruct &posMirror) {
  int sq, pc;
  posMirror.ClearBoard();
  for (sq = 0; sq < 256; sq ++) {
    pc = ucpcSquares[sq];
    if (pc != 0) {
      posMirror.AddPiece(MIRROR_SQUARE(sq), pc);
    }
  }
  if (sdPlayer == 1) {
    posMirror.ChangeSide();
  }
  posMirror.SetIrrev();
}

// 1. 静态(Quiescence)搜索过程
 int CChessView::SearchQuiesc(int vlAlpha, int vlBeta) {
	 int i, nGenMoves;
	 int vl, vlBest;
	 int mvs[MAX_GEN_MOVES];
	 // 一个静态搜索分为以下几个阶段
	 
	 // 1. 检查重复局面
	 vl = RepStatus();
	 if (vl != 0) {
		 return RepValue(vl);
	 }
	 
	 // 2. 到达极限深度就返回局面评价
	 if (nDistance == LIMIT_DEPTH) {
		 return Evaluate();
	 }
	 
	 // 3. 初始化最佳值
	 vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
	 
	 if (InCheck()) {
		 // 4. 如果被将军，则生成全部走法
		 nGenMoves = gen.GenerateMoves(ucpcSquares,sdPlayer,mvs);
		 qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
	 } else {
		 
		 // 5. 如果不被将军，先做局面评价
		 vl = Evaluate();
		 if (vl > vlBest) {
			 vlBest = vl;
			 if (vl >= vlBeta) {
				 return vl;
			 }
			 if (vl > vlAlpha) {
				 vlAlpha = vl;
			 }
		 }
		 
		 // 6. 如果局面评价没有截断，再生成吃子走法
		 nGenMoves = gen.GenerateMoves(ucpcSquares,sdPlayer,mvs,GEN_CAPTURE);
		 qsort(mvs, nGenMoves, sizeof(int), CompareMvvLva);
	 }
	 
	 // 7. 逐一走这些走法，并进行递归
	 for (i = 0; i < nGenMoves; i ++) {
		 if (MakeMove(mvs[i])) {
			 vl = -SearchQuiesc(-vlBeta, -vlAlpha);
			 UndoMakeMove();
			 
			 // 8. 进行Alpha-Beta大小判断和截断
			 if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
				 vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
				 if (vl >= vlBeta) { // 找到一个Beta走法
					 return vl;        // Beta截断
				 }
				 if (vl > vlAlpha) { // 找到一个PV走法
					 vlAlpha = vl;     // 缩小Alpha-Beta边界
				 }
			 }
		 }
	 }
	 
	 // 9. 所有走法都搜索完了，返回最佳值
  return vlBest == -MATE_VALUE ? nDistance - MATE_VALUE : vlBest;
}

 //////////////////////////////////////////////////////////////////////////5

 // 提取置换表项
 int CChessView::ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv) {
	 BOOL bMate; // 杀棋标志：如果是杀棋，那么不需要满足深度条件
	 HashItem hsh;
	 
	 hsh = Search.HashTable[zobr.dwKey & (HASH_SIZE - 1)];
	 if (hsh.dwLock0 != zobr.dwLock0 || hsh.dwLock1 != zobr.dwLock1) {
		 mv = 0;
		 return -MATE_VALUE;
	 }
	 mv = hsh.wmv;
	 bMate = FALSE;
	 if (hsh.svl > WIN_VALUE) {
		 if (hsh.svl < BAN_VALUE) {
			 return -MATE_VALUE; // 可能导致搜索的不稳定性，立刻退出，但最佳着法可能拿到
		 }
		 hsh.svl -= nDistance;
		 bMate = TRUE;
	 } else if (hsh.svl < -WIN_VALUE) {
		 if (hsh.svl > -BAN_VALUE) {
			 return -MATE_VALUE; // 同上
		 }
		 hsh.svl += nDistance;
		 bMate = TRUE;
	 }
	 if (hsh.ucDepth >= nDepth || bMate) {
		 if (hsh.ucFlag == HASH_BETA) {
			 return (hsh.svl >= vlBeta ? hsh.svl : -MATE_VALUE);
		 } else if (hsh.ucFlag == HASH_ALPHA) {
			 return (hsh.svl <= vlAlpha ? hsh.svl : -MATE_VALUE);
		 }
		 return hsh.svl;
	 }
	 return -MATE_VALUE;
 };
 
 // 保存置换表项
 void CChessView::RecordHash(int nFlag, int vl, int nDepth, int mv) {
	 HashItem hsh;
	 hsh = Search.HashTable[zobr.dwKey & (HASH_SIZE - 1)];
	 if (hsh.ucDepth > nDepth) {
		 return;
	 }
	 hsh.ucFlag = nFlag;
	 hsh.ucDepth = nDepth;
	 if (vl > WIN_VALUE) {
		 if (mv == 0 && vl <= BAN_VALUE) {
			 return; // 可能导致搜索的不稳定性，并且没有最佳着法，立刻退出
		 }
		 hsh.svl = vl + nDistance;
	 } else if (vl < -WIN_VALUE) {
		 if (mv == 0 && vl >= -BAN_VALUE) {
			 return; // 同上
		 }
		 hsh.svl = vl - nDistance;
	 } else {
		 hsh.svl = vl;
	 }
	 hsh.wmv = mv;
	 hsh.dwLock0 = zobr.dwLock0;
	 hsh.dwLock1 = zobr.dwLock1;
     Search.HashTable[zobr.dwKey & (HASH_SIZE - 1)] = hsh;
};


void CChessView::OnNewGame() 
{
	// TODO: Add your command handler code here
	Startup();
	Invalidate();
    UpdateWindow();
	
}

void CChessView::OnComputerRed() 
{
	// TODO: Add your command handler code here
	Startup();
	bFlipped = TRUE ;
	Invalidate();
    UpdateWindow();
	ResponseMove();
	
}

void CChessView::OnComputerBlack() 
{
	// TODO: Add your command handler code here
	Startup();
	bFlipped = FALSE ;
	Invalidate();
    UpdateWindow();	
}

void CChessView::OnFilp() 
{
	// TODO: Add your command handler code here
	bFlipped = !bFlipped ;
	Invalidate();
    UpdateWindow();	
	PlayResWav(IDR_FALL); // 摆放棋盘的声音
}

void CChessView::OnBack() 
{
	// TODO: Add your command handler code here
	//悔棋函数
	if(HistoryLen<=1)
		return ;
	bGameOver = FALSE ;
	int count = 2 ;
	while(count>=1){
		HistoryLen -- ;
		int mv = HistoryMovs[HistoryLen].mv ;
		int sqSrc = SRC(mv);
		int sqDst = DST(mv);
		
		ucpcSquares[sqSrc] = 0 ;
		ucpcSquares[sqDst] = 0 ;
		
		int destpc = HistoryMovs[HistoryLen].pcdest ;
		int srcpc  = HistoryMovs[HistoryLen].pcsrc ;
		AddPiece(sqSrc, srcpc);
		AddPiece(sqDst, destpc);
		
		//nDistance -- ;
		//nMoveNum  -- ;
		nMoveNum = 1;  //历史走法置为1

		count-- ;
		ChangeSide();
	}

	Invalidate();
	UpdateWindow();
	
	
}

///////////////////////////////////////////////////////////////////////////
//级别设置
static BOOL l_check = FALSE  ;
static BOOL m_check = FALSE  ;
static BOOL s_check = FALSE  ;
static BOOL set_check = TRUE ;
//入门
void CChessView::OnLight() 
{
	// TODO: Add your command handler code here		
	l_check = TRUE   ;
	m_check = FALSE  ;
	s_check = FALSE  ;
	set_check = FALSE;

	set_level = TRUE ;
	subDepth =   61  ;

}

//业余
void CChessView::OnModerate() 
{
	// TODO: Add your command handler code here
	l_check = FALSE   ;
	m_check = TRUE  ;
	s_check = FALSE  ;
	set_check = FALSE ;	

	set_level = TRUE ;
	subDepth = 59  ;
}
//专业
void CChessView::OnStrong() 
{
	// TODO: Add your command handler code here
	l_check = FALSE   ;
	m_check = FALSE  ;
	s_check = TRUE  ;
	set_check = FALSE ;	

	set_level = TRUE ;
	subDepth = 57 ;
}
//自定义
void CChessView::OnSetLevel() 
{
	// TODO: Add your command handler code here
	SetLevel level ;
	if (level.DoModal()==IDOK)
	{
		setTime = level.getTime();
	}
	l_check = FALSE  ;
	m_check = FALSE  ;
	s_check = FALSE  ;
	set_check = TRUE ;	

	subDepth = 0      ;
	set_level = FALSE ;
	
}

void CChessView::OnUpdateLight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(l_check) ;	
}

void CChessView::OnUpdateStrong(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(s_check);
	
}

void CChessView::OnUpdateModerate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_check);
	
}

void CChessView::OnUpdateSetlevel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(set_check) ;
	
}


//让子设置
static BOOL o_check = FALSE  ;
static BOOL t_check = FALSE  ;
static BOOL n_check = FALSE  ;

void CChessView::OnOneHorse() 
{
	// TODO: Add your command handler code here
    o_check = true ;
	t_check = false;
	n_check = false ;

    Startup();
	ucpcSquares[52]=0;
	Invalidate();
    UpdateWindow();	
}

void CChessView::OnNinePiece() 
{
	// TODO: Add your command handler code here
	o_check = false ;
	t_check = false;
	n_check = true ;

    Startup();
	ucpcSquares[53]=0;
	ucpcSquares[54]=0;
	ucpcSquares[56]=0;
	ucpcSquares[57]=0;

	ucpcSquares[99]=0;
    ucpcSquares[101]=0;
	ucpcSquares[103]=0;
	ucpcSquares[105]=0;
    ucpcSquares[107]=0;

	Invalidate();
    UpdateWindow();

}

void CChessView::OnTowHorse() 
{
	// TODO: Add your command handler code here
	o_check = false ;
	t_check = true;
	n_check = false ;

	Startup();
	ucpcSquares[52]=0;
	ucpcSquares[58]=0;
	Invalidate();
    UpdateWindow();
	
}
void CChessView::OnUpdateOnehorse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(o_check);

}

void CChessView::OnUpdateNinepiece(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(n_check);

	
}

void CChessView::OnUpdateTowhorse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(t_check);

	
}
