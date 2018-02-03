/***************************************************************************
Purpose: 着数生成器－根据棋盘的当前局面生成下一步所有可能的着数
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


/**** DATA ****/
static short offset[7][8] =
		{{-1, 1,13, 0, 0, 0, 0, 0},		/* PAWN {for BLUE side} */
		{-12,-14,12,14,0,0,0,0},		/* BISHOP */
		{-28,-24,24,28, 0, 0, 0, 0 },	/* ELEPHAN */
		{-11,-15,-25,-27,11,15,25,27},	/* KNIGHT */
		{-1, 1,-13,13, 0, 0, 0, 0},		/* CANNON */
		{-1, 1,-13,13, 0, 0, 0, 0},		/* ROOK */
		{-1, 1,-13,13, 0, 0, 0, 0}};	/* KING */

static short mailbox182[182] =
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,
		-1,-1, 9,10,11,12,13,14,15,16,17,-1,-1,
		-1,-1,18,19,20,21,22,23,24,25,26,-1,-1,
		-1,-1,27,28,29,30,31,32,33,34,35,-1,-1,
		-1,-1,36,37,38,39,40,41,42,43,44,-1,-1,
		-1,-1,45,46,47,48,49,50,51,52,53,-1,-1,
		-1,-1,54,55,56,57,58,59,60,61,62,-1,-1,
		-1,-1,63,64,65,66,67,68,69,70,71,-1,-1,
		-1,-1,72,73,74,75,76,77,78,79,80,-1,-1,
		-1,-1,81,82,83,84,85,86,87,88,89,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

static short mailbox90[90] =
		{28, 29, 30, 31, 32, 33, 34, 35, 36,
		 41, 42, 43, 44, 45, 46, 47, 48, 49,
		 54, 55, 56, 57, 58, 59, 60, 61, 62,
		 67, 68, 69, 70, 71, 72, 73, 74, 75,
		 80, 81, 82, 83, 84, 85, 86, 87, 88,
		 93, 94, 95, 96, 97, 98, 99,100,101,
		106, 107,108,109,110,111,112,113,114,
		119, 120,121,122,123,124,125,126,127,
		132, 133,134,135,136,137,138,139,140,
		145, 146,147,148,149,150,151,152,153};

static short legalposition[90] =
		{1, 1, 5, 3, 3, 3, 5, 1, 1,
		1, 1, 1, 3, 3, 3, 1, 1, 1,
		5, 1, 1, 3, 7, 3, 1, 1, 5,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		9, 1,13, 1, 9, 1,13, 1, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9,
		9, 9, 9, 9, 9, 9, 9, 9, 9};

static short maskpiece[7] = {8, 2, 4, 1, 1, 1, 2},
	knightcheck[8] = {1,-1,-9,-9,-1,1,9,9},
	elephancheck[8] = {-10,-8,8,10,0,0,0,0},
	kingpalace[2][9] = {{3,4,5,12,13,14,21,22,23},{66,67,68,75,76,77,84,85,86}}; // 红蓝双方将帅的合法位置

static short knightcheck2[8] = {-8,-10,-8,-10,8,10,8,10};

/*============= FUNCTIONS===========*/

void CBoard::InitGen()
{
	m_gen_begin[0] = 0; m_ply = 0;
}

/*---------------------------------------------------------
Function: Attack()
Purpose: 
Arguments:
Returns: 
---------------------------------------------------------*/
bool CBoard::Attack (short pos, short side)
{
	short j, k, x, y, fcannon;
	short sd, xsd;

	sd = side; xsd = 1 - sd;

	for (j=0; j<4; j++) {			// ROOK, CANNON, PAWN, KING
		x = mailbox90[pos]; fcannon = 0;
		for (k=0; k<9; k++) {
			x = x + offset[ROOK][j]; y = mailbox182[x]; if (y==-1) break;
			if (!fcannon) {
				if (m_color[y]==xsd)
					switch (m_piece[y]) {
					case ROOK: return true;
					case KING: if (m_piece[pos]==KING) return true;
					case PAWN: if (k==0 &&
								  ((sd==BLUE && j!=2) ||
								   (sd==RED && j!=3))) return true;
					}
				if (m_color[y]!=EMPTY) fcannon = 1;
			} 
			else 	// CANNON case
				if (m_color[y] != EMPTY) {
					if (m_color[y]==xsd && m_piece[y]==CANNON) return true;
					break;
				}
		} // for k
	} // for j

	for (j = 0; j < 8; j++)	{				// Knight Check
		y = mailbox182[mailbox90[pos]+offset[KNIGHT][j]];
		if (y==-1) continue;
		if (m_color[y]==xsd && m_piece[y]==KNIGHT && m_color[pos+knightcheck2[j]]==EMPTY)
			return true;
	}
	return false;
}


