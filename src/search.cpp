/***************************************************************************
 Purpose: 搜索器－深度优先搜索
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

#include "board.h"
#include <qnamespace.h> 
#include <qwidget.h> 
#include "qcursor.h"

#include "cnchessview.h"

/*检查基本变例--pv＝principal variation*/
void CBoard::Check_pv (void)
{
	short i;
	for (m_bFollowPV=false, i=m_gen_begin[m_ply]; i<m_gen_end[m_ply]; i++)
		if (m_gen_dat[i].m.from==m_pv[m_ply].from && m_gen_dat[i].m.dest==m_pv[m_ply].dest) {
			m_bFollowPV = true; m_gen_dat[i].prior += 1000; return;
		}
}

/*-----快速排序－－－－对m_gen_dat[]数组进行排序----*/
void CBoard::Quicksort(short q, short r)
{
	short 	i, j, x;
	GEN_REC g;

	i = q; j = r; x = m_gen_dat[(q+r)/2].prior;

	do {
		while (m_gen_dat[i].prior > x) i++;
		while (m_gen_dat[j].prior < x) j--;
		if (i <= j) {
			g = m_gen_dat[i]; m_gen_dat[i] = m_gen_dat[j];
			m_gen_dat[j] = g; i++; j--;
		}
	} while (i<=j);

	if (q < j) Quicksort(q, j);
	if (i < r) Quicksort(i, r);
}


void CBoard::Sort(void)
{
	Quicksort(m_gen_begin[m_ply], m_gen_end[m_ply]-1);
}

/*---------------------------------------------------------
Function: Quiescence()
Purpose: “静态搜索”(Quiescent Search)。当Alpha-Beta用尽深度后，通过调用静态搜索来代替调用“Evaluate()”。这个函数也对局面作评价，只是避免了在明显有对策的情况下看错局势。简而言之，静态搜索就是应对可能的动态局面的搜索。 典型的静态搜索只搜索吃子着法。这会引发一个问题，因为在象棋中吃子通常不是强制的,如果局势很平静，不应该强迫吃子
	这个函数调用静态评价，如果评价好得足以截断而不需要试图吃子时，就马上截断(返回Beta/value)。如果评价不足以产生截断，但是比Alpha好，那么就更新Alpha来反映静态评价。 
	然后尝试吃子着法，如果其中任何一个产生截断，搜索就中止。可能它们没有一个是好的，这也没问题。 
Arguments:
Returns: 这个函数有几个可能的结果：可能评价函数会返回足够高的数值，使得函数通过Beta截断马上返回；也可能某个吃子产生Beta截断；也可能静态评价比较坏，而任何吃子着法也不会更好；或者可能任何吃子都不好，但是静态评价只比Alpha高一点点。
---------------------------------------------------------*/
short CBoard::Quiescence(short alpha, short beta)
{
	short i, value, best;

	value = Eval();    // 调用静态评价函数
	if (value >= beta) return value;    // 如果静态评价好得足以截断而不需要试图吃子时，就马上截断
	if (value > alpha) alpha = value;  // 如果静态评价不足以产生截断，但是比Alpha好，那么就更新Alpha来反映静态评价。 

	GenCapture();
	if (m_gen_begin[m_ply] >= m_gen_end[m_ply]) return value;
	if (m_bFollowPV) 
		Check_pv(); 
	Sort();
	best = -INFINITY;

	for (i = m_gen_begin[m_ply];i<m_gen_end[m_ply] && best<beta; i++) {
		if (best > alpha) alpha = best;

		if (MakeMove(m_gen_dat[i].m)) 
			value = 1000-m_ply;
		else 
			value = -Quiescence(-beta, -alpha);
		UnMakeMove();

		if (value > best) {
			best = value;
			m_pv[m_ply] = m_gen_dat[i].m;
		}
	}

	return best;
}


/*---------------------------------------------------------
Function: AlphaBeta()
Purpose: 通过alpha-beta搜索算法来搜索游戏树，对于着法生成器生成的每一着进行搜索和评估，并最终确定采用哪一着棋
Arguments:
Returns: 返回值是“着数生成器”所生成的最好的那一着的评估值
---------------------------------------------------------*/
short CBoard::AlphaBeta(short alpha, short beta, short depth)
{
	short i, value, best;

	if (!depth) return Quiescence(alpha, beta);

	Gen();
	best = -INFINITY;

	// 对着数生成器生成的每一着进行搜索和评估
	for (i=m_gen_begin[m_ply]; i<m_gen_end[m_ply] && best<beta; i++) {
		if (best > alpha) alpha = best;

		if (MakeMove(m_gen_dat[i].m))
			value = 1000-m_ply;
		else
			value = -AlphaBeta(-beta, -alpha, depth-1);
		UnMakeMove();

		// 选择
		if (value > best) {
			best = value;
			if (!m_ply) m_newmove = m_gen_dat[i].m;
		}
	}

	return best;
}

// 电脑思考
void CBoard::ComputerThink(void)
{
	PreCalculate();

	if(max_ply>2)            // 只有级别在中级以上（即电脑的搜索深度大于2时），才可以使用开局库。       
		if (FollowOpeningBook()) return;	//  首先按“开局库-BOOK.DAT”走棋，如果成功，返回；如果不成功按下面的算法走棋。
	
	
	QCursor m_cursor1(Qt::WaitCursor);
	m_cnchessview->setCursor(m_cursor1);

	InitGen();
	SetMaterial();

	m_pv[0].from = NOMOVE;
	memset(&m_history, 0, sizeof(m_history));

	m_newmove.from = NOMOVE;

	for (short depth=1; depth<=max_ply; depth++) {
		m_bFollowPV = true;
		short best = AlphaBeta(-INFINITY, INFINITY, depth);
		if (m_newmove.from==NOMOVE) break;
	}
	
	QCursor m_cursor2(Qt::ArrowCursor);
	m_cnchessview->setCursor(m_cursor2);
}
