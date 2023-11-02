#pragma once

// qt
#include <QtWidgets>

// Remark: temporarily we will put the relative actions
// in the same place as the menus. However, we would like to put
// those actions into the corresponding individual menus.
class MainMenuBar : public QMenuBar
{
	Q_OBJECT;

public:
	MainMenuBar(QWidget* parent);
	~MainMenuBar();

public:
	// filemenu
	QMenu* mFileMenu = nullptr;
	QAction* mActionSave = nullptr;
	QAction* mActionLoad = nullptr;

	// edit menu
	QMenu* mEditMenu = nullptr;

	// view menu
	QMenu* mViewMenu = nullptr;
};