bool CBoard::IsInCheck (short side)
{
	short i, pos;
	i = 0;
	
	do{ 
		pos = kingpalace[side][i];
		i++;
	} while (m_piece[pos]!=KING);
	
	return Attack(pos, side);
}


/* not allow a move that makes a same position in last 30 positions */
#define MAXREP 30
bool CBoard::CheckMoveLoop(short side)
{
	short hdmap[MAXREP+1];
	short c, f, i, j, k, m, p, b;
	MOVE cm;

	/* if the side has not any attack pieces, it can make any move */
	if ((m_hdp<4) ||
	   (m_materialnumber[side][ROOK]+m_materialnumber[side][CANNON]+
		m_materialnumber[side][KNIGHT]+m_materialnumber[side][PAWN]==0))
		return false;

	for (i=0; i<MAXREP+1; i++) hdmap[i] = 0;
	if (m_hdp>MAXREP) m = MAXREP; else m = m_hdp;
	c = 0; i = 0; k = 0;

	while (i<m) {
		// capture a piece - can not loop
		if (m_hist_dat[m_hdp-1-i].capture!=EMPTY) return false;

		if (hdmap[MAXREP-i]==0) {
			c++; hdmap[MAXREP-i] = c;	// track moves of a piece
			p = m_hist_dat[m_hdp-1-i].m.dest; f = m_hist_dat[m_hdp-1-i].m.from;

			j = i+1;
			while (j<m) {
				if (f==m_hist_dat[m_hdp-1-j].m.dest) {
					f = m_hist_dat[m_hdp-1-j].m.from;
					hdmap[MAXREP-j] = c;
					if (p==f) {
						if (k<j) k = j;
						break;
					}
				} /* if (f */
				j++;
			} /* while (j<m) */

			if (j>=m) return false;
		} /* if (hdmap */
		i++;

		// it is loop, but if the loop piece is 
		// attached and new place will 
		// help it to avoid attaching, let it moves
		if (i>2 && i==k) {
			b = Attack (m_hist_dat[m_hdp-1].m.dest, side);
			if (!b) {
				cm = m_hist_dat[m_hdp-1].m;
				UnMakeMove();
				b = Attack (cm.from, side);
				MakeMove (cm);
				if (b) return false;
			}
			return true;
		}
	} /* while (i<m) */
	return false;
}


bool CBoard::IsSafeMove (short from, short dest)
{
	MOVE ms;
	bool k;

	if (from==88 && dest==69)
		k = true;
	if (m_piece[dest]==KING) return true;
	ms.from = from; ms.dest = dest;
	MakeMove (ms);
	k = IsInCheck (m_xside);
	if (!k) k = CheckMoveLoop(m_xside);
	UnMakeMove();
	return !k;
}

/*---------------------------------------------------------
Function: Gen_push()
Purpose: 将着数生成器生成的一着棋推入着数生成器的记录堆栈
Arguments:
Returns: 无
---------------------------------------------------------*/
void CBoard::Gen_push(short from, short dest)
{
	if (IsSafeMove(from, dest)) {
		m_gen_dat[m_gen_end[m_ply]].m.from = from;
		m_gen_dat[m_gen_end[m_ply]].m.dest = dest;
		if (m_piece[dest]!=EMPTY)
			 m_gen_dat[m_gen_end[m_ply]].prior = (m_piece[dest]+1)*100 - m_piece[from];
		else m_gen_dat[m_gen_end[m_ply]].prior = m_history[from][dest];
		m_gen_end[m_ply]++;
	}
}

