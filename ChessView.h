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
//�������,��������ʱ��Ҫʱ�����
#include <TIME.H>
#include <WINDOWS.H>
#include <STDLIB.H>
#include <Mmsystem.h>
//ˮƽѡ��Ի���
#include "SetLevel.h"
//////////////////////////////////////////////////////////////////////////
#include "GeneratorMove.h"  //�����߷�������
#include "SearchEngine.h"
#include "MainFrm.h"

//////////////////////////////////////////////////////////////////////////
//���幦��
static struct HistoryMovsStruct{
	int mv ;    //�߷�
	int pcsrc;  //�����������
	int pcdest; //�յ���������
}HistoryMovs[1024];
static int HistoryLen = 0;

// �û�����ṹ
struct HashItem {
	BYTE ucDepth, ucFlag; //�ñ�����֪ʱ��������ȡ���־���ñ���ֵ�����ͣ�
	short svl;    //��ֵ
	WORD wmv, wReserved; //����߷�
	DWORD dwLock0, dwLock1;//У����
};

// ���ֿ���ṹ
struct BookItem {
	DWORD dwLock;  //�û����е�У����
	WORD wmv, wvl; //����߷���Ȩ��
};


// �������йص�ȫ�ֱ���
static struct {
  int mvResult;                  // �����ߵ���
  int nHistoryTable[65536];      // ��ʷ��
  int mvKillers[LIMIT_DEPTH][2]; // ɱ���߷���
  HashItem HashTable[HASH_SIZE]; // �û���
  int nBookSize;                 // ���ֿ��С
  BookItem BookTable[BOOK_SIZE]; // ���ֿ�
} Search;

////////��ʱ����ṹ//////////////////////////////////////////////////////////////////
// ����ṹ
struct PositionStruct{
	BYTE ucpcSquares[256];   // �����ϵ�����
	int sdPlayer;
	int vlWhite, vlBlack;           // �졢��˫����������ֵ
	int nDistance1, nMoveNum;        // ������ڵ�Ĳ�������ʷ�߷���
	ZobristStruct zobr;             // Zobrist
	MoveStruct mvsList[MAX_MOVES];  // ��ʷ�߷���Ϣ�б�

	// �������
	void ClearBoard(void) {         
		sdPlayer = vlWhite = vlBlack = nDistance1 = 0;
		memset(ucpcSquares, 0, 256);
	    zobr.InitZero();
	}

