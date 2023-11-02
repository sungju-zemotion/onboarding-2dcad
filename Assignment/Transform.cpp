#include "Transform.h"

TransformMat3::TransformMat3() : mMatrix(Vec3{ 1, 0, 0 }, Vec3{ 0, 1, 0 }, Vec3{ 0, 0, 1 })
{
}

void TransformMat3::UpdateRotation(double rotateAngle)
{
	Mat3 rotation(Vec3{ cos(rotateAngle), -sin(rotateAngle), 0 }, Vec3{ sin(rotateAngle), cos(rotateAngle), 0 }, Vec3{ 0, 0, 1 });

	rotation.MultiplyMat3(this->mMatrix);
}

void TransformMat3::UpdateTranslation(const QPointF& translate)
{
	Mat3 translation(Vec3{ 1, 0, translate.x() }, Vec3{ 0, 1, translate.y() }, Vec3{ 0, 0, 1 });

	translation.MultiplyMat3(mMatrix);
}

void TransformMat3::UpdateScale(double factor)
{
	Mat3 scale(Vec3{ factor, 0, 0 }, Vec3{ 0, factor, 0 }, Vec3{ 0, 0, 1 });

	scale.MultiplyMat3(mMatrix);
	scaleFactor *= factor;
}

QPointF TransformMat3::Transform(const QPointF& point) const
{
	Vec3 v({ point.x(), point.y(), 1 });
	mMatrix.MultiplyVec3(v);

	return { v.x1, v.x2 };
}

void TransformMat3::ResetTransform()
{
	// set as the identity matrix
	mMatrix = Mat3(Vec3{ 1, 0, 0 }, Vec3{ 0, 1, 0 }, Vec3{ 0, 0, 1 });
}
