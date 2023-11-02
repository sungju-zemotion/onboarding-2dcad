#include "ShapeCommon.h"

void RenderPoint(QPainter* painter, const QPointF& point, const QColor& color)
{
	const int padding = 5;
	painter->setPen(Qt::black);
	painter->setBrush(color);
	painter->fillRect(point.x() - padding, point.y() - padding, padding * 2, padding * 2, Qt::blue);
}

Point* LoadPoint(const QJsonObject& json)
{
	const ShapeId id = QVariant(json["id"]).toInt();
	const qreal x = QVariant(json["x"]).toDouble();
	const qreal y = QVariant(json["y"]).toDouble();

	return new Point(id, x, y);
}

void SavePoint(const Point* point, QJsonArray& array)
{
	QJsonObject savedPoint;
	savedPoint["id"] = point->GetId();
	savedPoint["x"] = point->GetX();
	savedPoint["y"] = point->GetY();

	array.append(savedPoint);
}

Shape::Shape(ShapeId id, const QColor& color) : mId(id), mColor(color)
{
}

Point::Point(ShapeId id, qreal x, qreal y) : mX(x), mY(y), Shape(id, Qt::blue)
{
}

bool Point::HitTest(qreal x, qreal y, const Camera& camera)
{
	const QPointF point = GetViewPoint(camera);
	const qreal padding = 10;

	return (x - padding < point.x()) && (point.x() < x + padding) && (y - padding < point.y()) && (point.y() < y + padding);
}

void Point::Render(QPainter* painter, const Camera& camera)
{
	RenderPoint(painter, GetViewPoint(camera), GetColor());
}

void Point::Translate(const QPointF& viewTranslate)
{
	UpdateTranslate(viewTranslate);
}

QPointF Point::GetViewPoint(const Camera& camera) const
{
	const QPointF model = mTransformation.Transform(GetCoord());

	return camera.GetViewTransform(model);
}

void Point::RenderSurroundingRect(QPainter* painter, const Camera& camera)
{
	const int padding = 10;
	const QPointF viewPoint = GetViewPoint(camera);

	painter->setPen(Qt::DotLine);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(viewPoint.x() - padding, viewPoint.y() - padding, 2 * padding, 2 * padding);
}

void Point::ModelTranslate(const QPointF& move)
{
	mX += move.x();
	mY += move.y();
}
