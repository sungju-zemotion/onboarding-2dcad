#include "SceneState.h"

void EditPointState::Render(QPainter* painter, const Camera& camera) const
{
	SceneState::Render(painter, camera);
	mPoint->RenderSurroundingRect(painter, camera);
}

void EditPointState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	Point* point = scene->HitTestPoint(event->position(), camera);
	const ShapeId curr = Scene::GetInstance()->GetCurrentFocus();
	mPoint = scene->GetPoint(curr);

	if (point != mPoint)
	{
		Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
	}
}

void EditPointState::HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	std::optional<QPointF> startPoint = SceneState::GetDragEnterPoint();

	if (startPoint.has_value() && (scene->GetCurrentFocus() > 0)) // check whether there is a currently selected shape
	{
		const QPointF viewMove = event->position() - startPoint.value();
		const QPointF modelMove = 1 / camera.GetViewTransformScaleFactor() * viewMove;

		mPoint->Translate(modelMove);
		SetDragEnterPoint(event->position());
	}
}

void EditPointState::HandleMouseReleaseEvent(QMouseEvent* event, const Camera&)
{
	SetDragEnterPoint(std::nullopt);
	Scene::GetInstance()->SetCurrentFocus(-1);
	Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
}
