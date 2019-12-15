// ChessView.cpp : implementation of the CChessView class
//����ͼʵ��

#include "stdafx.h"
#include "Chess.h"

#include "ChessDoc.h"
#include "ChessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


byte CChessView::ucpcSquares[256] ={0}; //��̬�����ʼ��
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

	//��������
	MemDC.CreateCompatibleDC(&pDC); //�����ڴ��豸����
	CBitmap * OldBitmap = MemDC.SelectObject(&bmpBoard); //ѡ��ͼƬ��Դ
	pDC.BitBlt(0,0,BOARD_WIDTH,BOARD_HEIGHT,&MemDC,0,0,SRCCOPY);
	//��������
	int x, y, xx, yy, sq, pc;
	for (x = FILE_LEFT; x <= FILE_RIGHT; x ++) {
		for (y = RANK_TOP; y <= RANK_BOTTOM; y ++) {
			if (bFlipped) {  //�����Ƿ�ת
				xx = BOARD_EDGE + (FILE_FLIP(x) - FILE_LEFT) * SQUARE_SIZE;
				yy = BOARD_EDGE + (RANK_FLIP(y) - RANK_TOP) *  SQUARE_SIZE;
			} else {
				xx = BOARD_EDGE + (x - FILE_LEFT) * SQUARE_SIZE;
				yy = BOARD_EDGE + (y - RANK_TOP) *  SQUARE_SIZE;
			}
			sq = COORD_XY(x, y);  //ͨ��x,y����ϳ����ӱ��
			pc = ucpcSquares[sq];

			if (pc != 0) {   //�˴�������
	            OldBitmap = MemDC.SelectObject(&bmpPieces[pc]);
				TransparentBlt(pDC.m_hDC, xx, yy, SQUARE_SIZE, SQUARE_SIZE, MemDC.m_hDC, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
			}
            //�������Ƿ�ѡ��,���̷�ת��ʱ����õ�
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

    //��������λͼ
	bmpBoard.LoadBitmap(IDB_BOARD);
	//����ѡ�����ӱ�ʶλͼ
	bmpSelected.LoadBitmap(IDB_SELECTED);
	// ��ʼ��ȫ�ֱ���
	srand((DWORD) time(NULL));
	LoadBook();
	//��������,˫����7������
    for(int i = PIECE_KING ; i<=PIECE_PAWN ;i++){
         bmpPieces[SIDE_TAG(0)+i].LoadBitmap(IDB_RK+i); //���غ���
		 bmpPieces[SIDE_TAG(1)+i].LoadBitmap(IDB_BK+i); //���غ���
	}
	// ��ʼ��ȫ�ֱ���
    InitZobrist();
	//��ʼ������
	Startup();
	//����������ʱ��
	setTime = 1 ;

}

//MsiXmz32.lib 
//"DrawSquare"����
const BOOL DRAW_SELECTED = TRUE;

// ���Ƹ���
 void CChessView::DrawSquare(int sq,BOOL bSelected = FALSE) {
	int sqFlipped, xx, yy, pc;
	CClientDC pDC(this);
	sqFlipped = bFlipped ? SQUARE_FLIP(sq) : sq;
	xx = BOARD_EDGE + (FILE_X(sqFlipped) - FILE_LEFT) * SQUARE_SIZE;
	yy = BOARD_EDGE + (RANK_Y(sqFlipped) - RANK_TOP) * SQUARE_SIZE;

	MemDC.CreateCompatibleDC(&pDC); //�����ڴ��豸����
	CBitmap * OldBitmap = MemDC.SelectObject(&bmpBoard); //ѡ��ͼƬ��Դ
	pDC.BitBlt(xx,yy,SQUARE_SIZE,SQUARE_SIZE,&MemDC,xx,yy,SRCCOPY); //�ñ����ֲ�ˢ��

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
	 int cx = (point.x-BOARD_EDGE)/SQUARE_SIZE + FILE_LEFT;  //���ϲ���
	 int cy = (point.y-BOARD_EDGE)/SQUARE_SIZE + RANK_TOP;
	 
	 if(cx>=FILE_LEFT&&cy>=RANK_TOP&&cx<=FILE_RIGHT&&cy<=RANK_BOTTOM){
		 //���õ�����ӵĺ���
		 ClickSquare(COORD_XY(cx, cy));
	 }
	 
	 CView::OnLButtonDown(nFlags, point);
}




 //////////////////////////////////////////////////////////////////////////
//1.��ʼ������
void CChessView::Startup(void) {     
	int sq, pc;
	ClearBoard();
	zobr.InitZero();  //��ʼ��Zobrist
	//�����Ƿ�ת
	bFlipped = FALSE;
	sqSelected = mvLast = 0;  //ѡ�����Ӻ��߹����Ӷ�Ϊ0
	bGameOver = FALSE ;
	for (sq = 0; sq < 256; sq ++) {
		pc = cucpcStartup[sq];
		if (pc != 0) {
			AddPiece(sq, pc); //��ʱҲ������ucpcSquares���飬�Լ�������ֵ
		}
	}
	SetIrrev();  //��ʼ����ʷ��¼��
}

//2.�������ӷ�
void CChessView::ChangeSide(void) {        
	sdPlayer = 1 - sdPlayer ;
	//���巽�����������ʹ��ͬһ���棬���Ƕ��ڲ�ͬ���巽������ȫ��ͬ
	zobr.Xor(Zobrist.Player); //���ӷ����߹����Ӻ��������㣬Ȼ����Ƕ�Ӧ���ֵľ����ˡ�       
}

//3.�������Ϸ�һö����
void CChessView::AddPiece(int sq, int pc) { 
	ucpcSquares[sq] = pc;
    // �췽�ӷ֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)����
    if (pc < 16) {
		vlWhite += cucvlPiecePos[pc - 8][sq];
		zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
		vlBlack += cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
		zobr.Xor(Zobrist.Table[pc - 9][sq]);
    }
}

//4.������������һö����
void CChessView::DelPiece(int sq , int pc) {         
    ucpcSquares[sq] = 0;
    // �췽���֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)�ӷ�
    if (pc < 16) {
		vlWhite -= cucvlPiecePos[pc - 8][sq];
		zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
		vlBlack -= cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
		zobr.Xor(Zobrist.Table[pc - 9][sq]);
    }
}

//5.��һ���������
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

// 6.������һ���������
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

//7.��һ����
BOOL  CChessView::MakeMove(int mv) {    
	int pcCaptured = MovePiece(mv); //��¼�յ���������
	DWORD dwKey;
    dwKey = zobr.dwKey;
	if (gen.Checked(ucpcSquares,sdPlayer)) {    //�ж������ƶ��Ƿ��ڱ�������״̬
		UndoMovePiece(mv, pcCaptured);  //�����ƶ������ˣ������߷�
		return FALSE;
	}
	ChangeSide();  //��������ƶ����ֵ��Է�����
	mvsList[nMoveNum].Set(mv, pcCaptured, gen.Checked(ucpcSquares,sdPlayer), dwKey);
	nDistance ++;  //������ڵ�Ĳ�����1
	nMoveNum ++;

    return TRUE;
}
// 8.������һ����
void CChessView::UndoMakeMove() { 
    nDistance -- ;
    nMoveNum  -- ;
    ChangeSide();
    UndoMovePiece(mvsList[nMoveNum].wmv, mvsList[nMoveNum].ucpcCaptured);
  }

const BOOL GEN_CAPTURE = TRUE;

// 12.�ж϶Է��Ƿ���·���ߣ�Ҳ���ǽ�����
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
int evaluatCount = 0  ; //�����ľ�������
int CChessView::Evaluate(void) const {      // �������ۺ���
	 evaluatCount ++ ;
     return (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
 }

// "qsort"����ʷ������ıȽϺ������ɴ�С������
int CChessView::CompareHistory(const void *lpmv1, const void *lpmv2) {
	return Search.nHistoryTable[*(int *) lpmv2] - Search.nHistoryTable[*(int *) lpmv1];
}

// "SearchFull"�Ĳ���
const BOOL NO_NULL = TRUE;
// �����߽�(Fail-Soft)��Alpha-Beta��������
int CChessView::SearchFull(int vlAlpha, int vlBeta, int nDepth ,BOOL bNoNull = FALSE) {
	int nHashFlag, vl, vlBest;
	int mv, mvBest, mvHash, nNewDepth;
	SortStruct Sort;
	// һ��Alpha-Beta��ȫ������Ϊ���¼����׶�
	
	// 1. ����ˮƽ�ߣ�����þ�̬����(ע�⣺���ڿղ��ü�����ȿ���С����)
	if (nDepth <= 0) {
		return SearchQuiesc(vlAlpha, vlBeta);
	}
	
	// 1-1. ����ظ�����(ע�⣺��Ҫ�ڸ��ڵ��飬�����û���߷���)
	vl = RepStatus();
	if (vl != 0) {
		return RepValue(vl);
	}
	
	// 1-2. ���Ｋ����Ⱦͷ��ؾ�������
	if (nDistance == LIMIT_DEPTH) {
		return Evaluate();
	}
	
	// 1-3. �����û���ü������õ��û����߷�
	vl = ProbeHash(vlAlpha, vlBeta, nDepth, mvHash);
	if (vl > -MATE_VALUE) {
		//״̬����ʾ����˼��ϸ��
		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	    pFrame->m_wndStatusBar.SetPaneText(0,"�û�������...");
		evaluatCount ++ ; //�����û���Ҳ����һ�ξ�������
		return vl;
	}
	
	// 1-4. ���Կղ��ü�(���ڵ��Betaֵ��"MATE_VALUE"�����Բ����ܷ����ղ��ü�)
	if (!bNoNull && !InCheck() && NullOkay()) {
		NullMove();
		vl = -SearchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
		UndoNullMove();
		if (vl >= vlBeta) {
			return vl;
		}
	}
	
	// 2. ��ʼ�����ֵ������߷�
	nHashFlag = HASH_ALPHA;
	vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	mvBest = 0;           // ��������֪�����Ƿ���������Beta�߷���PV�߷����Ա㱣�浽��ʷ��
	
	// 3. ��ʼ���߷�����ṹ
	Sort.Init(mvHash);
	
	// 4. ��һ����Щ�߷��������еݹ�
	while ((mv = Sort.Next()) != 0) {
		if (MakeMove(mv)) {
			// ��������
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
			
			// 5. ����Alpha-Beta��С�жϺͽض�
			if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
				vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
				if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
					nHashFlag = HASH_BETA;
					mvBest = mv;      // Beta�߷�Ҫ���浽��ʷ��
					break;            // Beta�ض�
				}
				if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
					nHashFlag = HASH_PV;
					mvBest = mv;      // PV�߷�Ҫ���浽��ʷ��
					vlAlpha = vl;     // ��СAlpha-Beta�߽�
				}
			}
		}
	}
	
	// 5. �����߷����������ˣ�������߷�(������Alpha�߷�)���浽��ʷ���������ֵ
	if (vlBest == -MATE_VALUE) {
		// �����ɱ�壬�͸���ɱ�岽����������
		return nDistance - MATE_VALUE;
	}
	// ��¼���û���
	RecordHash(nHashFlag, vlBest, nDepth, mvBest);
	if (mvBest != 0) {
		// �������Alpha�߷����ͽ�����߷����浽��ʷ��
		SetBestMove(mvBest, nDepth);
	}
	return vlBest;
}


