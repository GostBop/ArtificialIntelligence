#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"
#include "qevent.h"
#include "qmessagebox.h"
#include <QTimer>
#include <QVector>
#include <iostream>
#include <QDebug>

#define CHESS_ROWS		9
#define CHESS_COLUMES	8
#define RECT_WIDTH		36
#define RECT_HEIGHT		37

#define START_X	(26-RECT_WIDTH/2)           //棋盘左上定点像素值（35，45），棋盘的格子大小（96，96）
#define START_Y	(38-RECT_HEIGHT/2)

//各子的控制区域价值（所在的位置的价值）
const int PositionValues[8][90] = {
  {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  },
  { // 帅
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    1, -8, -9,  0,  0,  0,  0,  0,  0,  0,
    5, -8, -9,  0,  0,  0,  0,  0,  0,  0,
    1, -8, -9,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  },
  { // 士
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  3,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  },
  { // 相
    0,  0, -2,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  3,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0, -2,  0,  0,  0,  0,  0,  0,  0
  },
  { // 马
     0, -3,  5,  4,  2,  2,  5,  4,  2,  2,
    -3,  2,  4,  6, 10, 12, 20, 10,  8,  2,
     2,  4,  6, 10, 13, 11, 12, 11, 15,  2,
     0,  5,  7,  7, 14, 15, 19, 15,  9,  8,
     2,-10,  4, 10, 15, 16, 12, 11,  6,  2,
     0,  5,  7,  7, 14, 15, 19, 15,  9,  8,
     2,  4,  6, 10, 13, 11, 12, 11, 15,  2,
    -3,  2,  4,  6, 10, 12, 20, 10,  8,  2,
     0, -3,  5,  4,  2,  2,  5,  4,  2,  2
  },
  { // 车
    -6,  5, -2,  4,  8,  8,  6,  6,  6,  6,
     6,  8,  8,  9, 12, 11, 13,  8, 12,  8,
     4,  6,  4,  4, 12, 11, 13,  7,  9,  7,
    12, 12, 12, 12, 14, 14, 16, 14, 16, 13,
     0,  0, 12, 14, 15, 15, 16, 16, 33, 14,
    12, 12, 12, 12, 14, 14, 16, 14, 16, 13,
     4,  6,  4,  4, 12, 11, 13,  7,  9,  7,
     6,  8,  8,  9, 12, 11, 13,  8, 12,  8,
    -6,  5, -2,  4,  8,  8,  6,  6,  6,  6
  },
  { // 炮
    0,  0,  1,  0, -1,  0,  0,  1,  2,  4,
    0,  1,  0,  0,  0,  0,  3,  1,  2,  4,
    1,  2,  4,  0,  3,  0,  3,  0,  0,  0,
    3,  2,  3,  0,  0,  0,  2, -5, -4, -5,
    3,  2,  5,  0,  4,  4,  4, -4, -7, -6,
    3,  2,  3,  0,  0,  0,  2, -5, -4, -5,
    1,  2,  4,  0,  3,  0,  3,  0,  0,  0,
    0,  1,  0,  0,  0,  0,  3,  1,  2,  4,
    0,  0,  1,  0, -1,  0,  0,  1,  2,  4
  },
  { // 兵
    0,  0,  0, -2,  3, 10, 20, 20, 20,  0,
    0,  0,  0,  0,  0, 18, 27, 30, 30,  0,
    0,  0,  0, -2,  4, 22, 30, 45, 50,  0,
    0,  0,  0,  0,  0, 35, 40, 55, 65,  2,
    0,  0,  0,  6,  7, 40, 42, 55, 70,  4,
    0,  0,  0,  0,  0, 35, 40, 55, 65,  2,
    0,  0,  0, -2,  4, 22, 30, 45, 50,  0,
    0,  0,  0,  0,  0, 18, 27, 30, 30,  0,
    0,  0,  0, -2,  3, 10, 20, 20, 20,  0
  }
};

//-----------------------------------------------------------------------//
//设置界面大小、图像，启动定时器
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _level = 4;
    ui->setupUi(this);
	ui->mainToolBar->hide();
	ui->menuBar->hide();

    //窗口以棋盘图片为大小
    QPixmap pixChessBoard(":/image/image/ChessBoard.png");
    setFixedSize(pixChessBoard.size());

	QPalette palette;
	palette.setBrush( QPalette::Window,QBrush(pixChessBoard));
	setPalette(palette); 
	
	m_ItemsImage = QPixmap(":/image/image/items.png");
    m_nItemWidth = m_ItemsImage.width() / COLOR_MAX;
    m_nItemHeight = m_ItemsImage.height() / ITEM_MAX;

	NewGame();
	startTimer(500);		//启动显隐选中元素的定时器
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent( QPaintEvent * )
{
	QPainter painter(this);
	for (int i = 0; i<m_items.size(); i++)
	{
		DrawItem(painter,m_items[i]);
	}
	update();
}

