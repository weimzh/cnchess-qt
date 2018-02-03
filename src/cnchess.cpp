#include "cnchess.h"

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>

#include <qaction.h>

#include "cnchessview.h"

#include "board.h"

#include "images/file_new.xpm"
#include "images/setting_red.xpm"
#include "images/setting_blue.xpm"
#include "images/help_about.xpm"
#include "images/edit_undo.xpm"
#include "images/edit_redo.xpm"

QString applicationdir;

int settingRedid, settingBlueid;
int levelNewerid, levelPrimaryid, levelMiddleid, levelHighid, levelMasterid ;

QPopupMenu *file, *operation, *setting, *level, *help;

MyMainWindow::MyMainWindow()
        : QMainWindow( 0,"MainWindow", WDestructiveClose )
{
	m_printer = new QPrinter;
	QPixmap newIcon, openIcon, saveIcon, printIcon, redIcon, blueIcon, aboutIcon, undoIcon, redoIcon;

	QToolBar * fileTools = new QToolBar( this, "file operations" );
	fileTools->setLabel( tr("File Operations") );

	newIcon = QPixmap( filenew );
	QToolButton * fileNew
		= new QToolButton( newIcon, tr("New Game"), QString::null,
			this, SLOT(fileNew()), fileTools, "New Game" );

	redIcon = QPixmap( settingred );
	QToolButton * settingRed
		= new QToolButton( redIcon, tr("Red First"), QString::null,
			this, SLOT(settingRed()), fileTools, "Red First" );

	blueIcon = QPixmap( settingblue );
	QToolButton * settingBlue
		= new QToolButton( blueIcon, tr("Black First"), QString::null,
			this, SLOT(settingBlue()), fileTools, "Black First" );

	undoIcon = QPixmap( editundo );
	QToolButton * editUndo
		= new QToolButton( undoIcon, tr("Undo"), QString::null,
			this, SLOT(editUndo()), fileTools, "Undo" );

	redoIcon = QPixmap( editredo );
	QToolButton * editRedo
		= new QToolButton( redoIcon, tr("Redo"), QString::null,
			this, SLOT(editRedo()), fileTools, "Redo" );

	aboutIcon = QPixmap( helpabout );
	QToolButton * helpAbout
		= new QToolButton( aboutIcon, tr("About"), QString::null,
			this, SLOT(helpAbout()), fileTools, "About" );

	file = new QPopupMenu( this );
	menuBar()->insertItem( tr("File"), file );
	file->insertItem( tr("New"), this, SLOT(fileNew()), CTRL+Key_N );
	file->insertItem( tr("Exit"), qApp, SLOT( closeAllWindows() ), CTRL+Key_Q );

	operation = new QPopupMenu( this );
	menuBar()->insertItem( tr("Game"), operation );	
	operation->insertItem( tr("Undo"), this, SLOT(editUndo()), CTRL+Key_Z );
	operation->insertItem( tr("Redo"), this, SLOT(editRedo()) );

	setting = new QPopupMenu( this );
	menuBar()->insertItem( tr("Settings"), setting );
	setting->setCheckable(true);
	settingRedid=setting->insertItem( tr("Red First"), this, SLOT(settingRed()) );
	setting->setItemChecked(settingRedid,true);
	settingBlueid=setting->insertItem( tr("Black First"), this, SLOT(settingBlue()) );

	level = new QPopupMenu( this );
	menuBar()->insertItem( tr("Level"), level );
	level->setCheckable(true);
	levelNewerid=level->insertItem( tr("Novice"), this, SLOT(levelNewer()));
	level->insertSeparator(-1);
	levelPrimaryid=level->insertItem( tr("Easy"), this, SLOT(levelPrimary()));
	levelMiddleid=level->insertItem( tr("Normal"), this, SLOT(levelMiddle()));
	level->setItemChecked(levelMiddleid,true);
	levelHighid=level->insertItem( tr("Hard"), this, SLOT(levelHigh()));
	level->insertSeparator(-1);
	levelMasterid=level->insertItem( tr("Master"), this, SLOT(levelMaster()));

	help = new QPopupMenu( this );
	menuBar()->insertItem( tr("Help"), help );
	help->insertItem( tr("About"), this, SLOT(helpAbout()), Key_F1 );
	help->insertItem( tr("About Qt"), this, SLOT(helpAboutQt()) );

	m_cnchessview = new CCnchessView( this ,"CnchessView",0);
	setCentralWidget( m_cnchessview );
	
	m_cnchessview->show();

	//  add draw code for native data here
	statusBar()->message( tr("Ready"), 2000 );

	setFixedSize(330, 430);

}

MyMainWindow::~MyMainWindow()
{
}

void MyMainWindow::init()
{
	m_changed = false;
}

