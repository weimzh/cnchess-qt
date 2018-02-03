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

#ifndef _DEFS_H_
#define _DEFS_H_


//#define	MAX_PLY			4    //搜索器的搜索深度；已废弃不用，被board.h中的max_ply变量所替换。

#define	SIZE_X			9    //棋盘的横边长
#define	SIZE_Y			10    ////棋盘的纵边长
#define BOARD_SIZE		SIZE_X*SIZE_Y

#define MOVE_STACK		4096    //着数生成器的堆栈的大小； 当想要增大搜索器的搜索深度时，要考虑增大这个数值，以防堆栈不够用
#define HIST_STACK		1000    //历史棋步记录堆栈的大小； 当想要增大搜索器的搜索深度时，要考虑增大这个数值，以防堆栈不够用

#define  BLUE			0
#define  RED			1



#define PAWN			0    //卒
#define BISHOP			1    //士
#define ELEPHAN			2    //相
#define KNIGHT			3    //马
#define CANNON			4    //炮
#define ROOK			5    //车
#define KING			6    //将或帅
#define EMPTY			7    //表示无棋子

struct MOVE { short from, dest; };	// 软件中将棋盘上的90个位置从左上角（向右向下）至右下角编号，编为0～89。
struct GEN_REC {  MOVE m; short prior; };    // 着数生成器生成的一着的记录，prior表示是第几着
struct HIST_REC { MOVE m; int capture;};    // 历史上的一步棋的记录，capture记录被吃的棋子


#define	INFINITY		20000
#define NOMOVE				-1

//游戏的三种状态
#define HUMANTHINKING			0
#define COMPUTERTHINKING		1
#define GAMEOVER			2

#endif		// ifndef  _DEFS_H_
