/***************************************************************************
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

#ifndef    CNCHESS_H
#define    CNCHESS_H

#include <qmainwindow.h>

extern QString applicationdir;

class MyMainWindow: public QMainWindow
{
Q_OBJECT
public:
	MyMainWindow();
	~MyMainWindow();

private slots:
	void fileNew();

	void editUndo();
	void editRedo();

	void settingRed();
	void settingBlue();

	void levelNewer();
	void levelPrimary();
	void levelMiddle();
	void levelHigh();
	void levelMaster();

	void helpAbout();
	void helpAboutQt();

private:
        void init();

	QPrinter *m_printer;
	QFont m_font;
	bool m_changed;

public:
	void setChanged( bool changed = true ) { m_changed = changed; }
};



#endif
