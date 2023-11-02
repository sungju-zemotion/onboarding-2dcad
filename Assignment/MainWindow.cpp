#include "MainWindow.h"
#include "SceneStateMachine.h"
#include "SceneState.h"

// remark: temporary
#include "File.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), mUi(new MainWindowUI(this))
{
	SceneStateMachine::GetInstance()->SetCurrentState(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
	mUi->SetupUI();
	SetupSignals();
}

MainWindow::~MainWindow()
{
}

void MainWindow::SetupSignals()
{
	// file save, file load
	// TODO: implement
	connect(mUi->mMainMenuBar->mActionSave, &QAction::triggered, [this]() {
		QJsonObject file;
		Scene::GetInstance()->Save(file);

		SaveJson(file, "test.json");
	});
	connect(mUi->mMainMenuBar->mActionLoad, &QAction::triggered, [this]() {
		QJsonObject file = LoadJson("test.json");
		Scene::GetInstance()->Load(file);
	});

	connect(mUi->mCreateToolBar->mActionCreateLine, &QAction::triggered, [this]() { SceneStateMachine::GetInstance()->SetCurrentState(StateInfo::DRAW_LINE, []() { return new DrawLineState; }); });

	connect(mUi->mSelectToolBar->mActionSelectLine, &QAction::triggered, [this]() { SceneStateMachine::GetInstance()->SetCurrentState(StateInfo::SELECT_LINE, []() { return new SelectLineState; }); });

	connect(mUi->mCreateToolBar->mActionCreateFace, &QAction::triggered, [this]() { SceneStateMachine::GetInstance()->SetCurrentState(StateInfo::DRAW_FACE, []() { return new DrawFaceState; }); });

	connect(mUi->mSelectToolBar->mActionSelectFace, &QAction::triggered, [this]() { SceneStateMachine::GetInstance()->SetCurrentState(StateInfo::SELECT_FACE, []() { return new SelectFaceState; }); });

	mUi->mViewPort->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(mUi->mViewPort, &ViewPort::customContextMenuRequested, [this](const QPoint& point) { mUi->mViewPort->ShowContextMenu(point); });
}

void MainWindowUI::SetupUI()
{
	mMainWindow->resize(800, 600);
	mMainWindow->setWindowTitle("Assignment Window");

	mMainMenuBar = MainMenuBarFactory();
	mCreateToolBar = CreateToolBarFactory();
	mSelectToolBar = SelectToolBarFactory();
	mViewPort = ViewPortFactory();
}

MainMenuBar* MainWindowUI::MainMenuBarFactory()
{
	MainMenuBar* menuBar = new MainMenuBar(mMainWindow);
	menuBar->setGeometry(0, 0, 780, 50);
	mMainWindow->setMenuBar(menuBar);

	return menuBar;
}

CreateToolBar* MainWindowUI::CreateToolBarFactory()
{
	CreateToolBar* toolBar = new CreateToolBar(mMainWindow);
	mMainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);

	return toolBar;
}

SelectToolBar* MainWindowUI::SelectToolBarFactory()
{
	SelectToolBar* toolBar = new SelectToolBar(mMainWindow);
	mMainWindow->addToolBar(Qt::BottomToolBarArea, toolBar);

	return toolBar;
}

ViewPort* MainWindowUI::ViewPortFactory()
{
	return new ViewPort(mMainWindow);
}
