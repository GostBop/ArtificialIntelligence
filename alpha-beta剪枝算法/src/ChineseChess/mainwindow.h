#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Item.h"
#include "qmap.h"
#include "step.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void InitItems();			//初始化所有棋子位置
	void NewGame();			//新游戏

	bool FindItemAtPoint(QPoint pt,Item& item);					//给定一个棋盘上的点，查找是否存在棋子
	bool DeleteItemAtPoint(QPoint pt,bool& bDeleteSHUAI);		//bDeleteSHUAI：是否打掉的是“帅”或“将”
	void SetItemShow(Item item,bool bShow);						//设置棋子显隐
	bool MoveItem(Item item,QPoint ptMoveTo);					//走棋逻辑
	void ChangeItemPoint(QPoint ptItem,QPoint pt);				//改变棋子位置属性

	void DrawItem(QPainter& painter,Item item);					//绘制棋子

	//获取棋子能移动的区域
	void GetMoveArea(Item item,QVector<QPoint>& area);
	void GetMoveArea_JU(Item item,QVector<QPoint>& area);
	void GetMoveArea_MA(Item item,QVector<QPoint>& area);
	void GetMoveArea_XIANG(Item item,QVector<QPoint>& area);
	void GetMoveArea_SHI(Item item,QVector<QPoint>& area);
	void GetMoveArea_SHUAI(Item item,QVector<QPoint>& area);
	void GetMoveArea_PAO(Item item,QVector<QPoint>& area);
	void GetMoveArea_BING(Item item,QVector<QPoint>& area);

    //computer move
    void computerMove();
    Step * getBestMove();
    void getAllPossibleMove(QVector<Step *> &steps1);
    void fakeMove(Step *step);
    void unfakeMove(Step *step);
    int getMinScore(int level, int curMinScore);
    int getMaxScore(int level, int curMaxScore);
    int calcScore();

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void timerEvent(QTimerEvent *);

private:
    int MAX_LEVEL = 4;
    int _level;

    Ui::MainWindow *ui;

	int m_nItemWidth;
	int m_nItemHeight;
	QPixmap m_ItemsImage;		//棋子图片

	QVector<Item> m_items;	//所有棋子
	Item m_SelectedItem;
	bool m_bExistSelectedItem;	//是否已存在选中的棋子
	bool m_bIsRedTurn;		//当前该红方下
    bool calc_bIsRedTurn;

//public slots:   //用到槽时需要这个，并且上面加一个Q_OBJECT
    //void computerMove();
};

#endif // MAINWINDOW_H
