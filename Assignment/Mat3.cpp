#include "Mat3.h"

inline static qreal Dot(const Vec3& v1, const Vec3& v2)
{
	return v1.x1 * v2.x1 + v1.x2 * v2.x2 + v1.x3 * v2.x3;
}

Mat3::Mat3(const Vec3& row1, const Vec3& row2, const Vec3& row3) : mRow1(row1), mRow2(row2), mRow3(row3)
{
}

void Mat3::MultiplyVec3(Vec3& v) const
{
	v = Vec3{ Dot(mRow1, v), Dot(mRow2, v), Dot(mRow3, v) };
}

void Mat3::MultiplyMat3(Mat3& m) const
{
	m.Transpose();
	Vec3 r1{ Dot(mRow1, m.GetRow1()), Dot(mRow1, m.GetRow2()), Dot(mRow1, m.GetRow3()) };
	Vec3 r2{ Dot(mRow2, m.GetRow1()), Dot(mRow2, m.GetRow2()), Dot(mRow2, m.GetRow3()) };
	Vec3 r3{ Dot(mRow3, m.GetRow1()), Dot(mRow3, m.GetRow2()), Dot(mRow3, m.GetRow3()) };

	m.SetRow1(r1);
	m.SetRow2(r2);
	m.SetRow3(r3);
}