	//3.�������Ϸ�һö����
	void AddPiece(int sq, int pc) { 
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

	//2.�������ӷ�
	void ChangeSide(void) {        
		sdPlayer = 1 - sdPlayer ;
		//���巽�����������ʹ��ͬһ���棬���Ƕ��ڲ�ͬ���巽������ȫ��ͬ
		zobr.Xor(Zobrist.Player); //���ӷ����߹����Ӻ��������㣬Ȼ����Ƕ�Ӧ���ֵľ����ˡ�       
	}
	// ���(��ʼ��)��ʷ�߷���Ϣ
	void SetIrrev(void) {           
		mvsList[0].Set(0, 0, Checked(), zobr.dwKey);
		nMoveNum = 1;  //��ʷ�߷���Ϊ1
	}

	// 11.�ж��Ƿ񱻶Է�����
	BOOL Checked() const {
		int i, j, sqSrc, sqDst;
		int pcSelfSide, pcOppSide, pcDst, nDelta;
		pcSelfSide = SIDE_TAG(sdPlayer);
		pcOppSide = OPP_SIDE_TAG(sdPlayer);
		// �ҵ������ϵ�˧(��)�����������жϣ�
		
		for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
			if (ucpcSquares[sqSrc] != pcSelfSide + PIECE_KING) {
				continue;
			}
			
			// 1. �ж��Ƿ񱻶Է��ı�(��)����
			if (ucpcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) {
				return TRUE;
			}
			for (nDelta = -1; nDelta <= 1; nDelta += 2) {
				if (ucpcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) {
					return TRUE;
				}
			}
			
			// 2. �ж��Ƿ񱻶Է�������(����(ʿ)�Ĳ�����������)
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
			
			// 3. �ж��Ƿ񱻶Է��ĳ����ڽ���(������˧����)
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
    int setTime ;  //����������ʱ��

protected: // create from serialization only
	CChessView();
	DECLARE_DYNCREATE(CChessView)

// Attributes
public:
	CChessDoc* GetDocument();
	//MoveGenerator mg ; 
	CDC     MemDC ;        //�豸������
    CBitmap bmpBoard;      //���屳��
    CBitmap bmpPieces[24]; //����λͼ����
	CBitmap bmpSelected ;  //ѡ�����ӱ�־λͼ

	bool    bFlipped ;   //�Ƿ�ת
	static int  sdPlayer ;   //�ֵ�˭0 �� 1 ��
	int     sqSelected, mvLast; //ѡ�еĸ���,��һ����
	static  byte ucpcSquares[256];

	//3.////////////////////////////////////////////////////////////////////////
	int vlWhite, vlBlack;           // �졢��˫����������ֵ
    static int nDistance;           // ������ڵ�Ĳ���

// Operations
public:
	HINSTANCE hInst; 
	// ��ʼ��������Ϣ
	void  Startup(void);
	// �������ӷ�
	void  ChangeSide(void);      
	// �������Ϸ�һö����
	void  AddPiece(int sq, int pc) ; 
	// ������������һö����
	void  DelPiece(int sq , int pc) ;       
	// ��һ���������
	int  MovePiece(int mv) ;       
	// ��һ����
	BOOL MakeMove(int mv); 
	
	void UndoMovePiece(int mv, int pcCaptured); // ������һ���������

    BOOL IsMate(void);                          // �ж��Ƿ�ɱ


	//////////////////////////
    void DrawSquare(int,BOOL); 
	void ClickSquare(int);
	//3////////////////////////////////////////////////////////////////////////
	int Evaluate(void) const ;
    void UndoMakeMove();  // ������һ����

    void SearchMain(void) ;  //����������������
    int SearchFull(int vlAlpha, int vlBeta, int nDepth,BOOL bNoNull);// �����߽�(Fail-Soft)��Alpha-Beta��������
	static int CompareHistory(const void *lpmv1, const void *lpmv2);// "qsort"����ʷ������ıȽϺ���
	void ResponseMove(void); //���Ի�Ӧһ����
//4////////////////////////////////////////////////////////////////////////
public:
	 int  nMoveNum;                  // ��ʷ�߷���
	 MoveStruct mvsList[MAX_MOVES];  // ��ʷ�߷���Ϣ�б�
     ZobristStruct zobr;             // Zobrist ����У���룬�������Ϊ��ǰ����У����
	 BOOL bGameOver;                  //�Ƿ���Ϸ����(���ü�������ȥ)

public:
	// ���(��ʼ��)��ʷ�߷���Ϣ
	void SetIrrev(void) {           
		mvsList[0].Set(0, 0, gen.Checked(ucpcSquares,sdPlayer), zobr.dwKey);
		nMoveNum = 1;  //��ʷ�߷���Ϊ1
	}
	BOOL InCheck(void) const {      // �Ƿ񱻽���
		return mvsList[nMoveNum - 1].ucbCheck;
	}
	BOOL Captured(void) const {     // ��һ���Ƿ����
		return mvsList[nMoveNum - 1].ucpcCaptured != 0;
	}

	void NullMove(void) {                       // ��һ���ղ�
		DWORD dwKey;
		dwKey = zobr.dwKey;
		ChangeSide();
		mvsList[nMoveNum].Set(0, 0, FALSE, dwKey);
		nMoveNum ++;
		nDistance ++;
	}

	void UndoNullMove(void) {                   // ������һ���ղ�
		nDistance --;
		nMoveNum --;
		ChangeSide();
	}

	int RepStatus(int nRecur = 1) const ;       // ����ظ�����
	int RepValue(int nRepStatus) const {        // �ظ������ֵ
		int vlReturn;
		vlReturn = ((nRepStatus & 2) == 0 ? 0 : nDistance - MATE_VALUE) +
			((nRepStatus & 4) == 0 ? 0 : MATE_VALUE - nDistance);
		return vlReturn == 0 ? -DRAW_VALUE : vlReturn;
	}
	BOOL NullOkay(void) const {                 // �ж��Ƿ�����ղ��ü�
		return (sdPlayer == 0 ? vlWhite : vlBlack) > NULL_MARGIN;
	}
    
	// ������Դ����
	 void PlayResWav(int nResId) {
		HINSTANCE hins = AfxGetInstanceHandle();
		PlaySound(MAKEINTRESOURCE(nResId), NULL, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
	}

	// ��MVV/LVAֵ
	 static int MvvLva(int mv) {
		return (cucMvvLva[ucpcSquares[DST(mv)]] << 3) - cucMvvLva[ucpcSquares[SRC(mv)]];
	}
	// "qsort"��MVV/LVAֵ����ıȽϺ���
	 static int CompareMvvLva(const void *lpmv1, const void *lpmv2) {
		 return MvvLva(*(int *) lpmv2) - MvvLva(*(int *) lpmv1);
	}


	// ��̬(Quiescence)��������
   int SearchQuiesc(int vlAlpha, int vlBeta);
   //////////////////////////////////////////////////////////////////////////


   // �����ֵ
   int DrawValue(void) const {                 
	   return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
   }
   // ��ȡ�û�����
   int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv) ;
   // �����û�����
   void RecordHash(int nFlag, int vl, int nDepth, int mv) ;

   // �߷�����ṹ
   struct SortStruct{
	   int mvHash, mvKiller1, mvKiller2; // �û����߷�������ɱ���߷�
	   int nPhase, nIndex, nGenMoves;    // ��ǰ�׶Σ���ǰ���õڼ����߷����ܹ��м����߷�
	   int mvs[MAX_GEN_MOVES];           // ���е��߷�
	   GeneratorMove g ;
	   void Init(int mvHash_){ // ��ʼ�����趨�û����߷�������ɱ���߷�
		   mvHash = mvHash_;
		   mvKiller1 = Search.mvKillers[nDistance][0];
		   mvKiller2 = Search.mvKillers[nDistance][1];
           nPhase = PHASE_HASH;
	   }
	   // �õ���һ���߷�
	   int Next(){
		   int mv;
		   switch (nPhase) {
			   // "nPhase"��ʾ�ŷ����������ɽ׶Σ�����Ϊ��
			   
			   // 0. �û����ŷ���������ɺ�����������һ�׶Σ�
		   case PHASE_HASH:
			   nPhase = PHASE_KILLER_1;
			   if (mvHash != 0) {
				   return mvHash;
			   }
			   // ���ɣ�����û��"break"����ʾ"switch"����һ��"case"ִ��������������һ��"case"����ͬ
			   
			   // 1. ɱ���ŷ�����(��һ��ɱ���ŷ�)����ɺ�����������һ�׶Σ�
		   case PHASE_KILLER_1:
			   nPhase = PHASE_KILLER_2;
			   if (mvKiller1 != mvHash && mvKiller1 != 0 && g.LegalMove(ucpcSquares,sdPlayer,mvKiller1)) {
				   return mvKiller1;
			   }
			   
			   // 2. ɱ���ŷ�����(�ڶ���ɱ���ŷ�)����ɺ�����������һ�׶Σ�
		   case PHASE_KILLER_2:
			   nPhase = PHASE_GEN_MOVES;
			   if (mvKiller2 != mvHash && mvKiller2 != 0 && g.LegalMove(ucpcSquares,sdPlayer,mvKiller2)) {
				   return mvKiller2;
			   }
			   
			   // 3. ���������ŷ�����ɺ�����������һ�׶Σ�
		   case PHASE_GEN_MOVES:
			   nPhase = PHASE_REST;
			   nGenMoves = g.GenerateMoves(ucpcSquares,sdPlayer,mvs);
			   qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
			   nIndex = 0;
			   
			   // 4. ��ʣ���ŷ�����ʷ��������
		   case PHASE_REST:
			   while (nIndex < nGenMoves) {
				   mv = mvs[nIndex];
				   nIndex ++;
				   if (mv != mvHash && mv != mvKiller1 && mv != mvKiller2) {
					   return mv;
				   }
			   }	
			   // 5. û���ŷ��ˣ������㡣
		   default:
			   return 0;
		   }
	   }
	};

	   // ������߷��Ĵ���
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
	   void ClearBoard(void) {         // �������
		   sdPlayer = vlWhite = vlBlack = nDistance = 0;
		   memset(ucpcSquares, 0, 256);
		   zobr.InitZero();
	   }

	   void Mirror(PositionStruct &posMirror) ; // �Ծ��澵��
	   // װ�뿪�ֿ�
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


		// �������ֿ�
		 int SearchBook(void) {
			int i, vl, nBookMoves, mv;
			int mvs[MAX_GEN_MOVES], vls[MAX_GEN_MOVES];
			BOOL bMirror;
			BookItem bkToSearch, *lpbk;
			PositionStruct posMirror;
			// �������ֿ�Ĺ��������¼�������
			
			// 1. ���û�п��ֿ⣬����������
			if (Search.nBookSize == 0) {
				return 0;
			}
			// 2. ������ǰ����
			bMirror = FALSE;
			bkToSearch.dwLock = zobr.dwLock1;
			lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
			// 3. ���û���ҵ�����ô������ǰ����ľ������
			if (lpbk == NULL) {
				bMirror = TRUE;
				Mirror(posMirror);
				bkToSearch.dwLock = posMirror.zobr.dwLock1;
				lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
			}
			// 4. ����������Ҳû�ҵ�������������
			if (lpbk == NULL) {
				return 0;
			}
			// 5. ����ҵ�������ǰ���һ�����ֿ���
			while (lpbk >= Search.BookTable && lpbk->dwLock == bkToSearch.dwLock) {
				lpbk --;
			}
			lpbk ++;
			// 6. ���߷��ͷ�ֵд�뵽"mvs"��"vls"������
			vl = nBookMoves = 0;
			while (lpbk < Search.BookTable + Search.nBookSize && lpbk->dwLock == bkToSearch.dwLock) {
				mv = (bMirror ? MIRROR_MOVE(lpbk->wmv) : lpbk->wmv);
				if (gen.LegalMove(ucpcSquares,sdPlayer,mv)) {
					mvs[nBookMoves] = mv;
					vls[nBookMoves] = lpbk->wvl;
					vl += vls[nBookMoves];
					nBookMoves ++;
					if (nBookMoves == MAX_GEN_MOVES) {
						break; // ��ֹ"BOOK.DAT"�к����쳣����
					}
				}
				lpbk ++;
			}
			if (vl == 0) {
				return 0; // ��ֹ"BOOK.DAT"�к����쳣����
			}
			// 7. ����Ȩ�����ѡ��һ���߷�
			vl = rand() % vl;
			for (i = 0; i < nBookMoves; i ++) {
				vl -= vls[i];
				if (vl < 0) {
					break;
				}
			}
			return mvs[i];
		}

		 // ���ڵ��Alpha-Beta��������
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
