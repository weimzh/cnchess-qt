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
#include <qapplication.h>
#include <qlocale.h>
#include "cnchess.h"
#ifdef __APPLE__
#include <stdint.h>
extern "C" extern int _NSGetExecutablePath(char *buf, uint32_t *bufsize);
#endif

int main(int argc, char *argv[])
{
#ifdef __APPLE__
	// Hack for OS X Mavericks or later
	char buf[4096];
	uint32_t size = 4096;
	_NSGetExecutablePath(buf, &size);

	char *p = &buf[strlen(buf) - 1];
	while (*p != '/') {
		p--;
	}
	*p = '\0';
	strcat(buf, "/../../..");
	printf("%s\n", buf);
	chdir(buf);
#endif
	QApplication app( argc, argv );
	QString locale = QLocale::system().name();

	// translation file for application strings
	QTranslator myapp (0);
#ifdef __APPLE__
	myapp.load(QString("cnchess.app/Contents/Resources/cnchess_") + locale, ".");
#elif !defined(_WIN32)
	myapp.load(QString("/usr/share/games/cnchess/cnchess_") + locale, ".");
#else
	myapp.load(QString("cnchess_") + locale, ".");
#endif
	app.installTranslator (&myapp);

	applicationdir=app.applicationDirPath();
	MyMainWindow *mymainwindow=new MyMainWindow;

	app.setMainWidget(mymainwindow);

	mymainwindow->show();
	app.connect( &app, SIGNAL(lastWindowClosed()),mymainwindow, SLOT(fileQuit()) );
	return app.exec();
}