void MainWindow::DrawItem(QPainter& painter, Item item )
{
	if (!item.m_bShow)
	{
		return;
	}
	QRect rcSrc(m_nItemWidth*item.m_color,m_nItemHeight*item.m_type,m_nItemWidth,m_nItemHeight);

	QRect rcTarget(	START_X + item.m_pt.x()*RECT_WIDTH,
					START_Y + item.m_pt.y()*RECT_HEIGHT,
					RECT_WIDTH,RECT_HEIGHT);

	painter.drawPixmap(rcTarget,m_ItemsImage,rcSrc);
}

//-----------------------------------------------------------------------//

//添加棋子到m_items
void MainWindow::InitItems()
{
	m_items.clear();

	//初始化所有黑方棋子
	Item item1(ITEM_JU,COLOR_BLACK,QPoint(0,0));
	Item item2(ITEM_MA,COLOR_BLACK,QPoint(1,0));
	Item item3(ITEM_XIANG,COLOR_BLACK,QPoint(2,0));
	Item item4(ITEM_SHI,COLOR_BLACK,QPoint(3,0));
	Item item5(ITEM_SHUAI,COLOR_BLACK,QPoint(4,0));
	Item item6(ITEM_SHI,COLOR_BLACK,QPoint(5,0));
	Item item7(ITEM_XIANG,COLOR_BLACK,QPoint(6,0));
	Item item8(ITEM_MA,COLOR_BLACK,QPoint(7,0));
	Item item9(ITEM_JU,COLOR_BLACK,QPoint(8,0));
	Item item10(ITEM_PAO,COLOR_BLACK,QPoint(1,2));
	Item item11(ITEM_PAO,COLOR_BLACK,QPoint(7,2));
	Item item12(ITEM_BING,COLOR_BLACK,QPoint(0,3));
	Item item13(ITEM_BING,COLOR_BLACK,QPoint(2,3));
	Item item14(ITEM_BING,COLOR_BLACK,QPoint(4,3));
	Item item15(ITEM_BING,COLOR_BLACK,QPoint(6,3));
	Item item16(ITEM_BING,COLOR_BLACK,QPoint(8,3));

	//初始化所有红方棋子
	Item item_1(ITEM_JU,COLOR_RED,QPoint(0,9));
	Item item_2(ITEM_MA,COLOR_RED,QPoint(1,9));
	Item item_3(ITEM_XIANG,COLOR_RED,QPoint(2,9));
	Item item_4(ITEM_SHI,COLOR_RED,QPoint(3,9));
	Item item_5(ITEM_SHUAI,COLOR_RED,QPoint(4,9));
	Item item_6(ITEM_SHI,COLOR_RED,QPoint(5,9));
	Item item_7(ITEM_XIANG,COLOR_RED,QPoint(6,9));
	Item item_8(ITEM_MA,COLOR_RED,QPoint(7,9));
	Item item_9(ITEM_JU,COLOR_RED,QPoint(8,9));
	Item item_10(ITEM_PAO,COLOR_RED,QPoint(1,7));
	Item item_11(ITEM_PAO,COLOR_RED,QPoint(7,7));
	Item item_12(ITEM_BING,COLOR_RED,QPoint(0,6));
	Item item_13(ITEM_BING,COLOR_RED,QPoint(2,6));
	Item item_14(ITEM_BING,COLOR_RED,QPoint(4,6));
	Item item_15(ITEM_BING,COLOR_RED,QPoint(6,6));
	Item item_16(ITEM_BING,COLOR_RED,QPoint(8,6));

	m_items.push_back(item1);
	m_items.push_back(item2);
	m_items.push_back(item3);
	m_items.push_back(item4);
	m_items.push_back(item5);
	m_items.push_back(item6);
	m_items.push_back(item7);
	m_items.push_back(item8);
	m_items.push_back(item9);
	m_items.push_back(item10);
	m_items.push_back(item11);
	m_items.push_back(item12);
	m_items.push_back(item13);
	m_items.push_back(item14);
	m_items.push_back(item15);
	m_items.push_back(item16);
	
	m_items.push_back(item_1);
	m_items.push_back(item_2);
	m_items.push_back(item_3);
	m_items.push_back(item_4);
	m_items.push_back(item_5);
	m_items.push_back(item_6);
	m_items.push_back(item_7);
	m_items.push_back(item_8);
	m_items.push_back(item_9);
	m_items.push_back(item_10);
	m_items.push_back(item_11);
	m_items.push_back(item_12);
	m_items.push_back(item_13);
	m_items.push_back(item_14);
	m_items.push_back(item_15);
	m_items.push_back(item_16);

}

