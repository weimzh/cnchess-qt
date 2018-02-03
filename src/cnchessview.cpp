#include "qmessagebox.h"
#include "qcolor.h"

#include "cnchessview.h"

#include "board.h"
#include "graphic.h"


CCnchessView *m_cnchessview;

CCnchessView::CCnchessView(QWidget *parent=0,const char* name=0,WFlags f=0)
        : QScrollView ( parent,name,f) 
{
	setBackgroundColor(QColor(255,0,255));
	
	// TODO: add construction code here
	graphic.InitGraphic();	
	m_Board.m_pieceSelecting = NOMOVE;
	setFixedSize(333, 370);
}

CCnchessView::~CCnchessView()
{
}

void CCnchessView::drawContents ( QPainter *p,int cx,int cy,int cw,int ch)
{
	// TODO: 在此处加入本地的绘制代码
	
	
	graphic.DrawBoard(p,m_Board.m_piece_interface,m_Board.m_color_interface, m_Board.m_pieceSelecting);
	
	
}

// 处理鼠标按下事键，通常鼠标按下事件表示人在走棋
void CCnchessView::contentsMousePressEvent ( QMouseEvent * e )
{

	//如果鼠标按下的位置不在棋盘上或不是人在思考，则此次按下无效，直接返回。
	if (!graphic.IsPtInBoard(e->pos()) || m_Board.m_gameState!=HUMANTHINKING)
		return;
		
	
	short k = graphic.GetPiecePos(e->pos());

	short z = m_Board.m_pieceSelecting;

	unsigned char piece = m_Board.m_piece_interface[k];
	unsigned char color = m_Board.m_color_interface[k];

	if (color==m_Board.m_side_interface) {	//如果鼠标按下位置的块的颜色等于m_side（人的颜色），则表示人在选择一个棋子。
		if (z != NOMOVE) {
			m_Board.m_pieceSelecting = NOMOVE;
			RequireDrawCell(z);
		}
		m_Board.m_pieceSelecting = k;
		RequireDrawCell(k);

	}
	else{	//如果鼠标按下的位置块的颜色不等于m_side(人的颜色)，则表示人将z处的棋子移往目标K处
		if (z != NOMOVE){
			if (m_Board.CheckLegalMove(z, k)){
				bool humanwon = false;
				
				m_Board.m_redo_depth=0;  // 一旦人走棋后，重做深度就应该清零。
				
				if (m_Board.UpdateNewMove(z, k)) 
					humanwon = true;
				
				
				
//?				pDoc->SetModifiedFlag( );    // Mark document as modified 
				m_Board.m_pieceSelecting = NOMOVE;
				RequireDrawCell(z);
				RequireDrawCell(k);

				m_Board.ChangeSide();

				// won if rival has no move
				if (!m_Board.IsThereLegalMove())
					humanwon = true;

				if (humanwon){
					m_Board.m_gameState = GAMEOVER;
					QMessageBox *m_msg = new QMessageBox(0,0);
					m_msg->setCaption(tr("You Win"));
					m_msg->setText(tr("Congratulations, you win!"));
					m_msg->show();
					return;
				}

				m_Board.m_gameState = COMPUTERTHINKING;

				OnLetComputerThink();
			}
		}
	}

}

// 重画某一块棋子
void CCnchessView::RequireDrawCell(short pos)
{
	QRect rect = graphic.GetPieceRect(pos);
	
	repaintContents(rect.left(),rect.top(),rect.right()-rect.left(),rect.bottom()-rect.top(),false);

}

// 让电脑思考
void CCnchessView::OnLetComputerThink() 
{

	if (m_Board.m_gameState==GAMEOVER)
		return;

	// Calculate a move
	m_Board.ComputerThink();
	short z=m_Board.m_newmove.from;
	short k=m_Board.m_newmove.dest;

	//显示电脑将要走的那个棋子为被选中状态
	m_Board.m_pieceSelecting = z;
	RequireDrawCell(z);
	sleep(1);
	
	bool compwon = false;
	if (m_Board.UpdateNewMove(z, k))	// "kill" human king
		compwon = true;

//?	pDoc->SetModifiedFlag( );    // Mark document as modified 
	
	RequireDrawCell(z);
	RequireDrawCell(k);

	// Won if no move for rival
	m_Board.ChangeSide();
	if (!m_Board.IsThereLegalMove())
		compwon = true;

	if (compwon) {
		m_Board.m_gameState = GAMEOVER;
		QMessageBox *m_msg = new QMessageBox(0,0);
		m_msg->setCaption(tr("You lose"));
		m_msg->setText(tr("Sorry, you lose"));
		m_msg->show();
		return;
	}

	m_Board.m_gameState = HUMANTHINKING;
}