void MyMainWindow::fileNew()
{
	m_Board.InitData();
	m_cnchessview->repaintContents(true);

	if(m_Board.firstflag==true){
		m_Board.m_side_interface = RED;
		m_Board.m_xside_interface = BLUE;
		m_Board.m_gameState = HUMANTHINKING;
		
	}
	else{
		m_Board.m_side_interface = BLUE;
		m_Board.m_xside_interface = RED;
		m_Board.m_gameState = COMPUTERTHINKING;
		m_cnchessview->OnLetComputerThink();
	}	
}

void MyMainWindow::editUndo()
{
	m_Board.m_pieceSelecting=NOMOVE; // 本语句表示在撤消操作中不需要显示哪个棋子被选中。
	short from, dest;
	m_Board.UndoMove(from, dest);
	m_cnchessview->RequireDrawCell(dest);
	m_cnchessview->RequireDrawCell(from);
	
	
}

void MyMainWindow::editRedo()
{
	short  from,dest;
	
	m_Board.RedoMove(from, dest);
	m_cnchessview->RequireDrawCell(from);
	m_cnchessview->RequireDrawCell(dest);
	
}

void MyMainWindow::settingRed()
{
	// 置菜单项被选中标志
	setting->setItemChecked(settingBlueid,false);
	setting->setItemChecked(settingRedid,true);

	// 新一局游戏的状态设置为“人先走棋”。
	m_Board.firstflag = true;
	m_Board.m_gameState = HUMANTHINKING;

	// 设置m_side代表红方，m_xside代表蓝方
	m_Board.m_side_interface = RED;
	m_Board.m_xside_interface = BLUE;

	m_Board.InitData();
	m_cnchessview->repaintContents(true);	
}

void MyMainWindow::settingBlue()
{
	// 置菜单项被选中标志
	setting->setItemChecked(settingRedid,false);
	setting->setItemChecked(settingBlueid,true);

	//新一局游戏的状态设置为“电脑先走棋”。
	m_Board.firstflag = false;
	m_Board.m_gameState = COMPUTERTHINKING;

	// 设置m_side代表蓝方，m_xside代表红方
	m_Board.m_side_interface = BLUE;  //
	m_Board.m_xside_interface = RED;  //

	m_Board.InitData();
	m_cnchessview->repaintContents(true);
	m_cnchessview->OnLetComputerThink();
	
}

void MyMainWindow::levelNewer()
{
	// 置游戏的对抗级别为新手，电脑搜索深度（看棋步数）为1步棋
	m_Board.max_ply=1;

	// 置菜单项被选中标志
	level->setItemChecked(levelPrimaryid,false);
	level->setItemChecked(levelMiddleid,false);
	level->setItemChecked(levelHighid,false);
	level->setItemChecked(levelMasterid,false);
	level->setItemChecked(levelNewerid,true);
}

void MyMainWindow::levelPrimary()
{
	// 置游戏的对抗级别为初级，电脑搜索深度（看棋步数）为2步棋
	m_Board.max_ply=2;

	// 置菜单项被选中标志
	level->setItemChecked(levelNewerid,false);
	level->setItemChecked(levelMiddleid,false);
	level->setItemChecked(levelHighid,false);
	level->setItemChecked(levelMasterid,false);
	level->setItemChecked(levelPrimaryid,true);
}

void MyMainWindow::levelMiddle()
{
	// 置游戏的对抗级别为中级，电脑搜索深度（看棋步数）为3步棋
	m_Board.max_ply=3;

	// 置菜单项被选中标志
	level->setItemChecked(levelNewerid,false);
	level->setItemChecked(levelPrimaryid,false);
	level->setItemChecked(levelHighid,false);
	level->setItemChecked(levelMasterid,false);
	level->setItemChecked(levelMiddleid,true);
}

void MyMainWindow::levelHigh()
{
	// 置游戏的对抗级别为高级，电脑搜索深度（看棋步数）为4步棋
	m_Board.max_ply=4;

	// 置菜单项被选中标志
	level->setItemChecked(levelNewerid,false);
	level->setItemChecked(levelPrimaryid,false);
	level->setItemChecked(levelMiddleid,false);
	level->setItemChecked(levelMasterid,false);
	level->setItemChecked(levelHighid,true);
}

void MyMainWindow::levelMaster()
{
	// 置游戏的对抗级别为大师，电脑搜索深度（看棋步数）为4步棋
	m_Board.max_ply=5;

	// 置菜单项被选中标志
	level->setItemChecked(levelNewerid,false);
	level->setItemChecked(levelPrimaryid,false);
	level->setItemChecked(levelMiddleid,false);
	level->setItemChecked(levelHighid,false);
	level->setItemChecked(levelMasterid,true);
}

void MyMainWindow::helpAbout()
{
	QMessageBox::about(0, tr("About Chinese Chess"),
		tr("By frog\nTaian, Shandong\n"
		"Oct 2005\nemail:gorfllew@163.com\n"
		"Thanks yunfan and lfm09"));
}

void MyMainWindow::helpAboutQt()
{
	QMessageBox::aboutQt(0, "About Qt"); 
}
