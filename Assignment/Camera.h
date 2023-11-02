#pragma once

// qt
#include <QtCore>
#include <QtWidgets>

// internal
#include "Transform.h"

class Camera
{
public:
	Camera(const QPoint& origin = { 0, 0 });

	void Pan(qreal dx, qreal dy);
	void Zoom(qreal scale, const QPointF& center);

	QPointF GetViewTransform(const QPointF& modelPoint) const;
	QPointF FromViewTransform(const QPointF& viewPoint) const;

	qreal GetViewTransformScaleFactor() const { return mViewTransform.GetScaleFactor(); }
	void ResetView();

private:
	QPoint mOrigin;
	TransformMat3 mViewTransform;		// from model(world) to view
	TransformMat3 mModelTransform;		// from view to model(world)
};

