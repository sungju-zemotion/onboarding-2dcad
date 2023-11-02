#pragma once

// qt
#include <QtWidgets>

class CreateToolBar : public QToolBar
{
	Q_OBJECT;

public:
	CreateToolBar(QWidget* parent = nullptr);

public:
	QAction* mActionCreateLine = nullptr;
	QAction* mActionCreateFace = nullptr;
};

class SelectToolBar : public QToolBar
{
	Q_OBJECT;

public:
	SelectToolBar(QWidget* parent = nullptr);

public:
	QAction* mActionSelectLine = nullptr;
	QAction* mActionSelectFace = nullptr;
};
