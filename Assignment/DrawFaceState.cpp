#include "SceneState.h"

void DrawFaceState::HandleMousePressEvent(QMouseEvent* event, const Camera& camera)
{
	// if it is the first line drawn, we set the drag enter point
	const QPointF startPoint = event->position();
	Line* currLine = mTempLines.isEmpty() ? DrawInitialLine(startPoint, camera) : DrawConnectedLine(mTempLines.last()->GetEndPoint(), startPoint, camera);

	Scene::GetInstance()->AddLine(currLine);
	mTempLines.append(currLine);

	SetDragEnterPoint(startPoint);
}

void DrawFaceState::HandleMouseMoveEvent(QMouseEvent* event, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();
	std::optional<QPointF> startPoint = SceneState::GetDragEnterPoint();

	if (startPoint.has_value() && !mTempLines.isEmpty())
	{
		const QPointF currPoint = event->position();
		const QPointF viewMove = currPoint - startPoint.value();
		const QPointF modelMove = 1 / camera.GetViewTransformScaleFactor() * viewMove;

		mTempLines.last()->GetEndPoint()->ModelTranslate(modelMove);

		SetDragEnterPoint(currPoint);
	}
}

void DrawFaceState::HandleMouseReleaseEvent(QMouseEvent* event, const Camera& camera)
{
	const QPointF currPoint = event->position();
	const QPointF initPoint = mTempLines.first()->GetStartPoint()->GetViewPoint(camera);

	// if the current point is colliding with the initial point, then
	// we finally draw a face
	const qreal padding = 10;

	if (currPoint.x() - padding < initPoint.x() && initPoint.x() < currPoint.x() + padding && currPoint.y() - padding < initPoint.y() && initPoint.y() < currPoint.y() + padding)
	{
		Scene* scene = Scene::GetInstance();
		Line* currLine = mTempLines.last();

		std::shared_ptr<Point> firstPoint = mTempLines.first()->GetStartPoint();
		currLine->SetEndPoint(std::shared_ptr(firstPoint));

		Face* face = new Face(scene->GetNextShapeId(), Qt::gray, mTempLines);
		scene->AddFace(face);

		// clear the resources
		mTempLines.clear();
		Scene::GetInstance()->SetCurrentFocus(-1);
		Enter(StateInfo::CAMERA_MOVE, []() { return new CameraMoveState; });
	}

	SetDragEnterPoint(std::nullopt);
}
