/***************************************************************************
Purpose: 入口程序
 *   Copyright (C) 2005 by frog                                            *
 *   root@sdu-zl9999                                                       *
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
//////////////////////////////////////////////////////////////////////
//
// board.cpp: implementation of the CBoard class.
//
//////////////////////////////////////////////////////////////////////


#include "board.h"
#include "cnchessview.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBoard m_Board;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoard::CBoard()
{
	//游戏的状态初始设置为“红先蓝后”(同步游戏的状态设为“人思考”）。
	firstflag = true;
	m_gameState = HUMANTHINKING;

	// 置游戏的初始对抗级别为初级，电脑搜索深度（看棋步数）为2步棋
	max_ply=3; 

	//  最初，m_side代表红方，m_xside代表蓝方
	m_side_interface = RED; m_xside_interface = BLUE;
	
	InitOpeningBook();
	
	InitData();
}

CBoard::~CBoard()
{
}


void CBoard::SetMaterial()
{
	int i;

	for (i=0; i<7; i++) {
		m_materialnumber[0][i] = 0;
		m_materialnumber[1][i] = 0;
	}

	for (i=0; i<90; i++)
		if (m_piece[i] != EMPTY) 
			m_materialnumber[m_color[i]][m_piece[i]]++;
}

/*初始化系统数据*/
void CBoard::InitData()
{
	//  初始化颜色，0为蓝方棋子的位置，1为红方棋子的位置，7为无。
	static unsigned char color[BOARD_SIZE]= {
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 0, 7, 7, 7, 7, 7, 0, 7,
		0, 7, 0, 7, 0, 7, 0, 7, 0,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		1, 7, 1, 7, 1, 7, 1, 7, 1,
		7, 1, 7, 7, 7, 7, 7, 1, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		1, 1, 1, 1, 1, 1, 1, 1, 1},

	//  初始化位置，0兵或卒，1士，2相，3马，4炮，5车，6将或帅，7无
	piece[BOARD_SIZE]= {
		5, 3, 2, 1, 6, 1, 2, 3, 5,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 4, 7, 7, 7, 7, 7, 4, 7,
		0, 7, 0, 7, 0, 7, 0, 7, 0,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		0, 7, 0, 7, 0, 7, 0, 7, 0,
		7, 4, 7, 7, 7, 7, 7, 4, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7,
		5, 3, 2, 1, 6, 1, 2, 3, 5};

	for (int i=0; i<BOARD_SIZE; i++){
		m_piece_interface[i] = piece[i];
		m_color_interface[i] = color[i];
	}

	//  m_hdp表示下棋的深度，即双方所走的步数
	m_hdp_interface = 0;
	m_redo_depth=0;
	
	m_pieceSelecting=NOMOVE; // 本语句表示在开局之初不需要显示哪个棋子被选中。	 
	
	
}

void CBoard::PreCalculate()
{
	for (int i=0; i<90; i++) {
		m_piece[i] = m_piece_interface[i];
		m_color[i] = m_color_interface[i];
	}

	m_hdp = m_hdp_interface;
	m_side = m_side_interface;
	m_xside = m_xside_interface;
}

void CBoard::ChangeSide()
{
	short t = m_side_interface;
	m_side_interface = m_xside_interface; m_xside_interface = t;
}

/*更新棋子从源移动到目标后的系统数据，并判断此次移动是否能将死对方的将或帅，如果将死则返回真*/
short CBoard::UpdateNewMove(short from, short dest)
{
	if (from==NOMOVE || dest==NOMOVE) return true;

	m_hist_dat[m_hdp_interface].m.from = from;
	m_hist_dat[m_hdp_interface].m.dest = dest;

	unsigned char p = m_piece_interface[dest];
	m_hist_dat[m_hdp_interface].capture = p;

	m_piece_interface[dest] = m_piece_interface[from];
	m_piece_interface[from] = EMPTY;
	m_color_interface[dest] = m_color_interface[from];
	m_color_interface[from] = EMPTY;

	m_hdp_interface++;

	UpdateNewMoveForFollowOpeningBook(from, dest);
	return p == KING;
}

// 悔棋
bool CBoard::UndoMove(short& from, short& dest)
{
	if(m_gameState == GAMEOVER)	return false;
	
	if (!m_hdp_interface) return false;
	m_hdp_interface--;
	m_redo_depth++;

	from = m_hist_dat[m_hdp_interface].m.from;
	dest = m_hist_dat[m_hdp_interface].m.dest;
	unsigned char p = m_hist_dat[m_hdp_interface].capture;

	m_piece_interface[from] = m_piece_interface[dest];
	m_piece_interface[dest] = p;
	m_color_interface[from] = m_color_interface[dest];
	if (p==EMPTY)
		m_color_interface[dest] = EMPTY;
	else
		m_color_interface[dest] = (unsigned char)m_side_interface;

	ChangeSide();
		
	if(m_gameState == HUMANTHINKING)
		m_gameState = COMPUTERTHINKING;
	else
		m_gameState = HUMANTHINKING;
		
	return true;
}

// 悔棋后的重做
bool CBoard::RedoMove(short& from, short& dest)
{
	if(m_gameState == GAMEOVER)	return false;
	
	if (!m_redo_depth) return false;
	m_redo_depth--;
	

	from = m_hist_dat[m_hdp_interface].m.from;
	dest = m_hist_dat[m_hdp_interface].m.dest;
	unsigned char p = m_hist_dat[m_hdp_interface].capture;

	m_piece_interface[dest] = m_piece_interface[from];
	m_piece_interface[from] = EMPTY;
	m_color_interface[dest] = m_color_interface[from];
	m_color_interface[from] = EMPTY;

	m_hdp_interface++;
	
	ChangeSide();
	
	if(m_gameState == HUMANTHINKING)
		m_gameState = COMPUTERTHINKING;
	else
		m_gameState = HUMANTHINKING;
	
	return true;
}

bool CBoard::CanUndoMove()
{
	return m_hdp_interface;
}

/*检查棋局是否还有合法的运动，如果有返回真；如果没有返回假，游戏到了结束点。*/
bool CBoard::IsThereLegalMove()
{
	PreCalculate();
	return Gen();
}

/*检查棋子从源到目标的运动是否合法*/
bool CBoard::CheckLegalMove(short from, short dest)
{
	PreCalculate();
	InitGen();
	Gen();
	for (int i=m_gen_begin[0]; i<m_gen_end[0]; i++)
		if (m_gen_dat[i].m.from==from && m_gen_dat[i].m.dest==dest)
			return true;

	return false;
}
