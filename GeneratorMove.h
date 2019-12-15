// GeneratorMove.h: interface for the GeneratorMove class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERATORMOVE_H__09577067_A2A4_405F_836E_D4965AC15EF8__INCLUDED_)
#define AFX_GENERATORMOVE_H__09577067_A2A4_405F_836E_D4965AC15EF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "extern.h"  //导入全局变量

class GeneratorMove  
{
public:
	GeneratorMove();
	virtual ~GeneratorMove();
public:
public:
	//为当前局面ucpcSquares当前选手sdPlayer产生所有走法mvs，bCapture为true表示只产生吃子走法
    int GenerateMoves(byte ucpcSquares[256], int sdPlayer, int *mvs, BOOL bCapture = FALSE);  
	//判断走法是否合法
	BOOL LegalMove(byte ucpcSquares[256],int sdPlayer, int mv);   
    //判断对方是否将军
	BOOL GeneratorMove::Checked(byte ucpcSquares[256],int sdPlayer) ;
protected:

};

#endif // !defined(AFX_GENERATORMOVE_H__09577067_A2A4_405F_836E_D4965AC15EF8__INCLUDED_)
