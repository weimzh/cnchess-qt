/***************************************************************************
Purpose: 棋盘生成／及动态更新器
 *   Copyright (C) 2005 by frog                                            *
 *   gorfllew@163.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// graphic.cpp: implementation of the CGraphic class.
//////////////////////////////////////////////////////////////////////


#include "qpen.h"
#include "qbrush.h"
#include "qpainter.h"
#include "qmessagebox.h"
#include "qimage.h"

#include "cnchess.h"
#include "graphic.h"
#include "defs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef __APPLE__
#define CHESSES_DIR "cnchess.app/Contents/Resources/chesses/"
#elif !defined (_WIN32)
#define CHESSES_DIR "/usr/share/games/cnchess/chesses/"
#else
#define CHESSES_DIR "chesses/"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphic graphic;

CGraphic::CGraphic()
{
}

CGraphic::~CGraphic()
{
}

void CGraphic::InitGraphic()
{
	static bool init = false;

	if (init) return;	// one time only
	init = true;

	m_xcell = 31+2; m_ycell = 31+1; // 31x31 is size of one piece

	m_imagelist[0].load(CHESSES_DIR "rpawn.bmp",0);
	m_imagelist[1].load(CHESSES_DIR "rbishop.bmp",0);
	m_imagelist[2].load(CHESSES_DIR "rminister.bmp",0);
	m_imagelist[3].load(CHESSES_DIR "rhorse.bmp",0);
	m_imagelist[4].load(CHESSES_DIR "rcannon.bmp",0);
	m_imagelist[5].load(CHESSES_DIR "rrook.bmp",0);
	m_imagelist[6].load(CHESSES_DIR "rmarshal.bmp",0);
	m_imagelist[7].load(CHESSES_DIR "bpawn.bmp",0);
	m_imagelist[8].load(CHESSES_DIR "bbishop.bmp",0);
	m_imagelist[9].load(CHESSES_DIR "bminister.bmp",0);
	m_imagelist[10].load(CHESSES_DIR "bhorse.bmp",0);
	m_imagelist[11].load(CHESSES_DIR "bcannon.bmp",0);
	m_imagelist[12].load(CHESSES_DIR "brook.bmp",0);
	m_imagelist[13].load(CHESSES_DIR "bgeneral.bmp",0);
	m_imagelist[14].load(CHESSES_DIR "selected.bmp",0);

	rectBoard.setLeft(XMARGIN);
	rectBoard.setRight(XMARGIN + m_xcell*9);
	rectBoard.setTop(YMARGIN);
	rectBoard.setBottom(YMARGIN + m_ycell*10);
}

// 画棋盘网格线
void CGraphic::DrawGrid(QPainter *painter)
{
	QBrush pOldBrush ;
	pOldBrush=painter->brush();
	

	QBrush brBg(QColor(0,0,0),Qt::SolidPattern);
	painter->setBrush(brBg);

	painter->moveTo(100,100);
	painter->drawRect(rectBoard.left()-2,rectBoard.top()-2,(rectBoard.right()+2)-(rectBoard.left()-2),(rectBoard.bottom()+2)-(rectBoard.top()-2));
	painter->setBrush(pOldBrush);


	// main grid
	QPen pOldPen;
	pOldPen=painter->pen();
	
	QPen pen(QColor(0,255,0),0,Qt::SolidLine);
	painter->setPen(pen);
	
	int i, x1, x2, y1, y2;
	
	x1 = rectBoard.left()+m_xcell/2;
	y1 = rectBoard.top()+m_ycell/2;
	x2 = x1+m_xcell*8;
	y2 = y1+m_ycell*9;

	for (i=0; i<10; i++) {
		painter->moveTo(x1,y1+i*m_ycell);
		painter->lineTo(x2,y1+i*m_ycell);
	}

	painter->moveTo(x1,y1);
	painter->lineTo(x1,y2);
	painter->moveTo(x2,y1);
	painter->lineTo(x2,y2);

	for (i=1; i<8; i++) {
		painter->moveTo(x1+i*m_xcell,y1);
		painter->lineTo(x1+i*m_xcell,y1+4*m_ycell);
		painter->moveTo(x1+i*m_xcell,y1+5*m_ycell);
		painter->lineTo(x1+i*m_xcell,y2);
	}

	// King palace 1
	painter->moveTo(x1+3*m_xcell,y1);
	painter->lineTo(x1+5*m_xcell,y1+2*m_ycell);
	painter->moveTo(x1+5*m_xcell,y1);
	painter->lineTo(x1+3*m_xcell,y1+2*m_ycell);

	// King palace 2
	painter->moveTo(x1+3*m_xcell,y1+7*m_ycell);
	painter->lineTo(x1+5*m_xcell,y1+9*m_ycell);
	painter->moveTo(x1+5*m_xcell,y1+7*m_ycell);
	painter->lineTo(x1+3*m_xcell,y1+9*m_ycell);

	// Pawn positions
	for (int j=3; j<=6; j+=3)
		for (i=2; i<=6; i+=2)
			DrawMark(painter, x1+i*m_xcell,y1+j*m_ycell, 3);
	DrawMark(painter,x1,y1+3*m_ycell, 2);
	DrawMark(painter,x1,y1+6*m_ycell, 2);
	DrawMark(painter,x1+8*m_xcell,y1+3*m_ycell, 1);
	DrawMark(painter,x1+8*m_xcell,y1+6*m_ycell, 1);

	// Cannon positions
	DrawMark(painter,x1+m_xcell,y1+2*m_ycell, 3);
	DrawMark(painter,x1+7*m_xcell,y1+2*m_ycell, 3);
	DrawMark(painter,x1+m_xcell,y1+7*m_ycell, 3);
	DrawMark(painter,x1+7*m_xcell,y1+7*m_ycell, 3);

	
	painter->setPen( pOldPen );

}

// 画棋盘中棋子位置的标记
void CGraphic::DrawMark(QPainter *painter, int x, int y, int mode)
{
	short sx, sy;
	sx = m_xcell / 5; sy = m_ycell / 5;

	if (mode & 1) {
		painter->moveTo(x-3-sx,y-3);
		painter->lineTo(x-3,y-3);
		painter->moveTo(x-3,y-3-sy);
		painter->lineTo(x-3,y-3);

		painter->moveTo(x-3-sx,y+3);
		painter->lineTo(x-3,y+3);
		painter->moveTo(x-3,y+3+sy);
		painter->lineTo(x-3,y+3);
	}

	if (mode & 2) {
		painter->moveTo(x+3+sx,y-3);
		painter->lineTo(x+3,y-3);
		painter->moveTo(x+3,y-3-sy);
		painter->lineTo(x+3,y-3);

		painter->moveTo(x+3+sx,y+3);
		painter->lineTo(x+3,y+3);
		painter->moveTo(x+3,y+3+sy);
		painter->lineTo(x+3,y+3);
	}
}

// 画一块棋子
void CGraphic::DrawPiece(QPainter *painter, unsigned char piece, unsigned char color, short pos, bool selecting)
{
	QPoint pt;
	pt.setX((pos % 9)*m_xcell + rectBoard.left());
	pt.setY((pos / 9)*m_ycell + rectBoard.top());

	if(piece!=EMPTY){
		int z = piece + ((color==RED)? 0: 7);
		painter->drawImage(pt,m_imagelist[z],0);
	
		if(selecting){
			painter->setRasterOp(Qt::OrROP);
			painter->drawImage(pt,m_imagelist[14],0);
			painter->setRasterOp(Qt::CopyROP);
		}
	}


}

// 画所有的棋子
void CGraphic::DrawAllPieces(QPainter *painter, unsigned char* piece, unsigned char* color, short selectingPos)
{
	for(short i=0; i<90; i++) {
		DrawPiece(painter, piece[i], color[i], i, i==selectingPos);
	}
}


// 画整个棋盘
void CGraphic::DrawBoard(QPainter *painter,unsigned char* piece, unsigned char* color, short selectingPos)
{
	DrawGrid(painter);
	DrawAllPieces(painter, piece, color, selectingPos);
}

// 判断点pt是否在棋盘中
bool CGraphic::IsPtInBoard(QPoint pt)
{
	return rectBoard.contains(pt);
}

// 得到某一块的位置（位置范围为0－89）
short CGraphic::GetPiecePos(QPoint pt)
{
	if (!rectBoard.contains(pt)) return -1;
	short x = (pt.x()-rectBoard.left()) / m_xcell;
	short y = (pt.y()-rectBoard.top())  / m_ycell;

	return x + y * 9;
}

// 得到某一块所在的矩型区域
QRect CGraphic::GetPieceRect(short pos)
{
	short x = XMARGIN + (pos % 9)*m_xcell;
	short y = YMARGIN + (pos / 9)*m_ycell;
	QRect rect(x, y, x+m_xcell, y+m_ycell);
	return rect;	
}
