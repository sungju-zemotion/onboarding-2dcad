#include "Line.h"

void SaveLine(Line* line, QJsonArray& array)
{
	QJsonObject lineJson;

	lineJson["id"] = line->GetId();
	lineJson["color"] = line->GetColor().name();
	lineJson["start"] = line->GetStartPoint()->GetId();
	lineJson["end"] = line->GetEndPoint()->GetId();

	array.append(lineJson);
}

Line* LoadLine(const QJsonObject& json, const QMap<ShapeId, Point*>& pointTable)
{
	const ShapeId startPointId = QVariant(json["start"]).toInt();
	std::shared_ptr<Point> startPoint = std::shared_ptr<Point>(pointTable[startPointId]);

	const ShapeId endPointId = QVariant(json["end"]).toInt();
	std::shared_ptr<Point> endPoint = std::shared_ptr<Point>(pointTable[endPointId]);

	const ShapeId lineId = QVariant(json["id"]).toInt();
	const QColor color = QColor(QVariant(json["color"]).toString());

	return new Line(lineId, color, startPoint, endPoint);
}

Line::Line(ShapeId id, const QColor& color, std::shared_ptr<Point> start, std::shared_ptr<Point> end)
	: Shape(id, color), mStart(start), mEnd(end)
{
	if (IsInvalid(start.get(), end.get()))
	{
		throw std::logic_error("Line not valid");
	}
}

bool Line::HitTest(qreal x, qreal y, const Camera& camera)
{
	const QPointF p1 = mStart->GetViewPoint(camera);
	const QPointF p2 = mEnd->GetViewPoint(camera);
	const qreal x1 = p1.x() - x, y1 = p1.y() - y;
	const qreal x2 = p2.x() - x, y2 = p2.y() - y;

	// check the intersection coordinate with the x-axis
	const qreal tolerance = 5;
	const qreal padding = 5;
	if (-tolerance < (y1 - y2) && (y1 - y2) < tolerance)
	{
		const qreal max = (x1 < x2 ? x2 : x1) + padding;
		const qreal min = (x1 < x2 ? x1 : x2) - padding;

		return min < 0 && 0 < max && -padding < y1 && y1 < padding && -padding < y2 && y2 < padding;
	}

	const qreal xIntersect = (-y1 * (x2 - x1) / (y2 - y1)) + x1;

	return xIntersect - padding < 0 && 0 < xIntersect + padding;
}

void Line::Render(QPainter* painter, const Camera& camera)
{
	const QPointF startPoint = mStart->GetViewPoint(camera);
	const QPointF endPoint = mEnd->GetViewPoint(camera);

	// draw line
	painter->setPen(QPen(GetColor(), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(startPoint, endPoint);

	mStart->Render(painter, camera);
	mEnd->Render(painter, camera);
}

bool Line::IsInvalid(Point* start, Point* end) { return start == end; }

void Line::Rotate(qreal angle, const QPointF& center)
{
	// TODO: consider when using zoom & panning later
	QPointF rotCenter = center;
	if (center == QPointF{ -1, -1 })
	{
		rotCenter = QPointF{
			(mStart->GetX() + mEnd->GetX()) / 2,
			(mStart->GetY() + mEnd->GetY()) / 2
		};
	}

	mStart->UpdateTranslate(-rotCenter);
	mStart->UpdateRotate(angle);
	mStart->UpdateTranslate(rotCenter);

	mEnd->UpdateTranslate(-rotCenter);
	mEnd->UpdateRotate(angle);
	mEnd->UpdateTranslate(rotCenter);
}

void Line::Translate(const QPointF& move)
{
	// TODO: consider when using zoom & panning later
	mStart->UpdateTranslate(move);
	mEnd->UpdateTranslate(move);
}

void Line::Scale(qreal factor, const QPointF& center)
{
	// TODO: consider when using zoom & panning later
	QPointF scaleCenter = center;
	if (center == QPointF{ -1, -1 })
	{
		scaleCenter = GetCenter();
	}

	mStart->UpdateTranslate(-scaleCenter);
	mStart->UpdateScale(factor);
	mStart->UpdateTranslate(scaleCenter);

	mEnd->UpdateTranslate(-scaleCenter);
	mEnd->UpdateScale(factor);
	mEnd->UpdateTranslate(scaleCenter);
}

QPointF Line::GetCenter() const
{
	return QPointF{
		(mStart->GetX() + mEnd->GetX()) / 2,
		(mStart->GetY() + mEnd->GetY()) / 2
	};
}

void Line::RenderSurroundingRect(QPainter* painter, const Camera& camera)
{
	QPointF v1 = mEnd->GetViewPoint(camera) - mStart->GetViewPoint(camera);
	const qreal length = std::sqrt(v1.x() * v1.x() + v1.y() * v1.y());
	if (length == 0)
	{
		return;
	}

	v1 = v1 / length;
	QPointF v2 = { -v1.y(), v1.x() };

	const int padding = 10;
	QPointF x = (-v1 + v2) * padding + mStart->GetViewPoint(camera);
	QPointF y = x - 2 * v2 * padding;
	QPointF z = (v1 - v2) * padding + mEnd->GetViewPoint(camera);
	QPointF u = z + 2 * v2 * padding;


	painter->setPen(Qt::DotLine);
	painter->drawPolyline({ x, y, z, u, x });
}

void Line::ModelTranslate(const QPointF& move)
{
	mStart->ModelTranslate(move);
	mEnd->ModelTranslate(move);
}
