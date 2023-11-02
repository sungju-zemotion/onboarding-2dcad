#include "Face.h"

void SaveFace(Face* face, QJsonArray& array)
{
	QJsonObject faceJson;

	faceJson["id"] = face->GetId();
	faceJson["color"] = face->GetColor().name();

	QJsonArray pointArray;
	using PointIterator = QVector<std::shared_ptr<Point>>::const_iterator;
	for (PointIterator pit = face->GetBeginPoint(); pit != face->GetEndPoint(); pit++)
	{
		pointArray.append((*pit)->GetId());
	}
	faceJson["points"] = pointArray;

	array.append(faceJson);
}

Face* LoadFace(const QJsonObject& json, QMap<ShapeId, Point*>& pointTable)
{
	QJsonArray pointIdsArray = json["points"].toArray();
	QVector<Point*> points;

	for (const QJsonValue value : pointIdsArray)
	{
		points.append(pointTable[value.toInt()]);
	}

	const ShapeId id = QVariant(json["id"]).toInt();
	const QColor color = QColor(QVariant(json["color"]).toString());

	return new Face(id, color, points);
}

static QPointF normalize(const QPointF& point)
{
	const qreal size = std::sqrt(point.x() * point.x() + point.y() * point.y());
	if (size < 0.05)
	{
		throw std::logic_error("unstable point vector");
	}

	return { point.x() / size, point.y() / size };
}

bool Face::IsInvalid(const QVector<Line*>& lines)
{
	// size check: a face must consist of at least three lines
	if (lines.size() < 3)
	{
		return true;
	}

	// linked-list check: two consecutive lines should share one single point
	Point* curr = lines.first()->GetEndPoint().get();

	for (QVector<Line*>::const_iterator it = ++lines.begin(); it != lines.end(); it++)
	{
		if (curr != (*it)->GetStartPoint().get())
		{
			return true;
		}
		curr = (*it)->GetEndPoint().get();
	}

	const Point* firstPoint = lines.first()->GetStartPoint().get();
	return curr != firstPoint;
}

Face::Face(ShapeId id, const QColor& color, const QVector<Line*>& lines) : Shape(id, color)
{
	if (Face::IsInvalid(lines))
	{
		throw std::logic_error("The face is not valid");
	}

	QPolygonF polygon;
	for (Line* line : lines)
	{
		mPoints.append(line->GetStartPoint());

		polygon << QPointF(line->GetStartPoint()->GetX(), line->GetStartPoint()->GetY());
	}
	mPolygon = polygon;
}

Face::Face(ShapeId id, const QColor& color, const QVector<Point*>& points) : Shape(id, color)
{
	QPolygonF polygon;
	for (Point* point : points)
	{
		mPoints.append(std::shared_ptr<Point>(point));
		polygon << QPointF(point->GetCoord());
	}

	mPolygon = polygon;
}

// algorithm reference: https://stackoverflow.com/a/2922778
bool Face::HitTest(qreal x, qreal y, const Camera& camera)
{
	int ret = 0;

	for (size_t i = 0; i < mPoints.size(); i++)
	{
		QPointF curr = mPoints[i]->GetViewPoint(camera);
		QPointF next = mPoints[(i + 1) % mPoints.size()]->GetViewPoint(camera);

		// (y is between cY and nY)  and (x is on the left side of the intersection)
		if (((y < curr.y()) != (y < next.y())) && (x < (next.x() - curr.x()) * (y - curr.y()) / (next.y() - curr.y()) + curr.x()))
		{
			ret++;
		}
	}

	return (ret & 1) == 1;
}

// reference: https://www.bogotobogo.com/Qt/Qt5_QPainterPath_QPolygon.php
void Face::Render(QPainter* painter, const Camera& camera)
{
	UpdatePolygon(camera);
	painter->drawPolygon(mPolygon);

	QBrush brush;
	brush.setColor(GetColor());
	brush.setStyle(Qt::SolidPattern);

	QPainterPath path;
	path.addPolygon(mPolygon);

	painter->fillPath(path, brush);
}

void Face::UpdatePolygon(const Camera& camera)
{
	QPolygonF newPolygon;
	for (std::shared_ptr<const Point> point : mPoints)
	{
		newPolygon << point->GetViewPoint(camera);
	}
	mPolygon = newPolygon;
}

void Face::Rotate(qreal angle, const QPointF& center)
{
	QPointF rotCenter = center;
	if (center == QPointF{ -1, -1 })
	{
		rotCenter = GetCenter();
	}
	for (std::shared_ptr<Point> point : mPoints)
	{
		point->UpdateTranslate(-rotCenter);
		point->UpdateRotate(angle);
		point->UpdateTranslate(rotCenter);
	}
}

void Face::Translate(const QPointF& move)
{
	for (std::shared_ptr<Point> point : mPoints)
	{
		point->UpdateTranslate(move);
	}
}

void Face::Scale(qreal factor, const QPointF& center)
{
	QPointF scaleCenter = center;
	if (center == QPointF{ -1, -1 })
	{
		scaleCenter = GetCenter();
	}

	for (std::shared_ptr<Point> point : mPoints)
	{
		point->UpdateTranslate(-scaleCenter);
		point->UpdateScale(factor);
		point->UpdateTranslate(scaleCenter);
	}
}

QPointF Face::GetCenter() const
{
	qreal x = 0, y = 0;
	unsigned int count = 0;
	for (std::shared_ptr<const Point> point : mPoints)
	{
		x += point->GetX();
		y += point->GetY();
		count++;
	}

	return { x / count, y / count };
}

void Face::RenderSurroundingRect(QPainter* painter, const Camera& camera)
{
	const int padding = 10;
	const int limit = 1 << 20;
	qreal top = limit, bottom = -limit, left = limit, right = -limit;

	for (std::shared_ptr<const Point> point : mPoints)
	{
		const QPointF viewPoint = point->GetViewPoint(camera);
		top = std::min(viewPoint.y(), top);
		bottom = std::max(viewPoint.y(), bottom);
		left = std::min(viewPoint.x(), left);
		right = std::max(viewPoint.x(), right);
	}

	painter->setPen(Qt::DotLine);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(left - padding, top - padding, right - left + 2 * padding, bottom - top + 2 * padding);
}

void Face::ModelTranslate(const QPointF& move)
{
	for (std::shared_ptr<Point> point : mPoints)
	{
		point->ModelTranslate(move);
	}
}
