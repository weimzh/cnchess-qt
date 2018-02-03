TEMPLATE = app

HEADERS += cnchess.h \
           cnchessview.h \
	   defs.h \
	   board.h \
	   graphic.h

SOURCES += cnchess.cpp \
           cnchessview.cpp \
	   main.cpp \
	   board.cpp\
	   eval.cpp \
	   gen.cpp \
	   graphic.cpp\
	   move.cpp\
	   openbook.cpp\
	   search.cpp

TARGET = ../bin/cnchess

TRANSLATIONS += cnchess_zh_CN.ts

unix {
	INSTALLDIR = $$(DESTDIR)/usr/share/games/cnchess

	target.path = $$(DESTDIR)/usr/bin

	desktop.path = $$(DESTDIR)/usr/share/applications/kde
	desktop.files = ../cnchess.desktop

	trans.path = $$INSTALLDIR
	trans.files = ../src/cnchess_zh_CN.qm

	pictures.path = $$INSTALLDIR/chesses
	pictures.files = ../chesses/*.bmp

	books.path = $$INSTALLDIR/book
	books.files = ../book/book.dat

	INSTALLS += target trans desktop books pictures
}
