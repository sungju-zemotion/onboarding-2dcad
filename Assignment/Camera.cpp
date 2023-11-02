#include "Camera.h"

Camera::Camera(const QPoint& origin) : mOrigin(origin)
{
}

void Camera::Pan(qreal dx, qreal dy)
{
	mViewTransform.UpdateTranslation({ -dx, -dy });
	mModelTransform.UpdateTranslation({ dx, dy });
}

void Camera::Zoom(qreal factor, const QPointF& center)
{
	// the order is important!
	// TODO: deal with zoom out problem
	mViewTransform.UpdateTranslation(-center);
	mViewTransform.UpdateScale(factor);
	mViewTransform.UpdateTranslation(center);

	mModelTransform.UpdateTranslation(center);
	mModelTransform.UpdateScale(1 / factor);
	mModelTransform.UpdateTranslation(-center);
}

QPointF Camera::GetViewTransform(const QPointF& modelPoint) const
{
	return mViewTransform.Transform(modelPoint);
}

QPointF Camera::FromViewTransform(const QPointF& viewPoint) const
{
	return mModelTransform.Transform(viewPoint);
}

void Camera::ResetView()
{
	mViewTransform.ResetTransform();
}
