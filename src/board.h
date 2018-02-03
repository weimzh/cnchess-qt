/***************************************************************************
 *   Copyright (C) 2005 by frog                                            *
 *   gorfllew@163.com                                                       *
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

/////////////////////////////////////////////////////////////////////////
// board.h: interface for the CBoard class.
//////////////////////////////////////////////////////////////////////

#ifndef BOARD_H
#define BOARD_H

#include "qstringlist.h"
#include "qstring.h"
#include "defs.h"



//////////////////////////////////////////////////
class CBoard 
{
public:
	CBoard();
	virtual ~CBoard();


public:
// For opening book－用于开局库
	QStringList	m_openingBook;
	QString		m_symmetricLine[4];
	void		InitOpeningBook();
	bool 		Readbook();
	short		SymmetricConvert(short m, short k);
	MOVE		GetOpeningBook();
	bool		FollowOpeningBook();
	void		UpdateNewMoveForFollowOpeningBook(short from, short dest);

// For interface
	void		ChangeSide();
	void		PreCalculate();
	void		ComputerThink(void);
	void		InitData();
	short		UpdateNewMove(short from, short dest);
	bool		IsThereLegalMove();
	//void		Serialize(CArchive& ar);
	bool		CheckLegalMove(short from, short dest);
	bool		UndoMove(short& from, short& dest);
	bool		RedoMove(short& from, short& dest);
	bool		CanUndoMove();

/* Data for interface 
	We need duplicate data such as m_side, m_piece and so on
	because while computer computes its move, the data will be
	changed, but interface still needs original data for
	display
*/
	short		m_side_interface, m_xside_interface;
	short		m_hdp_interface;	// 下棋的历史深度，即已下棋的总步数--hdp=history depth
	
	unsigned char		m_piece_interface[BOARD_SIZE], m_color_interface[BOARD_SIZE];
	
	bool		firstflag;      // 先手标志，true表示红先黑后，false表示黑先红后。
	short		m_gameState;	// 游戏的运行状态，分为COMPUTERTHINKING、HUMANTHINKING、GAMEOVER三种
	
	MOVE		m_newmove;      // 用于记录电脑的下一步棋

	int		max_ply;	// 搜索器的搜索深度；
	
	short		m_pieceSelecting;	//表示当前被选择棋子的位置
	short		m_hdp;    // hdp=history depth=历史深度＝下棋的深度，即已下棋的总步数
	short		m_redo_depth;	// 悔棋后恢复的深度
	
protected:
// For calculation of move (Computer move)
	// data
	short		m_side, m_xside;
	short		m_ply;    // 用于记录着数生成器已走过了多少步
	unsigned char	m_piece[BOARD_SIZE], m_color[BOARD_SIZE];

	GEN_REC		m_gen_dat[MOVE_STACK];    // 着数生成器的着数记录堆栈
	short	m_gen_begin[HIST_STACK], m_gen_end[HIST_STACK];
	HIST_REC	m_hist_dat[HIST_STACK];	// 记录双方所下每一步棋的历史棋步记录堆栈

	short		m_history[BOARD_SIZE][BOARD_SIZE];
	char		m_materialnumber[2][7];
	MOVE		m_pv[HIST_STACK];    // PV=Principal Variation Search=主要变例
	bool		m_bFollowPV;

	// Gen functions-着数生成器函数
	void		InitGen();
	bool		Attack (short pos, short side);
	bool		IsInCheck (short side);
	bool		CheckMoveLoop(short side);
	bool		IsSafeMove (short from, short dest);
	void		Gen_push(short from, short dest);
	bool		Gen();
	bool		GenCapture();

	// Make move functions-
	bool		MakeMove(MOVE m);
	void		UnMakeMove(void);

	// Eval function-评估器函数
	short		Bonous(void);
	short		Eval(void);

	// AlphaBeta function-搜索器函数
	short		AlphaBeta(short alpha, short beta, short depth);
	void		SetMaterial();
	void		Check_pv (void);
	void		Quicksort(short q, short r);
	void		Sort(void);
	short		Quiescence(short alpha, short beta);
	
private:
	

	
};

extern CBoard m_Board; 

#endif // ifndef BOARD_H
