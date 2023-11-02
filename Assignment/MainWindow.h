#pragma once

// qt
#include <QtWidgets>

// internal
#include "ToolBar.h"
#include "MainMenuBar.h"
#include "ViewPort.h"

class MainWindowUI;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	MainWindowUI* mUi = nullptr;

	void SetupSignals();

};

class MainWindowUI
{
public:
	MainWindowUI(QMainWindow* mainWindow) : mMainWindow(mainWindow) {}

	void SetupUI();

public:
	// menuBar
	MainMenuBar* mMainMenuBar = nullptr;

	// createToolBar
	CreateToolBar* mCreateToolBar = nullptr;

	// selectToolBar
	SelectToolBar* mSelectToolBar = nullptr;

	// viewArea
	ViewPort* mViewPort = nullptr;

private:
	QMainWindow* mMainWindow = nullptr;

	MainMenuBar* MainMenuBarFactory();
	CreateToolBar* CreateToolBarFactory();
	SelectToolBar* SelectToolBarFactory();
	ViewPort* ViewPortFactory();
};