//鼠标点击，走棋、吃棋等逻辑
void MainWindow::mousePressEvent(QMouseEvent * e)
{
    if(!m_bIsRedTurn) {
        return;
    }
	//获得鼠标点所对应的棋盘点pt
	QPoint pt;
	pt.setX( (e->pos().x() - START_X ) / RECT_WIDTH);
	pt.setY( (e->pos().y() - START_Y ) / RECT_HEIGHT);

	//是否有选中的棋子
	if(m_bExistSelectedItem)
	{
		//已存在棋子，判断鼠标点击的是否是选中棋子
		if (pt == m_SelectedItem.m_pt)
		{
			//再次点击已经选择的棋子，什么也不做
			return;
		}

        //点击其它棋子(同色)
		Item ClickedItem;
		if (FindItemAtPoint(pt,ClickedItem))
		{
			//点击的同色的另外一个棋子，改选
			if ( (m_bIsRedTurn && ClickedItem.m_color == COLOR_RED) ||
				 (!m_bIsRedTurn && ClickedItem.m_color != COLOR_RED))
			{
				SetItemShow(m_SelectedItem,true);
				m_SelectedItem = ClickedItem;
				return;
			}
		}

        //获取已选择棋子的可移动区域
		QVector<QPoint> moveArea;
        GetMoveArea(m_SelectedItem,moveArea);
		if (moveArea.contains(pt))					
        {   //点击的异色棋子
			//包含当前鼠标点中的棋子，则能吃
			bool bDeleteSHUAI = false;
			DeleteItemAtPoint(pt,bDeleteSHUAI);
            //移动棋子
			ChangeItemPoint(m_SelectedItem.m_pt,pt);
            //游戏结束
			if (bDeleteSHUAI)
			{
                QString str = m_bIsRedTurn?QStringLiteral("红方胜利！    "):QStringLiteral("黑方胜利！      ");
				QMessageBox::information(NULL,  "GAME OVER  ",str, QMessageBox::Yes , QMessageBox::Yes);
				NewGame();
				return ;
			}
			m_bExistSelectedItem = false;
			m_bIsRedTurn = !m_bIsRedTurn;
            calc_bIsRedTurn = false;
			update();
            //QTimer::singleShot(100, this, SLOT(computerMove()));
            computerMove();
		}
		else
		{
			//不能走到该位置
			return ;
		}
	}
	else
	{
		//当前没有选中棋子
		Item ClickedItem;
		if (FindItemAtPoint(pt,ClickedItem))
		{
			//如果点中一个棋子,是当前走棋方的颜色，就选中了
			if ( (m_bIsRedTurn && ClickedItem.m_color == COLOR_RED) ||
				(!m_bIsRedTurn && ClickedItem.m_color == COLOR_BLACK))
			{
				m_SelectedItem = ClickedItem;
				m_bExistSelectedItem = true;
				return;
			}
		}
	}
}

//查找m_items中是否有棋子在位置pt，有则把item设为这个棋子（判断是否选中其它棋子时使用）
bool MainWindow::FindItemAtPoint(QPoint pt,Item& item)
{
    for (int i = 0; i<m_items.size(); i++)
    {
        if (m_items[i].m_pt == pt)
        {
            item = m_items[i];
            return true;
        }
    }
    return false;
}


//把item移到位置pt（包括删除位置pt当前的棋子）
bool MainWindow::MoveItem(Item item,QPoint pt)
{
    QVector<QPoint> moveArea;
    GetMoveArea(item,moveArea);
    if (moveArea.contains(pt))
    {
        bool bDeleteSHUAI = false;
        DeleteItemAtPoint(pt,bDeleteSHUAI);
        ChangeItemPoint(item.m_pt,pt);
        if (bDeleteSHUAI)
        {
            QString str = m_bIsRedTurn?QStringLiteral("红方胜利！"):QStringLiteral("黑方胜利！");
            QMessageBox::information(NULL,  "GAME OVER  ",str, QMessageBox::Yes , QMessageBox::Yes);
            NewGame();
        }
        return true;
    }
    else
    {
        return false;
    }
}

