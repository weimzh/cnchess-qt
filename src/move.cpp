/***************************************************************************
Purpose: 移动器
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


/*---- 走一步棋 ----*/
bool CBoard::MakeMove(MOVE m)
{
	short	p, from, dest, t;

	from = m.from; dest = m.dest; 
	p = m_piece[dest];
	if (p!= EMPTY) m_materialnumber[m_xside][p]--;
	m_hist_dat[m_hdp].m = m; m_hist_dat[m_hdp].capture = p;
	m_piece[dest] = m_piece[from]; m_piece[from] = EMPTY;
	m_color[dest] = m_color[from]; m_color[from] = EMPTY;
	m_hdp++; m_ply++;
	t = m_side; m_side = m_xside; m_xside = t;
	return p == KING;
}


/*---- 撤销上一步棋 ----*/
void CBoard::UnMakeMove()
{
	short from, dest, t;
	unsigned char p;

	m_hdp--; m_ply--; t = m_side; m_side = m_xside; m_xside = t;
	from = m_hist_dat[m_hdp].m.from; dest = m_hist_dat[m_hdp].m.dest;
	p = m_hist_dat[m_hdp].capture;
	m_piece[from] = m_piece[dest]; m_piece[dest] = p;
	m_color[from] = m_color[dest]; m_color[dest] = EMPTY;
	if (p != EMPTY) {
		m_color[dest] = (unsigned char)m_xside;
		m_materialnumber[m_xside][p]++;
	}
}

