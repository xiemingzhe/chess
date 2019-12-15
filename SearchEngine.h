// SearchEngine.h: interface for the SearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHENGINE_H__4A058EAC_DB7C_4C90_B821_CFB20859D37C__INCLUDED_)
#define AFX_SEARCHENGINE_H__4A058EAC_DB7C_4C90_B821_CFB20859D37C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "extern.h"

class SearchEngine  
{
public:
	SearchEngine();
	virtual ~SearchEngine();
public:
	//搜索时用于当前结点的数组
	byte ucpcSquares[256];
public:

};

#endif // !defined(AFX_SEARCHENGINE_H__4A058EAC_DB7C_4C90_B821_CFB20859D37C__INCLUDED_)
