#pragma once

#include <QtCore>
#include <cmath>

#include "Mat3.h"

class TransformMat3
{
public:
	TransformMat3();

	void UpdateRotation(double rotateAngle);
	void UpdateTranslation(const QPointF& translate);
	void UpdateScale(double factor);

	QPointF Transform(const QPointF& point) const;

	void ResetTransform();
	qreal GetScaleFactor() const
	{
		return scaleFactor;
	}

private:
	Mat3 mMatrix;
	qreal scaleFactor = 1;
};