//根据棋子属性判断能移动的范围（想移动棋子，首先判断棋子能移动的范围）
void MainWindow::GetMoveArea(Item item,QVector<QPoint>& moveArea)
{
    switch (item.m_type)
    {
    case ITEM_JU:
        {
            GetMoveArea_JU(item,moveArea);
            break;
        }
    case ITEM_MA:
        {
            GetMoveArea_MA(item,moveArea);
            break;
        }
    case ITEM_XIANG:
        {
            GetMoveArea_XIANG(item,moveArea);
            break;
        }
    case ITEM_SHI:
        {
            GetMoveArea_SHI(item,moveArea);
            break;
        }
    case ITEM_SHUAI:
        {
            GetMoveArea_SHUAI(item,moveArea);
            break;
        }
    case ITEM_PAO:
        {
            GetMoveArea_PAO(item,moveArea);
            break;
        }
    case ITEM_BING:
        {
            GetMoveArea_BING(item,moveArea);
            break;
        }
    }
}

//查找m_items中是否有棋子在位置pt，有则删掉这个棋子，并判断这个棋子是否为‘帅’（要移动棋子，先删除目标位置的棋子）
bool MainWindow::DeleteItemAtPoint(QPoint pt,bool& bDeleteSHUAI)
{
    bool bDeleted = false;
    for (int i = 0; i<m_items.size(); i++)
    {
        if (m_items[i].m_pt == pt)
        {
            bDeleteSHUAI = (m_items[i].m_type == ITEM_SHUAI);

            m_items.erase(m_items.begin()+i);
            bDeleted = true;
            break;
        }
    }
    return bDeleted;
}

//将棋子移动到位置pt（m_items中寻找位置与ptItem相同的点，将其位置改为pt，并且不再选中点）
void MainWindow::ChangeItemPoint(QPoint ptItem,QPoint pt)
{
    for (int i = 0; i<m_items.size(); i++)
    {
        if (m_items[i].m_pt == ptItem)
        {
            m_SelectedItem.m_pt = QPoint(-1,-1);
            m_items[i].m_pt = pt;
            m_items[i].m_bShow = true; // ?
            break;
        }
    }
}

//-----------------------------------------------------------------------//

//机器走
void MainWindow::computerMove() {
    if(m_bIsRedTurn) return;

    Step * step = getBestMove();
    if(step != NULL) {
        MoveItem(step->move_item,step->to_pt);
    }
    m_bIsRedTurn = true;
    calc_bIsRedTurn = true;
    update();
}

//获得当前所有可能的走法，评估每种走法的分数，选择最好的
Step * MainWindow::getBestMove() {

    QVector<Step*> steps1;

    int maxInAllMinScore = -1000000;
    Step * ret = NULL;
    getAllPossibleMove(steps1);

    while(steps1.count())
    {
        Step * step = steps1.last();
        steps1.removeLast();

        fakeMove(step);
        calc_bIsRedTurn = !calc_bIsRedTurn;
        int minScore = getMinScore(_level-1, maxInAllMinScore);
        calc_bIsRedTurn = !calc_bIsRedTurn;
        unfakeMove(step);

        //选择极大值
        if (minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
            if (ret) {
                ret = NULL;
            }
            ret = step;
        }
        else
        {
            delete step;
            step = NULL;
        }
    }

    return ret;
}

//获得当前所有可行的step
void MainWindow::getAllPossibleMove(QVector<Step *> &steps1)
{
    for (int i = 0; i<m_items.size(); i++)
    {
        if ( (calc_bIsRedTurn && m_items[i].m_color == COLOR_RED) ||
            (!calc_bIsRedTurn && m_items[i].m_color == COLOR_BLACK))
        {
            QVector<QPoint> moveArea;
            GetMoveArea(m_items[i],moveArea);
            for(int j = 0; j < moveArea.count(); j++) {
                Step* s = new Step();
                s->move_item = m_items[i];
                Item temp;
                temp.m_pt = QPoint(-1,-1);
                FindItemAtPoint(moveArea[j], temp);
                s->kill_item = temp;
                s->from_pt = m_items[i].m_pt;
                s->to_pt = moveArea[j];
                steps1.append(s);
            }
        }

    }

}

void MainWindow::fakeMove(Step *step)
{
    Item temp;
    if(FindItemAtPoint(step->to_pt, temp)) {
        step->kill_item = temp;
        bool bDeleteSHUAI = false;
        DeleteItemAtPoint(step->to_pt,bDeleteSHUAI);
    }
    ChangeItemPoint(step->from_pt,step->to_pt);
    step->move_item.m_pt = step->to_pt;
}

