#pragma once
#include "qpainter.h"


enum ITEM_TYPE{
	ITEM_SHUAI = 0,	//帅
	ITEM_SHI,		//士
	ITEM_XIANG,		//象
	ITEM_JU,			//车
	ITEM_MA,			//马
	ITEM_PAO,		//炮
	ITEM_BING,		//兵
	ITEM_MAX,
};

enum ITEM_COLOR{
	COLOR_RED = 0,	//红方
	COLOR_BLACK,		//黑方
	COLOR_MAX,
};

class Item
{
public:
	Item(){}
	Item(ITEM_TYPE t,ITEM_COLOR c,QPoint pt);	
	~Item(void);

public:
	ITEM_TYPE m_type;		//棋子类型
	ITEM_COLOR m_color;	//红方黑方

	QPoint m_pt;			//位置
	bool m_bShow;			//是否显示，实现选中时闪烁效果
};

