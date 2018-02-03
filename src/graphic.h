/***************************************************************************
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
///////////////////////////////////////////////////////////////////////
// Graphic.h: interface for the CGraphic class.
//////////////////////////////////////////////////////////////////////


#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "qpoint.h"
#include "qrect.h"
#include "qimage.h"

#define XMARGIN		16
#define YMARGIN		20

class CGraphic  
{
private:
	QImage		m_imagelist[15];	// 用于存放15个不同类型棋子图像的数组
	short		m_xcell, m_ycell;
	QRect		rectBoard;

public:
	QRect GetPieceRect(short pos);
	short GetPiecePos(QPoint pt);
	bool IsPtInBoard(QPoint pt);
	CGraphic();
	virtual ~CGraphic();

	void InitGraphic();
	void DrawMark(QPainter *painter, int x, int y, int mode);
	void DrawGrid(QPainter *painter);
	void DrawPiece(QPainter *painter, unsigned char piece, unsigned char color, short pos, bool selecting);
	void DrawAllPieces(QPainter *painter, unsigned char* piece, unsigned char* color, short selectingPos);
	void DrawBoard(QPainter *painter, unsigned char* piece, unsigned char* color, short selectingPos);
};


extern CGraphic graphic;

#endif // #ifndef GRAPHIC_H