void MainWindow::unfakeMove(Step *step)
{
    ChangeItemPoint(step->to_pt,step->from_pt);
    step->move_item.m_pt = step->from_pt;

    if(step->kill_item.m_pt != QPoint(-1,-1)) {
        Item kill_item(step->kill_item.m_type,step->kill_item.m_color,step->kill_item.m_pt);
        m_items.append(kill_item);
    }
}

int MainWindow::getMinScore(int level, int curMinScore)
{
    //到达最大搜索深度，返回分数
    if (level == 0)
        return calcScore();

    //获取当前分支所有可行的step
    QVector<Step*> steps1;
    getAllPossibleMove(steps1);
    int minInAllMaxScore = 1000000;

    while(steps1.count())
    {
        Step * step = steps1.last();
        steps1.removeLast();

        fakeMove(step);
        calc_bIsRedTurn = !calc_bIsRedTurn;
        int maxScore = getMaxScore(level - 1, minInAllMaxScore);
        calc_bIsRedTurn = !calc_bIsRedTurn;
        unfakeMove(step);

        step = NULL;

        //剪枝，因为这一分支所得的最大分数比别的分支的还要小，黑方绝对不会选这一分支
        if (maxScore <= curMinScore)
        {
            while(steps1.count())
            {
                Step* step = steps1.last();
                steps1.removeLast();
                step = NULL;
            }
            return maxScore;
        }

        if(maxScore < minInAllMaxScore)
        {
            minInAllMaxScore = maxScore;
        }
    }
    return minInAllMaxScore;
}

int MainWindow::getMaxScore(int level, int curMaxScore)
{
    if (level == 0)
        return calcScore();

    QVector<Step*> steps1;
    getAllPossibleMove(steps1);
     int maxInAllMinScore = -1000000;

     while(steps1.count())
     {
         Step * step = steps1.last();
         steps1.removeLast();
         fakeMove(step);
         calc_bIsRedTurn = !calc_bIsRedTurn;
         int minScore = getMinScore(level - 1, maxInAllMinScore);
         unfakeMove(step);
         calc_bIsRedTurn = !calc_bIsRedTurn;
         step = NULL;

         if(minScore >= curMaxScore)
         {
             while(steps1.count())
             {
                 Step* step = steps1.last();
                 steps1.removeLast();
                 step = NULL;
             }
             return minScore;
         }

         if(minScore > maxInAllMinScore)
         {
             maxInAllMinScore = minScore;
         }

     }
     return maxInAllMinScore;
}

//评估函数
int MainWindow::calcScore()
{
    int redTotScore = 0;
    int blackTotScore = 0;

    /*
    enum ITEM_TYPE{
        ITEM_SHUAI = 0,	//帅10000
        ITEM_SHI,		//士150
        ITEM_XIANG,		//象150
        ITEM_JU,			//车700
        ITEM_MA,			//马320
        ITEM_PAO,		//炮300
        ITEM_BING,		//兵100
        ITEM_MAX,
    };*/
    static int chessScore[] = {10000, 150, 150, 700, 320, 300, 100};

    for (int i = 0; i<m_items.size(); i++)
    {

        if (m_items[i].m_color == COLOR_RED)
        {
            redTotScore += chessScore[m_items[i].m_type];
            int position = m_items[i].m_pt.x() * 10 + m_items[i].m_pt.y();
            switch (m_items[i].m_type)
            {
            case ITEM_JU:
                {
                    redTotScore += PositionValues[5][position];
                    break;
                }
            case ITEM_MA:
                {
                    redTotScore += PositionValues[4][position];
                    break;
                }
            case ITEM_XIANG:
                {
                    redTotScore += PositionValues[3][position];
                    break;
                }
            case ITEM_SHI:
                {
                    redTotScore += PositionValues[2][position];
                    break;
                }
            case ITEM_SHUAI:
                {
                    redTotScore += PositionValues[1][position];
                    break;
                }
            case ITEM_PAO:
                {
                    redTotScore += PositionValues[6][position];
                    break;
                }
            case ITEM_BING:
                {
                    redTotScore += PositionValues[7][position];
                    break;
                }
            }

        }
        else if (m_items[i].m_color == COLOR_BLACK)
        {
            blackTotScore += chessScore[m_items[i].m_type];
            int position = m_items[i].m_pt.x() * 10 + 9 - m_items[i].m_pt.y();
            switch (m_items[i].m_type)
            {
            case ITEM_JU:
                {
                    blackTotScore += PositionValues[5][position];
                    break;
                }
            case ITEM_MA:
                {
                    blackTotScore += PositionValues[4][position];
                    break;
                }
            case ITEM_XIANG:
                {
                    blackTotScore += PositionValues[3][position];
                    break;
                }
            case ITEM_SHI:
                {
                    blackTotScore += PositionValues[2][position];
                    break;
                }
            case ITEM_SHUAI:
                {
                    blackTotScore += PositionValues[1][position];
                    break;
                }
            case ITEM_PAO:
                {
                    blackTotScore += PositionValues[6][position];
                    break;
                }
            case ITEM_BING:
                {
                    blackTotScore += PositionValues[7][position];
                    break;
                }
            }
        }
    }

    return blackTotScore - redTotScore;
}