// ���ڵ��Alpha-Beta��������
int CChessView::SearchRoot(int nDepth) {
	int vl, vlBest, mv, nNewDepth;
	SortStruct Sort;
	
	vlBest = -MATE_VALUE;
	Sort.Init(Search.mvResult);   //����ʷ�����������߷�
	while ((mv = Sort.Next()) != 0) {
		if (MakeMove(mv)) { //��������߷����ᵼ�½���������֮��
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
//���ü����õ��ı���
static BOOL set_level = FALSE;
static int subDepth = 0 ;
//////////////////////////////////////////////////////////////////////////

// ����������������
void CChessView::SearchMain(void) {
	int i, t, vl, nGenMoves;
	int mvs[MAX_GEN_MOVES];
	
	// ��ʼ��
	memset(Search.nHistoryTable, 0, 65536 * sizeof(int));       // �����ʷ��
	memset(Search.mvKillers, 0, LIMIT_DEPTH * 2 * sizeof(int)); // ���ɱ���߷���
	memset(Search.HashTable, 0, HASH_SIZE * sizeof(HashItem));  // ����û���
	t = clock();       // ��ʼ����ʱ��
	nDistance = 0; // ��ʼ����

	//״̬����ʾ����˼��ϸ��
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString outPutInfo;  //��¼��Ⱥ���ʱ
	
	// �������ֿ�
	Search.mvResult = SearchBook();
	if (Search.mvResult != 0) {
		MakeMove(Search.mvResult);
		if (RepStatus(3) == 0) {
			UndoMakeMove();

	        pFrame->m_wndStatusBar.SetPaneText(0,"���ֿ�����");

			return;
		}
		UndoMakeMove();
	}
	
	// ����Ƿ�ֻ��Ψһ�߷�
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

	// �����������
	for (i = 1; i <= LIMIT_DEPTH - subDepth ; i ++) {
		vl = SearchRoot(i);
		// ������ɱ�壬����ֹ����
		if (vl > WIN_VALUE || vl < -WIN_VALUE) {

			outPutInfo.Format("���:%d��  ��������:%d��  ��ʱ:%0.4lf��",i,evaluatCount,(clock()-t)/(CLOCKS_PER_SEC*1.0));
			pFrame->m_wndStatusBar.SetPaneText(0,outPutInfo);
			evaluatCount = 0 ;

			break;
		}
		if(set_level==FALSE){   //���û����ˮƽ���ͼ�������ʱ�����
			// ����һ�룬����ֹ����
			int time = clock() - t ;
			if (time > CLOCKS_PER_SEC*setTime) {
				
				outPutInfo.Format("���:%d��  ��������:%d��  ��ʱ:%0.4lf��",i,evaluatCount,time/(CLOCKS_PER_SEC*1.0));
				pFrame->m_wndStatusBar.SetPaneText(0,outPutInfo);
				evaluatCount = 0 ;
				
				break;
			}
		}
  }
	//�������ˮƽ�ˣ���ʾ����
    if(set_level==TRUE){
		int time = clock() - t ;
		outPutInfo.Format("���:%d��  ��������:%d��  ��ʱ:%0.4lf��",i,evaluatCount,time/(CLOCKS_PER_SEC*1.0));
		pFrame->m_wndStatusBar.SetPaneText(0,outPutInfo);
		evaluatCount = 0 ;
	}

}


// ��������¼�����
void CChessView::ClickSquare(int sq) {
	if(bGameOver) return ;  //��Ϸ������ֱ�ӷ���

	int pc, mv, vlRep;
	sq = bFlipped ? SQUARE_FLIP(sq) : sq; //�����Ƿ�ת
	pc = ucpcSquares[sq];  //���ָ��λ�õ�����ID 8-14��
	//(pc & 8) != 0 ��ʾ�췽�����ӣ�(pc & 16) != 0 ��ʾ�ڷ�������
	if ((pc & SIDE_TAG(sdPlayer)) != 0) {
		// �������Լ����ӣ���ôֱ��ѡ�и���
		if (sqSelected != 0) {
			DrawSquare(sqSelected); //��֮ǰѡ�еı�����
		}
		sqSelected = sq;  //���õ�ǰѡ�ж���
		DrawSquare(sq, DRAW_SELECTED);
		if (mvLast != 0) { //���֮ǰ�߹����ӣ�����ʷ��¼���
			DrawSquare(SRC(mvLast));
			DrawSquare(DST(mvLast));
		}	
		PlayResWav(IDR_CLICK); // ���ŵ��������
	} else if (sqSelected != 0 && !bGameOver) {
		// �������Ĳ����Լ����ӣ�������ѡ����(һ�����Լ�����)����ô�������
		mv = MOVE(sqSelected, sq);
		//��ʱ��¼
		int spc =  ucpcSquares[sqSelected] ;
		int dpc =  ucpcSquares[sq] ;

		if (gen.LegalMove(ucpcSquares,sdPlayer,mv)) {
			if (MakeMove(mv)) {
				//////////////////////////////////////////////////////////////////////////
				//��¼���ӣ��Ա����ʹ��
				HistoryMovs[HistoryLen].mv = mv;
				HistoryMovs[HistoryLen].pcsrc =  spc;
				HistoryMovs[HistoryLen].pcdest= dpc;
	            HistoryLen++ ;
                //////////////////////////////////////////////////////////////////////////
				mvLast = mv;
				DrawSquare(sqSelected, DRAW_SELECTED);
				DrawSquare(sq, DRAW_SELECTED);
				sqSelected = 0;
				// ����ظ�����
				vlRep = RepStatus(3);
				if (IsMate()) {
					// ����ֳ�ʤ������ô����ʤ�������������ҵ���������������ʾ��
					//PlayResWav(IDR_WIN);
					MessageBox("ף����ȡ��ʤ����","ϵͳ��ʾ");
					bGameOver = TRUE;
				} else if (vlRep > 0) {
					vlRep = RepValue(vlRep);
					// ע�⣺"vlRep"�ǶԵ�����˵�ķ�ֵ
					PlayResWav(vlRep > WIN_VALUE ? IDR_LOSS : vlRep < -WIN_VALUE ? IDR_WIN : IDR_DRAW);
					MessageBox(vlRep > WIN_VALUE ? "�����������ٽ�������" :
					vlRep < -WIN_VALUE ? "���Գ���������ף����ȡ��ʤ����" : "˫���������ͣ�","ϵͳ��ʾ");
					bGameOver = TRUE;
				} else if (nMoveNum > 100) {
					PlayResWav(IDR_DRAW);
					MessageBox("������Ȼ�������ͣ�","ϵͳ��ʾ");
					bGameOver = TRUE;
				} else {
					// ���û�зֳ�ʤ������ô���Ž��������ӻ�һ�����ӵ�����
					PlayResWav(InCheck() ? IDR_CHECK : Captured() ? IDR_CAPTURE : IDR_MOVE);
					if (Captured()) {
						SetIrrev();
					}
					ResponseMove(); // �ֵ���������
				}
			} else {
				PlayResWav(IDR_ILLEGAL); // ���ű�����������
			}
		}
		// ��������Ͳ������߷�(������������)����ô���������
  }
}


// ���Ի�Ӧһ����
void CChessView::ResponseMove(void) {
	int vlRep;
	// ������һ����
	SetCursor((HCURSOR) LoadImage(NULL, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	SearchMain();
	SetCursor((HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
    //////////////////////////////////////////////////////////////////////////
	//��¼���ӣ��Ա����ʹ��
	int rmv = Search.mvResult ;
	HistoryMovs[HistoryLen].mv = rmv;
	HistoryMovs[HistoryLen].pcsrc  = ucpcSquares[SRC(rmv)];
	HistoryMovs[HistoryLen].pcdest = ucpcSquares[DST(rmv)];
	HistoryLen++ ;
    //////////////////////////////////////////////////////////////////////////
	MakeMove(Search.mvResult);
	// �����һ�����ѡ����
	DrawSquare(SRC(mvLast));
	DrawSquare(DST(mvLast));
	// �ѵ����ߵ����ǳ���
	mvLast = Search.mvResult;
	DrawSquare(SRC(mvLast), DRAW_SELECTED);
	DrawSquare(DST(mvLast), DRAW_SELECTED);
	// ����ظ�����
	vlRep = RepStatus(3);
	if (IsMate()) {
		// ����ֳ�ʤ������ô����ʤ�������������ҵ���������������ʾ��
		PlayResWav(IDR_LOSS);
		MessageBox("���ã����ϣ����ٽ�������","ϵͳ��ʾ");
		bGameOver = TRUE;
	} else if (vlRep > 0) {
		vlRep = RepValue(vlRep);
		// ע�⣺"vlRep"�Ƕ������˵�ķ�ֵ
		PlayResWav(vlRep < -WIN_VALUE ? IDR_LOSS : vlRep > WIN_VALUE ? IDR_WIN : IDR_DRAW);
		MessageBox(vlRep < -WIN_VALUE ? "�������������ٽ�������" :
        vlRep > WIN_VALUE ? "���Գ���������ף����ȡ��ʤ����" : "˫���������ͣ�","ϵͳ��ʾ");
		bGameOver = TRUE;
	} else if (nMoveNum > 100) {
		PlayResWav(IDR_DRAW);
		MessageBox("������Ȼ�������ͣ�","ϵͳ��ʾ");
		bGameOver = TRUE;
	} else {
		// ���û�зֳ�ʤ������ô���Ž��������ӻ�һ�����ӵ�����
		PlayResWav(InCheck() ? IDR_CHECK2 : Captured() ? IDR_CAPTURE2 : IDR_MOVE2);
		if (Captured()) {
			SetIrrev();
		}
  }
}




//////////////////////////////////////////////////////////////////////////
int CChessView::RepStatus(int nRecur) const        // ����ظ�����
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

// �Ծ��澵��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

// 1. ��̬(Quiescence)��������
 int CChessView::SearchQuiesc(int vlAlpha, int vlBeta) {
	 int i, nGenMoves;
	 int vl, vlBest;
	 int mvs[MAX_GEN_MOVES];
	 // һ����̬������Ϊ���¼����׶�
	 
	 // 1. ����ظ�����
	 vl = RepStatus();
	 if (vl != 0) {
		 return RepValue(vl);
	 }
	 
	 // 2. ���Ｋ����Ⱦͷ��ؾ�������
	 if (nDistance == LIMIT_DEPTH) {
		 return Evaluate();
	 }
	 
	 // 3. ��ʼ�����ֵ
	 vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	 
	 if (InCheck()) {
		 // 4. �����������������ȫ���߷�
		 nGenMoves = gen.GenerateMoves(ucpcSquares,sdPlayer,mvs);
		 qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
	 } else {
		 
		 // 5. �������������������������
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
		 
		 // 6. �����������û�нضϣ������ɳ����߷�
		 nGenMoves = gen.GenerateMoves(ucpcSquares,sdPlayer,mvs,GEN_CAPTURE);
		 qsort(mvs, nGenMoves, sizeof(int), CompareMvvLva);
	 }
	 
	 // 7. ��һ����Щ�߷��������еݹ�
	 for (i = 0; i < nGenMoves; i ++) {
		 if (MakeMove(mvs[i])) {
			 vl = -SearchQuiesc(-vlBeta, -vlAlpha);
			 UndoMakeMove();
			 
			 // 8. ����Alpha-Beta��С�жϺͽض�
			 if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
				 vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
				 if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
					 return vl;        // Beta�ض�
				 }
				 if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
					 vlAlpha = vl;     // ��СAlpha-Beta�߽�
				 }
			 }
		 }
	 }
	 
	 // 9. �����߷����������ˣ��������ֵ
  return vlBest == -MATE_VALUE ? nDistance - MATE_VALUE : vlBest;
}

 //////////////////////////////////////////////////////////////////////////5

 // ��ȡ�û�����
 int CChessView::ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv) {
	 BOOL bMate; // ɱ���־�������ɱ�壬��ô����Ҫ�����������
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
			 return -MATE_VALUE; // ���ܵ��������Ĳ��ȶ��ԣ������˳���������ŷ������õ�
		 }
		 hsh.svl -= nDistance;
		 bMate = TRUE;
	 } else if (hsh.svl < -WIN_VALUE) {
		 if (hsh.svl > -BAN_VALUE) {
			 return -MATE_VALUE; // ͬ��
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
 
 // �����û�����
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
			 return; // ���ܵ��������Ĳ��ȶ��ԣ�����û������ŷ��������˳�
		 }
		 hsh.svl = vl + nDistance;
	 } else if (vl < -WIN_VALUE) {
		 if (mv == 0 && vl >= -BAN_VALUE) {
			 return; // ͬ��
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
	PlayResWav(IDR_FALL); // �ڷ����̵�����
}

void CChessView::OnBack() 
{
	// TODO: Add your command handler code here
	//���庯��
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
		nMoveNum = 1;  //��ʷ�߷���Ϊ1

		count-- ;
		ChangeSide();
	}

	Invalidate();
	UpdateWindow();
	
	
}

///////////////////////////////////////////////////////////////////////////
//��������
static BOOL l_check = FALSE  ;
static BOOL m_check = FALSE  ;
static BOOL s_check = FALSE  ;
static BOOL set_check = TRUE ;
//����
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

//ҵ��
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
//רҵ
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
//�Զ���
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


//��������
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
