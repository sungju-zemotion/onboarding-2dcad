#include "SceneState.h"

void EditLineState::Render(QPainter* painter, const Camera& camera) const
{
	SceneState::Render(painter, camera);
	mLine->RenderSurroundingRect(painter, camera);
}

void EditLineState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	Line* line = Scene::GetInstance()->HitTestLine(event->position(), camera);

	if (line != mLine)
	{
		Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
	}
}

void EditLineState::HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	std::optional<QPointF> startPoint = SceneState::GetDragEnterPoint();

	if (startPoint.has_value() && (scene->GetCurrentFocus() > 0)) // check whether there is a currently selected shape
	{
		QPointF viewMove = event->position() - startPoint.value();
		QPointF modelMove = 1 / camera.GetViewTransformScaleFactor() * viewMove;

		mLine->ModelTranslate(modelMove);
		SetDragEnterPoint(event->position());
	}
}

void EditLineState::HandleMouseReleaseEvent(QMouseEvent* event, const Camera&)
{
	SetDragEnterPoint(std::nullopt);
	Scene::GetInstance()->SetCurrentFocus(-1);
	Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
}
