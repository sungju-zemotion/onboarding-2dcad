#include "SceneState.h"

void CameraMoveState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	SetDragEnterPoint(event->position());
}

void CameraMoveState::HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera)
{
	HandleMouseMoveEvent(event, const_cast<Camera&>(camera));
}

void CameraMoveState::HandleMouseMoveEvent(QMouseEvent* event, Camera& camera)
{
	if (std::optional<QPointF> startPoint = SceneState::GetDragEnterPoint())
	{
		const QPointF currentPoint = event->position();
		Scene* currentScene = Scene::GetInstance();
		const QPointF move = currentPoint - startPoint.value();
		camera.Pan(-move.x(), -move.y());

		SetDragEnterPoint(currentPoint);
	}
}

void CameraMoveState::HandleMouseReleaseEvent(QMouseEvent* event, const Camera&)
{
	SetDragEnterPoint(std::nullopt);
}

