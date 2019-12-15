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
//������: GenerateMoves
//��  ��: л����
//��  ��: 2013��1��1��
//��  ��: �������кϷ��߷�
//�������: ucpcSquares����ľ���
//          sdPlayer�ֵ�˭���� 0:�� 1:��
//          mvs��������߷�������
//          bCapture�Ƿ�ֻ�������ӱ�� 
//����ֵ:   �߷�����nGenMoves  �߷�mvs 
//˵  ��:   �߷��������ֵ�˭���������
//==================================================================
int GeneratorMove::GenerateMoves(byte ucpcSquares[256], int sdPlayer, int *mvs, BOOL bCapture){
	int i, j, nGenMoves, nDelta, sqSrc, sqDst;
	int pcSelfSide, pcOppSide, pcSrc, pcDst;
	// ���������߷�����Ҫ�������¼������裺

	nGenMoves = 0;                          //��ʼ���߷�����
	pcSelfSide = SIDE_TAG(sdPlayer);        //��ñ������ ��:8 ��:16 
	pcOppSide  = OPP_SIDE_TAG(sdPlayer);    //��öԷ���� ��:8 ��:16 
	for (sqSrc = 0; sqSrc < 256; sqSrc++) {
		
		// 1. �ҵ�һ����������:���̱�źͱ�����ǽ���������,(pcSrc&pcSelfSide!=0)˵���Ǳ�������
		pcSrc = ucpcSquares[sqSrc];
		if ((pcSrc & pcSelfSide) == 0) {
			continue;
		}
		
		// 2. ������������ȷ���߷�
		switch (pcSrc - pcSelfSide) {     //���ӱ�� 0:�� 1:ʿ 2:�� 3:�� 4:�� 5:�� 6:��
		case PIECE_KING:  //��/˧
			for (i = 0; i < 4; i ++) {    //��4�������ƶ�������
				sqDst = sqSrc + ccKingDelta[i]; //ԭλ���ϼ�ָ������
				if (!IN_FORT(sqDst)) {          //Ŀ��λ�ò��ھŹ����ڣ�˵��Խ��
					continue;
				}
				//������˵��Ŀ��λ�úϷ�
				pcDst = ucpcSquares[sqDst];  //��¼Ŀ��λ�õ����̱��
				//bCaptureΪtrueʱ��pcDst & pcOppSide)!=0˵��Ŀ��λ���Ƕ��ֵ�����,�����߷�
				//bCaptureΪfalseʱ,pcDst & pcSelfSide��ֻҪĿ��λ�ò����Լ������Ӷ�������
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
					mvs[nGenMoves] = MOVE(sqSrc, sqDst); //�߷������յ�����߷�������
					nGenMoves ++;    //�߷�����һ
				}
			}
			break;
		case PIECE_ADVISOR:  //  ʿ/��
			for (i = 0; i < 4; i ++) { //�ƶ����������4��
				sqDst = sqSrc + ccAdvisorDelta[i]; // ԭλ�ü�ָ������
				if (!IN_FORT(sqDst)) {  //Ŀ��λ������Ƿ��ھŹ�����
					continue;
				}
				pcDst = ucpcSquares[sqDst]; //Ŀ��λ�����̱��
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
					mvs[nGenMoves] = MOVE(sqSrc, sqDst);
					nGenMoves ++;
				}
			}
			break;
		case PIECE_BISHOP: // ��/��
			for (i = 0; i < 4; i ++) { // �ƶ����������4��
				sqDst = sqSrc + ccAdvisorDelta[i]; //����: ���һ���ƶ����Կ�����ʿ������ͬ�����ƶ�
				//Ŀ��λ����������&Ŀ��λ��δ����&Ŀ��λ��û������(����)
				if (!(IN_BOARD(sqDst) && HOME_HALF(sqDst, sdPlayer) && ucpcSquares[sqDst] == 0)) {
					continue;
				}
				sqDst += ccAdvisorDelta[i];  //������һ�β���(�������Ŀ��λ��)
				pcDst = ucpcSquares[sqDst];  //Ŀ��λ�����̱��
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
					mvs[nGenMoves] = MOVE(sqSrc, sqDst);
					nGenMoves ++;
				}
			}
			break;
		case PIECE_KNIGHT:  //  ��
			//����4������8���ƶ�����
			for (i = 0; i < 4; i ++) {  //ѭ��ÿ��������
				sqDst = sqSrc + ccKingDelta[i]; //���ȵĲ����ͽ�/˧������ͬ
				if (ucpcSquares[sqDst] != 0) {  //����λ����������ֱ�ӷ���
					continue;
				}
				for (j = 0; j < 2; j ++) {  //ÿ�����ȶ�Ӧ�����߷�
					sqDst = sqSrc + ccKnightDelta[i][j]; //ԭλ�ü������ȶ�Ӧ���ƶ�������߷�����
					if (!IN_BOARD(sqDst)) {  //Ŀ��λ�ò��������У�ֱ�ӷ���
						continue;
					}
					pcDst = ucpcSquares[sqDst]; //���Ŀ��λ�����ӱ�ʶ
				   //bCaptureΪtrueʱ��pcDst & pcOppSide)!=0˵��Ŀ��λ���Ƕ��ֵ�����,�����߷�
				   //bCaptureΪfalseʱ,pcDst & pcSelfSide��ֻҪĿ��λ�ò����Լ������Ӷ�������
					if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
						mvs[nGenMoves] = MOVE(sqSrc, sqDst);  //���߷���ӵ��߷�������
						nGenMoves ++;
					}
				}
			}
			break;
		case PIECE_ROOK:  //  �� 
			for (i = 0; i < 4; i ++) {    //��4�ֿ����ƶ��ķ���
				nDelta = ccKingDelta[i];  //�ý��Բ�����Ϊ��ÿ�������һ������
				sqDst = sqSrc + nDelta;   //��i�����ƶ�һ������
				while (IN_BOARD(sqDst)) { //������������
					pcDst = ucpcSquares[sqDst]; //Ŀ��λ�����̱��
					if (pcDst == 0) {    //Ŀ��λ��û�����ӣ��������ǳ����߷�
						if (!bCapture) { //����Ҫ����Ҫֻ���������߷�
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					} else {  //Ŀ��λ��������
						if ((pcDst & pcOppSide) != 0) { //�ǶԷ������ӣ������߷�(���۲����Ƿ�Ҫ���ⶼ��Ҫ��¼���߷�)
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
						break;
					}
					sqDst += nDelta;   //���ƶ�һ������������ͬ������
				}
			}
			break;
		case PIECE_CANNON:  // ��/�h
			for (i = 0; i < 4; i ++) {   //��4�������ƶ�����
				nDelta = ccKingDelta[i];
				sqDst = sqSrc + nDelta;  //ͬ��һ�����ƶ�һ������
				while (IN_BOARD(sqDst)) {
					pcDst = ucpcSquares[sqDst];
					if (pcDst == 0) { //Ŀ��λ��û������
						if (!bCapture) { //���ò�����Ҫ��ֻ���������߷�����¼�߷�
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					} else {  //���Ŀ��λ��������(��̨)������ѭ��
						break;
					}
					sqDst += nDelta;
				}
				//�Ѿ�����̨�ˣ����ڼ����ƶ�
				sqDst += nDelta;  //�ƶ�һ������
				while (IN_BOARD(sqDst)) { 
					pcDst = ucpcSquares[sqDst];
					if (pcDst != 0) {  //������һ������
						if ((pcDst & pcOppSide) != 0) { //�����ǶԷ��ģ���¼�߷�
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
						break;   //ֱ������
					}
					sqDst += nDelta;
				}
			}
			break;
		case PIECE_PAWN:  // ��/��
			sqDst = SQUARE_FORWARD(sqSrc, sdPlayer); // ������ǰ��һ��(�����Ƿ���Ӷ�����ǰ��)
			if (IN_BOARD(sqDst)) {  //Ŀ��λ����������
				pcDst = ucpcSquares[sqDst]; //ȡ��Ŀ��λ�����̱��
				if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) { //�����Ƿ�Ҫ��ֻ���������߷������м�¼
					mvs[nGenMoves] = MOVE(sqSrc, sqDst);
					nGenMoves ++;
				}
			}
			// ����
			if (AWAY_HALF(sqSrc, sdPlayer)) {
				for (nDelta = -1; nDelta <= 1; nDelta += 2) { //���Ӻ���2���߷�������
					sqDst = sqSrc + nDelta;   //����ָ������
					if (IN_BOARD(sqDst)) {    //Ŀ��λ����������
						pcDst = ucpcSquares[sqDst];  //Ŀ��λ�����̱��
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
  return nGenMoves;  //�����߷�����
}

//==================================================================
//������: LegalMove
//��  ��: л����
//��  ��: 2013��1��1��
//��  ��: �ж��߷��Ƿ����
//�������: ucpcSquares �жϵľ���
//          sdPlayer�ֵ�˭���� 0:�� 1:��
//          mvs��������߷�������
//����ֵ:   BOOL���� TRUE�Ϸ� FALSE���Ϸ� 
//˵  ��:   �߷��������ֵ�˭���������
//==================================================================
BOOL GeneratorMove::LegalMove(byte ucpcSquares[256],int sdPlayer, int mv)  {
	int sqSrc, sqDst, sqPin;
	int pcSelfSide, pcSrc, pcDst, nDelta;
	// �ж��߷��Ƿ�Ϸ�����Ҫ�������µ��жϹ��̣�
	
	// 1. �ж���ʼ���Ƿ����Լ�������


	sqSrc = SRC(mv);
	//�����ʼλ�����̱��
	pcSrc = ucpcSquares[sqSrc];  
	//�õ�������� ��:8 ��:16  
	pcSelfSide = SIDE_TAG(sdPlayer); 
	//������λ�ò����Լ�������,����FALSE
	if ((pcSrc & pcSelfSide) == 0) { 
		return FALSE;
	}
	
	// 2. �ж�Ŀ��λ���Ƿ����Լ�������
	sqDst = DST(mv);
	pcDst = ucpcSquares[sqDst];
	if ((pcDst & pcSelfSide) != 0) { //Ŀ��λ�����Լ������ӣ�����FALSE
		return FALSE;
	}
	
	// 3. �������ӵ������ж��߷��Ƿ�Ϸ�
	switch (pcSrc - pcSelfSide) {  //������ӱ��
	case PIECE_KING:   // ��
		return IN_FORT(sqDst) && KING_SPAN(sqSrc, sqDst);   //�ھŹ�����&���Ͻ�����
	case PIECE_ADVISOR:// ʿ
		return IN_FORT(sqDst) && ADVISOR_SPAN(sqSrc, sqDst);//�ھŹ�����&����ʿ����
	case PIECE_BISHOP: // ��
		//�ں�ͬһ��&������Ĳ���&���۴�û������
		return SAME_HALF(sqSrc, sqDst) && BISHOP_SPAN(sqSrc, sqDst) &&
			ucpcSquares[BISHOP_PIN(sqSrc, sqDst)] == 0;
	case PIECE_KNIGHT: // ��
		sqPin = KNIGHT_PIN(sqSrc, sqDst); //����߷��Ϸ�����������λ�ã����򷵻����λ��
		return sqPin != sqSrc && ucpcSquares[sqPin] == 0; //δ�������λ�ã��򷵻�����λ��&����λ��û������
	case PIECE_ROOK:   // ��
	case PIECE_CANNON: // ��
		if (SAME_RANK(sqSrc, sqDst)) { //�����ͬһ�У�ȷ������
			nDelta = (sqDst < sqSrc ? -1 : 1);
		} else if (SAME_FILE(sqSrc, sqDst)) {  //�����ͬһ�У�ȷ������
			nDelta = (sqDst < sqSrc ? -16 : 16);
		} else {  //�Ȳ���ͬһ��Ҳ������һ�У��Ƿ��߷�
			return FALSE;
		}
		//�������ڲ����ӵ�������߷���һ����
		sqPin = sqSrc + nDelta;  //����ָ������
		while (sqPin != sqDst && ucpcSquares[sqPin] == 0) { //δ��Ŀ��λ��&δ������������
			sqPin += nDelta;
		}
		if (sqPin == sqDst) {   //whileѭ���򵽴��յ�λ�ö�����
			//��2���Ѿ��ж��ˣ�Ŀ��λ�������Լ������ӣ���������߲�������
			return pcDst == 0 || pcSrc - pcSelfSide == PIECE_ROOK; //�յ�λ��û������||������(�϶��Ƕ��ֵ�����)������������ǳ�(����)��
		} else if (pcDst != 0 && pcSrc - pcSelfSide == PIECE_CANNON) { //δ���յ�λ�õ�ǰ����(�϶�����Ϊ����������while������)���յ�λ��������(�Է���)&�����������
			sqPin += nDelta;  //����ָ������
			while (sqPin != sqDst && ucpcSquares[sqPin] == 0) { //����ɨ�裬�Ƿ�����������
				sqPin += nDelta;
			}
			return sqPin == sqDst; //ΪTRUE˵����ɨ�赽�յ�λ�ã�Ҳû���ٷ������ӣ�Ҳ������̨�Ϸ�
		} else {  //������������ǷǷ���
			return FALSE;
		}
	case PIECE_PAWN:  //  ��/��
		//����&���������������һ�����Ϸ���
		if (AWAY_HALF(sqDst, sdPlayer) && (sqDst == sqSrc - 1 || sqDst == sqSrc + 1)) {
			return TRUE;
		}
		return sqDst == SQUARE_FORWARD(sqSrc, sdPlayer);  //���۹������ֻҪ����ǰ��һ�������ǺϷ��ġ�
	default: //���򷵻�FALSE
		return FALSE;
	}
}

/*
  ��ʵ�ж�˧(��)�Ƿ񱻽����Ĺ��̲������ӣ�
  (1) ����˧(��)�ǳ����ж����Ƿ��ܳԵ��Է��ĳ��ͽ�(˧)(�й��������н�˧���ܶ����Ĺ���)��
  (2) ����˧(��)���ڣ��ж����Ƿ��ܳԵ��Է����ڣ�
  (3) ����˧(��)�����ж����Ƿ��ܳԵ��Է�������Ҫע����ǣ�˧(��)�������õ������� ccAdvisorDelta�������� ccKingDelta��
  (4) ����˧(��)�ǹ��ӵı�(��)���ж����Ƿ��ܳԵ��Է�����(��)��
*/
//==================================================================
//������: Checked
//��  ��: л����
//��  ��: 2013��1��1��
//��  ��: �ж��Ƿ��ڱ�������״̬
//�������: ucpcSquares �жϵľ���
//          sdPlayer�ֵ�˭���� 0:�� 1:��
//          mvs��������߷�������
//����ֵ:   BOOL���� TRUE�Ϸ� FALSE���Ϸ� 
//˵  ��:   �߷��������ֵ�˭���������
//==================================================================
BOOL GeneratorMove::Checked(byte ucpcSquares[256],int sdPlayer) {
	int i, j, sqSrc, sqDst;
	int pcSelfSide, pcOppSide, pcDst, nDelta;
	pcSelfSide = SIDE_TAG(sdPlayer);
	pcOppSide = OPP_SIDE_TAG(sdPlayer);
	// �ҵ������ϵ�˧(��)�����������жϣ�
	
	for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
		if (ucpcSquares[sqSrc] != pcSelfSide + PIECE_KING) {  //�ҵ������Ľ�˧
			continue;
		}
		
		// 1. �ж��Ƿ񱻶Է��ı�(��)����
		if (ucpcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) { //��������䣬ǰ��һ�����Ƿ���Ե���Է����λ��
			return TRUE;
		}
		for (nDelta = -1; nDelta <= 1; nDelta += 2) {
			if (ucpcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) { //��������䣬������һ�����Ƿ���Ե���Է����λ��
				return TRUE;
			}
		}
		
		// 2. �ж��Ƿ񱻶Է�������(����(ʿ)�Ĳ�����������)
		for (i = 0; i < 4; i ++) {
			//������ǶԷ����������ȵ�λ�þͲ��ܰ��Է����������ˣ����ǰ�ʿ�Ĳ�����Ϊ����λ��(����)��
			if (ucpcSquares[sqSrc + ccAdvisorDelta[i]] != 0) { 
				continue;
			}
			for (j = 0; j < 2; j ++) {
				pcDst = ucpcSquares[sqSrc + ccKnightCheckDelta[i][j]];
				if (pcDst == pcOppSide + PIECE_KNIGHT) {    //������������Ƿ���Ե���Է����λ��
					return TRUE;
				}
			}
		}
		// 3. �ж��Ƿ񱻶Է��ĳ����ڽ���(������˧����)
		for (i = 0; i < 4; i ++) {    //4������
			nDelta = ccKingDelta[i];  //����
			sqDst = sqSrc + nDelta;
			while (IN_BOARD(sqDst)) {  //Ѱ�ҶԷŵĳ�
				pcDst = ucpcSquares[sqDst];
				if (pcDst != 0) {  //Ŀ��λ�������ӣ��ǶԷ��Ľ�(����)�����ǶԷ��ĳ������Ǵ��ڽ���״̬
					if (pcDst == pcOppSide + PIECE_ROOK || pcDst == pcOppSide + PIECE_KING) {
						return TRUE;
					}
					break; //���������ӣ����ǲ�����Ȥ��ֱ�ӷ���
				}
				sqDst += nDelta;
			}
			//while������ΪԽ�����ֹ�����Ǹ���Ȥ������Ϊ�������Ӷ���ֹ��(��̨)
			sqDst += nDelta;
			while (IN_BOARD(sqDst)) {
				int pcDst = ucpcSquares[sqDst];
				if (pcDst != 0) {
					if (pcDst == pcOppSide + PIECE_CANNON) {  //����ɨ��ʱ���ҵ��Է�����
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
