#include "SceneState.h"

void DrawLineState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	const QPointF startPoint = event->position();
	SetDragEnterPoint(startPoint);

	Scene* scene = Scene::GetInstance();
	mTempLine = DrawInitialLine(startPoint, camera);
	scene->AddLine(mTempLine);
}

void DrawLineState::HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	std::optional<QPointF> startPoint = SceneState::GetDragEnterPoint();

	if (startPoint.has_value())
	{
		const QPointF currPoint = event->position();
		const QPointF viewMove = currPoint - startPoint.value();
		const QPointF modelMove = 1 / camera.GetViewTransformScaleFactor() * viewMove;

		mTempLine->GetEndPoint()->ModelTranslate(modelMove);

		SetDragEnterPoint(currPoint);
	}
}

void DrawLineState::HandleMouseReleaseEvent(QMouseEvent* event, const Camera&)
{
	SetDragEnterPoint(std::nullopt);

	mTempLine = nullptr;
	Enter(StateInfo::CAMERA_MOVE, []() {
		return new CameraMoveState;
		});
}
