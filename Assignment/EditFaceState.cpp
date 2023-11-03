#include "SceneState.h"

void EditFaceState::Render(QPainter* painter, const Camera& camera) const
{
	SceneState::Render(painter, camera);
	mFace->RenderSurroundingRect(painter, camera);
}

void EditFaceState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	Face* face = Scene::GetInstance()->HitTestFace(event->position(), camera);

	if (face != mFace)
	{
		Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
	}
}

void EditFaceState::HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	std::optional<QPointF> startPoint = SceneState::GetDragEnterPoint();

	if (startPoint.has_value() && (scene->GetCurrentFocus() > 0)) // check whether there is a currently selected shape
	{
		QPointF viewMove = event->position() - startPoint.value();
		QPointF modelMove = 1 / camera.GetViewTransformScaleFactor() * viewMove;

		mFace->ModelTranslate(modelMove);
		SetDragEnterPoint(event->position());
	}
}

void EditFaceState::HandleMouseReleaseEvent(QMouseEvent* event, const Camera&)
{
	SetDragEnterPoint(std::nullopt);
	Scene::GetInstance()->SetCurrentFocus(-1);
	Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
}