//-----------------------------------------------------------------------//

void MainWindow::GetMoveArea_JU(Item item,QVector<QPoint>& moveArea)
{
	//棋子“车”的计算可移动区域算法简介：
	//1，向4个方向上依次遍历点，把空点都加入。
	//当碰到棋子时停止，如果是同色棋子，不加入；是异色棋子，则加入（该处认为可走，因为可以吃子）
	//左边
	int x = item.m_pt.x() - 1;
	while(x>=0)
	{
		QPoint ptNew = QPoint(x,item.m_pt.y());
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (item.m_color != item2.m_color)
			{
				moveArea.append(ptNew);
			}
			break;
		}
		moveArea.append(ptNew);
		x--;
	}
	//右边
	x = item.m_pt.x() + 1;
	while(x<=8)
	{
		QPoint ptNew = QPoint(x,item.m_pt.y());
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (item.m_color != item2.m_color)
			{
				moveArea.append(ptNew);
			}
			break;
		}
		moveArea.append(ptNew);
		x++;
	}

	//上边
	int y = item.m_pt.y() - 1;
	while(y>=0)
	{
		QPoint ptNew = QPoint(item.m_pt.x(),y);
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (item.m_color != item2.m_color)
			{
				moveArea.append(ptNew);
			}
			break;
		}
		moveArea.append(ptNew);
		y--;
	}
	//下方
	y = item.m_pt.y() + 1;
	while(y<=9)
	{
		QPoint ptNew = QPoint(item.m_pt.x(),y);
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (item.m_color != item2.m_color)
			{
				moveArea.append(ptNew);
			}
			break;
		}
		moveArea.append(ptNew);
		y++;
	}
}

void MainWindow::GetMoveArea_MA( Item item,QVector<QPoint>& moveArea )
{
	//棋子“马”的计算可移动区域算法简介：
	//1，求出8个待选位置，8个位置的偏移是（-2，-1）（-2，1）（2，-1）（2，1）（1，-2）（1，2）（-1，-2）（-1，2）存在关系：|x|+|y|=3
	//2，判断待选位置是否在棋盘内
	//3，判断中间是否有卡位的棋子
	//4，位置上是否已存在同色棋子
	Item item2;
	for (int i = -2; i<=2; i++)
	{
		for(int j = -2; j<=2; j++)
		{
			if (qAbs(i) + qAbs(j) == 3)
			{
				QPoint ptNew = item.m_pt + QPoint(i,j);
				if (ptNew.x() >= 0 && ptNew.x() <= 8 && ptNew.y()>=0 && ptNew.y() <= 9)
				{

				}
				else
				{
					continue;
				}

				//求该方向行走路线的 卡位元素位置
				QPoint ptDirect(0,0);	
				if (qAbs(i) > qAbs(j))
				{
					if (i>0)
					{
						ptDirect = QPoint(1,0);
					}
					else
					{
						ptDirect = QPoint(-1,0);
					}
				}
				else
				{
					if (j>0)
					{
						ptDirect = QPoint(0,1);
					}
					else
					{
						ptDirect = QPoint(0,-1);
					}
				}
				QPoint ptHit = item.m_pt + ptDirect;	//马的卡位元素位置
				if (FindItemAtPoint(ptHit,item2))
				{
					//卡位
					continue;
				}
				if (FindItemAtPoint(ptNew ,item2) && item.m_color == item2.m_color)
				{
					//有本组item
					continue;
				}
				moveArea.append(ptNew);
			}
		}
	}
}

