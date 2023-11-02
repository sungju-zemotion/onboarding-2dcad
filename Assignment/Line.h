#pragma once

// system
#include <memory>
#include <stdexcept>
#include <cmath>

// internal
#include "ShapeCommon.h"
#include "Camera.h"

class Line : public Shape
{
public:
	Line(ShapeId id, const QColor& color, std::shared_ptr<Point> start, std::shared_ptr<Point> end);

	std::shared_ptr<Point> GetStartPoint() const { return mStart; }
	std::shared_ptr<Point> GetEndPoint() const { return mEnd; }
	void SetEndPoint(std::shared_ptr<Point> point) { mEnd = point; }

	bool HitTest(qreal x, qreal y, const Camera& camera) override; // aabb(not right now) can be used instead
	void Render(QPainter* painter, const Camera& camera) override;

	void RenderSurroundingRect(QPainter* painter, const Camera& camera) override;

	void Rotate(qreal angle, const QPointF& center = { -1, -1 });
	void Translate(const QPointF& move);
	void Scale(qreal factor, const QPointF& center = { -1, -1 });

	void ModelTranslate(const QPointF& move) override;

	friend void SaveLine(Line* line, QJsonArray& array);

private:
	static bool IsInvalid(Point* start, Point* end);

	std::shared_ptr<Point> mStart;
	std::shared_ptr<Point> mEnd;

	QPointF GetCenter() const;

	// TODO: implement the following data for data I/O
	//static Line* FromJson(const QJsonObject& json);
	//static QJsonObject ToJson(const Line& line);
};

Line* LoadLine(const QJsonObject& json, const QMap<ShapeId, Point*>& pointTable);
