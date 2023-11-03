#pragma once

// system
#include <stdexcept>
#include <cmath>

// internal
#include "Line.h"
#include "Camera.h"

class Face : public Shape
{
public:
	QVector<std::shared_ptr<Point>>::const_iterator GetBeginPoint() const
	{
		return mPoints.begin();
	}
	QVector<std::shared_ptr<Point>>::const_iterator GetEndPoint() const
	{
		return mPoints.end();
	}

	Face(ShapeId id, const QColor& color, const QVector<Line*>& lines);
	Face(ShapeId id, const QColor& color, const QVector<Point*>& points);

	bool HitTest(qreal x, qreal y, const Camera& camera) const override;

	void Render(QPainter* painter, const Camera& camera) const override;

	void RenderSurroundingRect(QPainter* painter, const Camera& camera) const override;

	void Rotate(qreal angle, const QPointF& center = { -1, -1 }) override;
	void Translate(const QPointF& move) override;
	void Scale(qreal factor, const QPointF& center = { -1, -1 }) override;

	friend void SaveFace(Face* face, QJsonArray& array);

	void ModelTranslate(const QPointF& move) override;

private:
	static bool IsInvalid(const QVector<Line*>& lines);

	QPointF GetCenter() const;

	QVector<std::shared_ptr<Point>> mPoints;
	QPolygonF mPolygon;
};

Face* LoadFace(const QJsonObject& json, QMap<ShapeId, Point*>& pointTable);