void MainWindow::GetMoveArea_XIANG( Item item,QVector<QPoint>& moveArea )
{
	//棋子“相”的计算可移动区域算法简介：
	//1，求出4个待选位置
	//2，判断是否在该“相”对应的半边棋盘内
	//3，判断中间是否有卡位的棋子
	//4，位置上是否已存在同色棋子
	QVector<QPoint> directions;
	directions.push_back(QPoint(-2,-2));
	directions.push_back(QPoint(2,-2));
	directions.push_back(QPoint(2,2));
	directions.push_back(QPoint(-2,2));

	for (int i = 0; i<directions.size(); i++)
	{
		QPoint ptNew = item.m_pt + directions[i];									//移动后的点
		if (item.m_pt.y() <= 4)														//上方的
		{
			if (ptNew.x() >= 0 && ptNew.x() <= 8 && ptNew.y()>=0 && ptNew.y() <= 4)		//移动后需仍在上半棋盘
			{

			}
			else
			{
				continue;
			}
		}
		else
		{
			if (ptNew.x() >= 0 && ptNew.x() <= 8 && ptNew.y()>=5 && ptNew.y() <= 9)		//移动后需仍在下半棋盘
			{

			}
			else
			{
				continue;
			}
		}

		QPoint ptHit = item.m_pt + QPoint(directions[i].x()/2,directions[i].y()/2);		//卡位点在中点
		Item item2;
		if (FindItemAtPoint(ptHit,item2))
		{
			//存在卡位棋子
			continue;
		}
		if (FindItemAtPoint(ptNew ,item2) && item.m_color == item2.m_color)
		{
			//终点是同色的棋子
			continue;
		}
		moveArea.append(ptNew);
	}
}

void MainWindow::GetMoveArea_SHI( Item item,QVector<QPoint>& moveArea )
{
	//移动算法与相类似
	//4个待选的位置偏移
	QVector<QPoint> directions;
	directions.push_back(QPoint(-1,-1));
	directions.push_back(QPoint(1,-1));
	directions.push_back(QPoint(1,1));
	directions.push_back(QPoint(-1,1));

	for (int i = 0; i<directions.size(); i++)
	{
		QPoint ptNew = item.m_pt + directions[i];									//移动后的点
		if (item.m_pt.y() <= 4)														//上方的
		{
			if (ptNew.x() >= 3 && ptNew.x() <= 5 && ptNew.y()>=0 && ptNew.y() <= 2)		//移动后在上棋盘的(3,0)~(5,2)田字内
			{

			}
			else
			{
				continue;
			}
		}
		else
		{
			if (ptNew.x() >= 3 && ptNew.x() <= 5 && ptNew.y()>=7 && ptNew.y() <= 9)		//移动后需仍在下半棋盘
			{

			}
			else
			{
				continue;
			}
		}

		//QPoint ptHit = item.m_pt + QPoint(directions[i].x()/2,directions[i].y()/2);		//卡位点在中点
		Item item2;
		//if (FindItemAtPoint(ptHit,item2))
		//{
		//	//存在卡位的棋子
		//	continue;
		//}
		if (FindItemAtPoint(ptNew ,item2) && item.m_color == item2.m_color)
		{
			//终点是同色的棋子
			continue;
		}
		moveArea.append(ptNew);
	}
}

void MainWindow::GetMoveArea_SHUAI( Item item,QVector<QPoint>& moveArea )
{
	QVector<QPoint> directions;
	directions.push_back(QPoint(-1,0));
	directions.push_back(QPoint(1,0));
	directions.push_back(QPoint(0,1));
	directions.push_back(QPoint(0,-1));

	for (int i = 0; i<directions.size(); i++)
	{
		QPoint ptNew = item.m_pt + directions[i];									//移动后的点
		if (item.m_pt.y() <= 4)														//上方的
		{
			if (ptNew.x() >= 3 && ptNew.x() <= 5 && ptNew.y()>=0 && ptNew.y() <= 2)		//移动后在上棋盘的(3,0)~(5,2)田字内
			{

			}
			else
			{
				continue;
			}
		}
		else
		{
			if (ptNew.x() >= 3 && ptNew.x() <= 5 && ptNew.y()>=7 && ptNew.y() <= 9)		//移动后需仍在下半棋盘
			{

			}
			else
			{
				continue;
			}
		}
		Item item2;
		if (FindItemAtPoint(ptNew ,item2) && item.m_color == item2.m_color)
		{
			//终点是同色的棋子
			continue;
		}
		moveArea.append(ptNew);
	}
}

