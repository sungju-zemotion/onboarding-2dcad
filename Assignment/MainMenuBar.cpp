#include "MainMenuBar.h"

MainMenuBar::MainMenuBar(QWidget* parent) : QMenuBar(parent)
{
	// file menu
	mFileMenu = new QMenu("file", this);
	addAction(mFileMenu->menuAction());
	mActionSave = new QAction("save", this);
	mActionLoad = new QAction("load", this);
	mFileMenu->addAction(mActionSave);
	mFileMenu->addAction(mActionLoad);

	// edit menu
	QMenu* editMenu = new QMenu("edit", this);
	addAction(editMenu->menuAction());

	// view menu
	QMenu* viewMenu = new QMenu("view", this);
	addAction(viewMenu->menuAction());
}

MainMenuBar::~MainMenuBar()
{
}