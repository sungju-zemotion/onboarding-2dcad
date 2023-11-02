#include "ViewPort.h"
#include "Line.h"
#include "Face.h"
#include "Modal.h"
#include "Scene.h"
#include "SceneState.h"
#include "SceneStateMachine.h"

ViewPort::ViewPort(QWidget* parent) : QWidget(parent), mPainter(new QPainter(this))
{
	setGeometry(100, 100, WIDTH, HEIGHT);
	setMinimumSize(HEIGHT, HEIGHT);

	mSceneStateMachine = SceneStateMachine::GetInstance();
}

void ViewPort::Render(const Camera& camera)
{
	SceneStateMachine::GetInstance()->GetCurrentState()->Render(mPainter, camera);
}

void ViewPort::paintEvent(QPaintEvent* event)
{
	mPainter->begin(this);
	DrawGrid();
	Render(mCamera);
	mPainter->end();

	update();
}

void ViewPort::ShowContextMenu(const QPoint& point)
{
	QMenu menu(this);
	QAction* copyAction = new QAction("copy", this);
	QAction* deleteAction = new QAction("delete", this);
	QAction* propertiesAction = new QAction("properties", this);

	Shape* target = Scene::GetInstance()->HitTestLine(point, mCamera);

	if (target == nullptr)
	{
		target = Scene::GetInstance()->HitTestFace(point, mCamera);
	}

	if (target == nullptr)
	{
		return;
	}

	connect(propertiesAction, &QAction::triggered, [this, target]() {
		PropertiesModal modal(target, this);
		modal.exec();
	});

	menu.addAction(copyAction);
	menu.addAction(deleteAction);
	menu.addAction(propertiesAction);

	menu.exec(mapToGlobal(point));
}

void ViewPort::mousePressEvent(QMouseEvent* event)
{
	SceneState* currentState = SceneStateMachine::GetInstance()->GetCurrentState();
	currentState->HandleMousePressEvent(event, mCamera);
}

void ViewPort::mouseMoveEvent(QMouseEvent* event)
{
	SceneState* currentState = SceneStateMachine::GetInstance()->GetCurrentState();
	currentState->HandleMouseMoveEvent(event, mCamera);
}

void ViewPort::mouseReleaseEvent(QMouseEvent* event)
{
	SceneState* currentState = SceneStateMachine::GetInstance()->GetCurrentState();
	currentState->HandleMouseReleaseEvent(event, mCamera);
}

void ViewPort::wheelEvent(QWheelEvent* event)
{
	SceneState* currentState = SceneStateMachine::GetInstance()->GetCurrentState();
	currentState->HandleMouseWheelEvent(event, &mCamera);
}

void ViewPort::DrawGrid()
{
	QPen gridPen;
	gridPen.setColor(Qt::gray);
	mPainter->setPen(gridPen);

	const QPointF offset = mCamera.GetViewTransform({ 0, 0 });
	const int spacing = 50 * mCamera.GetViewTransformScaleFactor();

	for (int i = spacing; i < width(); i += spacing)
	{
		mPainter->drawLine(i, offset.x(), i, HEIGHT);
	}

	for (int j = spacing; j < height(); j += spacing)
	{
		mPainter->drawLine(offset.y(), j, WIDTH, j);
	}
}
