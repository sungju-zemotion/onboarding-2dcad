#pragma once

// qt
#include <QtCore>
#include <QtWidgets>

// internal
#include "Transform.h"
#include "Camera.h"

using ShapeId = int;		// TODO: if possible, change it to UUID

class Point;

void RenderPoint(QPainter* painter, const QPointF& point, const QColor& color);

class Shape
{
public:
	virtual ~Shape() = default;

	// basic features
	const ShapeId GetId() const { return mId; }
	QColor GetColor() const { return mColor; }
	void SetColor(const QColor& color) { mColor = color; }
	bool GetIsVisible() const { return mIsVisible; }
	void SetIsVisible(bool isVisible) { mIsVisible = isVisible; }
	bool GetIsSelected() const { return mIsSelected; }
	void SetIsSelected(bool isSelected) { mIsSelected = isSelected; }

	virtual void Rotate(qreal angle, const QPointF& center) = 0;
	virtual void Translate(const QPointF& viewTranslation) = 0;
	virtual void Scale(qreal viewFactor, const QPointF& pinPoint) = 0;

	virtual void ModelTranslate(const QPointF& move) = 0;

	// render
	virtual bool HitTest(qreal x, qreal y, const Camera& camera) = 0;		// aabb(not right now) can be used instead
	virtual void Render(QPainter* painter, const Camera& camera) = 0;
	virtual void RenderSurroundingRect(QPainter* painter, const Camera& camera) = 0;

protected:
	Shape(ShapeId id, const QColor& color);

private:
	const ShapeId mId = 0;
	QColor mColor;
	bool mIsVisible = true;
	bool mIsSelected = false;
};

class Point : public Shape
{
public:
	Point(ShapeId id, qreal x, qreal y);

	qreal GetX() const { return mX; }
	void SetX(qreal x) { mX = x; }

	qreal GetY() const { return mY; }
	void SetY(qreal y) { mY = y; }

	QPointF GetCoord() const { return { mX, mY }; }

	bool HitTest(qreal x, qreal y, const Camera& camera) override;
	void Render(QPainter* painter, const Camera& camera) override;

	// TODO: implement the following
	void RenderSurroundingRect(QPainter* painter, const Camera& camera) override;

	void Rotate(qreal angle, const QPointF& center) override {}			// not necessary
	void Translate(const QPointF& viewTranslation) override;
	void Scale(qreal viewFactor, const QPointF& pinPoint) override {}	// not necessary

	QPointF GetViewPoint(const Camera& camera) const;

	void ModelTranslate(const QPointF& move) override;

	friend class Line;
	friend class Face;
	friend void SavePoint(const Point* point, QJsonArray& array);

private:
	qreal mX, mY;		// world coordinate

	// TODO: modify the namings later
	void UpdateRotate(qreal angle) { mTransformation.UpdateRotation(angle); };
	void UpdateTranslate(const QPointF& viewTranslation) { mTransformation.UpdateTranslation(viewTranslation); }
	void UpdateScale(qreal viewFactor) { mTransformation.UpdateScale(viewFactor); }
	QPointF Transform(const QPointF& point) { return mTransformation.Transform(point); }

	TransformMat3 mTransformation;
};

Point* LoadPoint(const QJsonObject& json);

