// GeneratorMove.h: interface for the GeneratorMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERATORMOVE_H__09577067_A2A4_405F_836E_D4965AC15EF8__INCLUDED_)
#define AFX_GENERATORMOVE_H__09577067_A2A4_405F_836E_D4965AC15EF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "extern.h"  //����ȫ�ֱ���

class GeneratorMove  
{
public:
	GeneratorMove();
	virtual ~GeneratorMove();
public:
public:
	//Ϊ��ǰ����ucpcSquares��ǰѡ��sdPlayer���������߷�mvs��bCaptureΪtrue��ʾֻ���������߷�
    int GenerateMoves(byte ucpcSquares[256], int sdPlayer, int *mvs, BOOL bCapture = FALSE);  
	//�ж��߷��Ƿ�Ϸ�
	BOOL LegalMove(byte ucpcSquares[256],int sdPlayer, int mv);   
    //�ж϶Է��Ƿ񽫾�
	BOOL GeneratorMove::Checked(byte ucpcSquares[256],int sdPlayer) ;
protected:

};

#endif // !defined(AFX_GENERATORMOVE_H__09577067_A2A4_405F_836E_D4965AC15EF8__INCLUDED_)
