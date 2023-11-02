#include "SceneState.h"

std::optional<QPointF> SceneState::GetDragEnterPoint()
{
	return gDragEnterPoint;
}

void SceneState::SetDragEnterPoint(const std::optional<QPointF>& point)
{
	gDragEnterPoint = point;
}

std::optional<QPointF> SceneState::gDragEnterPoint = std::nullopt;

void SceneState::Render(QPainter* painter, const Camera& camera)
{
	Scene* scene = Scene::GetInstance();

	for (LineTable::const_iterator it = scene->GetLineBegin(); it != scene->GetLineEnd(); it++)
	{
		(*it)->Render(painter, camera);
	}

	for (FaceTable::const_iterator it = scene->GetFaceBegin(); it != scene->GetFaceEnd(); it++)
	{
		(*it)->Render(painter, camera);
	}
}

void SceneState::HandleMouseWheelEvent(QWheelEvent* event, Camera* camera)
{
	const qreal rollData = event->angleDelta().y(); // 0, 120 for usual mouses for upper direction
	const qreal roll = rollData > 0 ? rollData / 100 : -100 / rollData;

	camera->Zoom(roll, event->position());
}

Line* DrawInitialLine(const QPointF& startPoint, const Camera& camera)
{
	QPointF modelPoint = camera.FromViewTransform(startPoint);

	Scene* scene = Scene::GetInstance();

	return new Line(
		scene->GetNextShapeId(),
		Qt::red,
		std::make_shared<Point>(scene->GetNextShapeId(), modelPoint.x(), modelPoint.y()),
		std::make_shared<Point>(scene->GetNextShapeId(), modelPoint.x(), modelPoint.y())
	);
}


Line* DrawConnectedLine(
	std::shared_ptr<Point> lastPoint,
	const QPointF& currPoint,
	const Camera& camera
)
{
	Scene* scene = Scene::GetInstance();
	const QPointF modelPoint = camera.FromViewTransform(currPoint);

	return new Line(
		scene->GetNextShapeId(),
		Qt::red,
		std::shared_ptr<Point>(lastPoint),
		std::make_shared<Point>(scene->GetNextShapeId(), modelPoint.x(), modelPoint.y())
	);
}