void MainWindow::GetMoveArea_PAO( Item item,QVector<QPoint>& moveArea )
{
	//炮的可移动区域分为两部分：第一部分同“车”，第二部分为直线上隔一个棋子后的异色棋子
	//QVector<QPoint> juMoveArea;
	//GetMoveArea_JU(item,moveArea);

	//左边
	bool bFind = false;
	int x = item.m_pt.x() - 1;
	while(x>=0)
	{
		QPoint ptNew = QPoint(x,item.m_pt.y());
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (bFind)
			{
				//如果之前找到过一个点，那么这是第2个点
				if(item.m_color != item2.m_color)
				{
					//如果这第2个点是异色的，那么是可以吃的
					moveArea.append(ptNew);
				}
				break;
			}
			bFind = true;
			x--;
			continue;
		}
		if (!bFind)
		{
			moveArea.append(ptNew);
		}
		x--;
		
	}
	//右边
	bFind = false;
	x = item.m_pt.x() + 1;
	while(x<=8)
	{
		QPoint ptNew = QPoint(x,item.m_pt.y());
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (bFind)
			{
				//如果之前找到过一个点，那么这是第2个点
				if(item.m_color != item2.m_color)
				{
					//如果这第2个点是异色的，那么是可以吃的
					moveArea.append(ptNew);
				}
				break;
			}
			bFind = true;
			x++;
			continue;
		}
		if (!bFind)
		{
			moveArea.append(ptNew);
		}
		x++;
		
	}

	//上边
	bFind = false;
	int y = item.m_pt.y() - 1;
	while(y>=0)
	{
		QPoint ptNew = QPoint(item.m_pt.x(),y);
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (bFind)
			{
				//如果之前找到过一个点，那么这是第2个点
				if(item.m_color != item2.m_color)
				{
					//如果这第2个点是异色的，那么是可以吃的
					moveArea.append(ptNew);
				}
				break;
			}
			bFind = true;
			y--;
			continue;
		}
		if (!bFind)
		{
			moveArea.append(ptNew);
		}
		y--;
		
	}
	//下方
	bFind = false;
	y = item.m_pt.y() + 1;
	while(y<=9)
	{
		QPoint ptNew = QPoint(item.m_pt.x(),y);
		Item item2;
		if (FindItemAtPoint(ptNew ,item2))
		{
			if (bFind)
			{
				//如果之前找到过一个点，那么这是第2个点
				if(item.m_color != item2.m_color)
				{
					//如果这第2个点是异色的，那么是可以吃的
					moveArea.append(ptNew);
				}
				break;
			}
			bFind = true;
			y++;
			continue;
		}
		if (!bFind)
		{
			moveArea.append(ptNew);
		}
		y++;
		
	}
}

void MainWindow::GetMoveArea_BING( Item item,QVector<QPoint>& moveArea )
{
	QVector<QPoint> directions;
	
	if (item.m_color == COLOR_RED)
	{
		directions.push_back(QPoint(0,-1));	//红色上移
		if (item.m_pt.y() < 5 )			//红色在下方，y<5就是已经过河，可以横向移动
		{
			directions.push_back(QPoint(-1,0));
			directions.push_back(QPoint(1,0));
		}
	}
	else
	{
		directions.push_back(QPoint(0,1));	//黑色下移
		if (item.m_pt.y() >= 5 )			//黑色在上方，y>=5就是已经过河，可以横向移动
		{
			directions.push_back(QPoint(-1,0));
			directions.push_back(QPoint(1,0));
		}
	}

	for (int i = 0; i<directions.size(); i++)
	{
		QPoint ptNew = item.m_pt + directions[i];									//移动后的点
		if (ptNew.x() >= 0 && ptNew.x() <= 8 && ptNew.y()>=0 && ptNew.y() <= 9)		//移动后需仍在棋盘
		{

		}
		else
		{
			continue;
		}
		Item item2;
		if (FindItemAtPoint(ptNew ,item2) && item.m_color == item2.m_color)
		{
			//终点是同色的棋子
			continue;
		}
		moveArea.append(ptNew);
	}

}

//-----------------------------------------------------------------------//

void MainWindow::timerEvent( QTimerEvent * )
{
    for (int i = 0; i<m_items.size(); i++)
    {
        if (m_items[i].m_pt == m_SelectedItem.m_pt)
        {
            m_items[i].m_bShow = !m_items[i].m_bShow;
        }
    }
}

void MainWindow::SetItemShow( Item item,bool bShow )
{
	for (int i = 0; i<m_items.size(); i++)
	{
		if (m_items[i].m_pt == item.m_pt)
		{
			m_items[i].m_bShow = bShow;
			break;
		}
	}
}

//-----------------------------------------------------------------------//
void MainWindow::NewGame()
{
    InitItems();
    m_SelectedItem.m_pt = QPoint(-1,-1);
    m_bIsRedTurn = true;
    calc_bIsRedTurn = true;
    m_bExistSelectedItem = false;
}

