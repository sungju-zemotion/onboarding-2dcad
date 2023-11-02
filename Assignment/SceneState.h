#pragma once

// qt
#include <QtCore>
#include <QtWidgets>
#include <optional>

// internal
#include "ShapeCommon.h"
#include "Transform.h"
#include "Scene.h"
#include "Line.h"
#include "Face.h"
#include "Camera.h"
#include "SceneStateMachine.h"


Line* DrawInitialLine(const QPointF& startPoint, const Camera& camera);

Line* DrawConnectedLine(
	std::shared_ptr<Point> lastPoint,
	const QPointF& currPoint,
	const Camera& camera
);

class SceneState {

public:
	virtual ~SceneState() = default;

	virtual void Render(QPainter* painter, const Camera& camera);
	// call Shape->Paint(transformation, this);

	// common event handler
	virtual void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) = 0;
	virtual void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) = 0;		// have its own version below
	virtual void HandleMouseReleaseEvent(QMouseEvent* event, const Camera& camera) = 0;
	void HandleMouseWheelEvent(QWheelEvent* event, Camera* camera);

protected:
	SceneState() = default;

	static std::optional<QPointF> GetDragEnterPoint();
	static void SetDragEnterPoint(const std::optional<QPointF>& point);

	virtual void Enter(StateInfo info, std::function<SceneState* ()> stateFactory) {
		SceneStateMachine::GetInstance()->SetCurrentState(info, stateFactory);
	}

private:
	static std::optional<QPointF> gDragEnterPoint;
};

class CameraMoveState : public SceneState
{
public:
	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override;

private:
	void HandleMouseMoveEvent(QMouseEvent* event, Camera& camera);
};

class EditPointState : public SceneState
{
public:
	void Render(QPainter* painter, const Camera& camera) override;

	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override;

	void SetPoint(Point* point) { mPoint = point; }

private:
	Point* mPoint = nullptr;
};

class EditLineState : public SceneState
{
public:
	void Render(QPainter* painter, const Camera& camera) override;

	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override;

	void SetLine(Line* line) { mLine = line; }

private:
	Line* mLine = nullptr;
};

class EditFaceState : public SceneState
{
public:
	void Render(QPainter* painter, const Camera& camera) override;

	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override;

	void SetFace(Face* face) { mFace = face; }

private:
	Face* mFace = nullptr;
};

class SelectLineState : public SceneState
{
public:
	void Render(QPainter* painter, const Camera& camera) override;

	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override {}		// not necessary
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) {}					// not necessary
};

class SelectFaceState : public SceneState
{
public:
	void Render(QPainter* painter, const Camera& camera) override;

	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override {}		// not necessary
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override {}			// not necessary
};

class DrawLineState : public SceneState
{
public:
	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera&) override;

private:
	Line* mTempLine = nullptr;
};

class DrawFaceState : public SceneState
{
public:
	void HandleMousePressEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera) override;
	void HandleMouseReleaseEvent(QMouseEvent* event, const Camera& camera) override;

private:
	QVector<Line*> mTempLines;
};