/*---------------------------------------------------------
Function: Gen()
Purpose: 生成当前局面下所有可能的着数
Arguments:
Returns: 
---------------------------------------------------------*/
bool CBoard::Gen()
{
	short i, j, k, n, p, x, y, t, fcannon;

	m_gen_end[m_ply] = m_gen_begin[m_ply];

	for(i=0; i < BOARD_SIZE; i++)  //检查棋盘的每一个位置
		if (m_color[i]==m_side) {
			if (i==84)
				i = i;
			p = m_piece[i];
			for(j=0; j<8; j++) {
				if (!offset[p][j]) break;
				x = mailbox90[i]; fcannon = 0;
				if (p==ROOK || p==CANNON) n = 9; else n = 1;
				for (k=0; k<n; k++) {
					if (p==PAWN && m_side==RED) x -= offset[p][j]; else x += offset[p][j];

					y = mailbox182[x];
					if (m_side==BLUE) t = y; else t = 89-y;
					if (y==-1 || (legalposition[t] & maskpiece[p])==0) break;
					if (!fcannon) {
						if (m_color[y]!=m_side)
							switch (p) {
							case KNIGHT: if (m_color[i+knightcheck[j]]==EMPTY) Gen_push(i, y); break;
							case ELEPHAN:if (m_color[i+elephancheck[j]]==EMPTY) Gen_push(i, y); break;
							case CANNON: if (m_color[y]==EMPTY) Gen_push(i, y); break;
							default: Gen_push(i, y);
							}
						if (m_color[y]!=EMPTY) { if (p==CANNON) fcannon++; else break; }
					}
					else {	/* CANNON switch */
						if (m_color[y] != EMPTY) {
							if (m_color[y]==m_xside) Gen_push(i, y);
							break;
						}
					}
				} /* for k */
			} /* for j */
	}

	m_gen_end[m_ply+1] = m_gen_end[m_ply]; m_gen_begin[m_ply+1] = m_gen_end[m_ply];
	return (m_gen_end[m_ply]>m_gen_begin[m_ply]);
}

/*---------------------------------------------------------
Function: GenCapture()
Purpose: 生成可以吃子的着数
Arguments:
Returns: 
---------------------------------------------------------*/
bool CBoard::GenCapture()
{
	short i, j, k, n, p, x, y, t, fcannon;

	m_gen_end[m_ply] = m_gen_begin[m_ply];

	for (i=0; i<BOARD_SIZE; i++)
		if (m_color[i]==m_side) {
			p = m_piece[i];
			for (j=0; j<8; j++) {
				if (offset[p][j]==0) break;
				x = mailbox90[i]; fcannon = 0;

				if (p==ROOK || p==CANNON) n = 9; else n = 1;
				for (k=0; k<n; k++) {
					if (p==PAWN && m_side==RED) x -= offset[p][j]; else x += offset[p][j];

					y = mailbox182[x];
					if (m_side==BLUE) t = y; else t = 89-y;
					if (y==-1 || (legalposition[t] & maskpiece[p])==0) break;
					if (!fcannon) {
						if (m_color[y] != m_side) {
							switch (p) {
							case KNIGHT: if (m_color[i+knightcheck[j]]==EMPTY && m_color[y]==m_xside) Gen_push(i, y); break;
							case ELEPHAN:if (m_color[i+elephancheck[j]]==EMPTY && m_color[y]==m_xside) Gen_push(i, y); break;
							case CANNON: break;
							default: if (m_color[y]==m_xside) Gen_push(i, y);
							}
						}
						if (m_color[y]!=EMPTY) { if (p==CANNON) fcannon = 1; else break; }
					}
					else {	/* CANNON switch */
						if (m_color[y]!=EMPTY) {
							if (m_color[y]==m_xside) Gen_push(i, y);
							break;
						}
					}
				} /* for k */
			} /* for j */
		}
	m_gen_end[m_ply+1] = m_gen_end[m_ply]; m_gen_begin[m_ply+1] = m_gen_end[m_ply];

	return  (m_gen_begin[m_ply] < m_gen_end[m_ply]);
}
