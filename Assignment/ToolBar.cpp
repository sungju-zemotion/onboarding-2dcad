#include "ToolBar.h"

CreateToolBar::CreateToolBar(QWidget* parent)
	: QToolBar(parent)
{
	// actions
	QAction* createLine = new QAction("Line", this);
	addAction(createLine);

	QAction* createFace = new QAction("Face", this);
	addAction(createFace);

	mActionCreateLine = createLine;
	mActionCreateFace = createFace;
}

SelectToolBar::SelectToolBar(QWidget* parent)
	: QToolBar(parent)
{
	// actions
	QAction* selectLine = new QAction("Line", this);
	addAction(selectLine);
	mActionSelectLine = selectLine;

	QAction* selectFace = new QAction("Face", this);
	addAction(selectFace);
	mActionSelectFace = selectFace;
}

