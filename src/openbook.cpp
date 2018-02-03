/***************************************************************************
Purpose: 开局库处理器
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
/***** OPENING BOOK *****/
/*
	When you have a move's line, it may be not in opening book but
	one of its symmetry may be in. So, you have to convert move's line into all
	of symmetrical forms. There are 4 symmetrical forms as the following:
		(1) normal move
		(2) vertical axial symmetry
		(3) horizontal axial symmetry
		(4) central symmetry
	After finding out a symmetry in opening book, you can get a new move
	and convert it into normal move.
*/

#include "qmessagebox.h"

#include "board.h"
#include <qstring.h>	// for QStringList
#include "qfile.h"

static bool bStateBook = false;

/*---------------------------------------------------------
Function:Readbook()
Purpose: 搜索开局库，将各种开局线路写入一个字符串链表CBoard::m_openingBook。
Arguments:
Returns: 如果开局库可用，返回真；否则返回假。
---------------------------------------------------------*/
bool CBoard::Readbook()
{
	static bool bReadBook=false;

	if (bReadBook) return bStateBook;

	bReadBook = true; // one time only
	m_openingBook.clear();
#ifdef __APPLE__
	QFile myfile("cnchess.app/Contents/Resources/book/book.dat");
#elif !defined(_WIN32)
	QFile myfile("/usr/share/games/cnchess/book/book.dat");
#else
	QFile myfile("book/book.dat");
#endif
	if (!myfile.open(IO_ReadOnly)){
	        printf("Error: Can't open 'book.dat'\n");
		return false;
	}

	QTextStream ar( &myfile);

	QString s, line;

	while ( !ar.atEnd()) {
		line="";
		s=ar.readLine();
		// 把字符串转换成位置 
		if(s[0]!=';') {
		for (int i=0; i<s.length(); i++)
			if (s[i] >= 'A') {
				line+=((const char *)s)[i]-65 + (SIZE_X-((const char *)s)[i+1]+48)*SIZE_X;
				i++;
			}
		}
		if (!line.isEmpty()) { 	// save in single linked list 
			m_openingBook.append(line);
			bStateBook = true;
		}
	}

	myfile.close();
	return bStateBook;
}

/* 转换某个位置到棋盘的对称的位置 */
short CBoard::SymmetricConvert(short m, short k)
{
	switch (k) {
		case 0: return m;  // 原位置
		case 1: return m+SIZE_X-1-2*(m%SIZE_X);  // 垂直轴(Y)轴为对称轴
		case 2: return m + (SIZE_Y-1 - 2*(m/SIZE_X))*SIZE_X;  // 水平轴(X轴)为对称轴
		case 3: return BOARD_SIZE-1-m;  // 对角线（中央）对称
	}
	return m;
}

/*---------------------------------------------------------
Function: GetOpeningBook()
Purpose: 搜索开局库链表的，找到可用的它的一条对称线。如果找到的话，就找到新的运动，并且把它按对称种类转换成正常的运动。
Arguments:
Returns: 返回值是“一着棋”
---------------------------------------------------------*/
MOVE CBoard::GetOpeningBook()
{
	int		k, sk ,i;			/* sk: symmetrical kind - 对称种类*/
	bool	b = false;
	QString line;
	MOVE	move;

	move.from = NOMOVE;
	i=0;

	/* Computer go first */
	if (firstflag == false) {  // 如果计算机是先手的话
		if (m_side_interface==BLUE) sk = 2; else sk = 0;
		k = 0; b = true;
	} else
		if (m_hdp_interface * 2==m_symmetricLine[0].length()) {
			// 如果历史棋步与开局库的某一条开局线路的对称线路的前面步数相同的话
			int p = m_openingBook.size();

			while (i<p) {
				line = *(m_openingBook.at(i));
				for(sk=0; sk<4; sk++) {
					b = true;
					for (k=0; k<m_hdp_interface*2 && b; k++)
						if (m_symmetricLine[sk][k]!=line[k])
							b = false;
					if (b) break;
				}
				if (b) break;
				i++;
			}
		}

	/* Convert new move into normal */
	if (b && line != "") {
		move.from = SymmetricConvert(((const char *)line)[k], sk);
		move.dest = SymmetricConvert(((const char *)line)[k+1], sk);
	}

	return move;
}


//  沿着开局库－OPENINGBOOK.DAT走棋
bool CBoard::FollowOpeningBook()
{
	if (!bStateBook) return false;

	m_newmove = GetOpeningBook();
	return (m_newmove.from!=NOMOVE);
}

void CBoard::UpdateNewMoveForFollowOpeningBook(short from, short dest)
{
	if (bStateBook)
		if (m_hdp_interface*2<=m_symmetricLine[0].length()+2) {
			/* Save 4 symmetrical forms */
			int k = m_hdp_interface*2;
			for (int i=0; i<4; i++) {
				if (k<m_symmetricLine[i].length())
					m_symmetricLine[i].left(k);
				m_symmetricLine[i] += (char)SymmetricConvert(from, i);
				m_symmetricLine[i] += (char)SymmetricConvert(dest, i);
				// m_symmetricLine[i] += SymmetricConvert(from, i);
				// m_symmetricLine[i] += SymmetricConvert(dest, i);
				
			}
		}
}

// 在游戏的开始调用该函数,初始化开局库
void CBoard::InitOpeningBook()
{
	Readbook();

	for (int i=0; i<4; i++) {
		m_symmetricLine[i]="";
	}
}
