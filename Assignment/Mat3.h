#pragma once

#include <QtCore>

struct Vec3 {
	qreal x1 = 0;
	qreal x2 = 0;
	qreal x3 = 0;
};

class Mat3 {
public:
	Mat3() = default;
	Mat3(const Vec3& row1, const Vec3& row2, const Vec3& row3);
	void MultiplyVec3(Vec3& v) const;
	void MultiplyMat3(Mat3& m) const;

	Vec3 GetRow1() const { return mRow1; }
	Vec3 GetRow2() const { return mRow2; }
	Vec3 GetRow3() const { return mRow3; }

	void SetRow1(const Vec3& v) { mRow1 = v; }
	void SetRow2(const Vec3& v) { mRow2 = v; }
	void SetRow3(const Vec3& v) { mRow3 = v; }

	void Transpose() {
		Vec3 r1{ mRow1.x1, mRow2.x1, mRow3.x1 };
		Vec3 r2{ mRow1.x2, mRow2.x2, mRow3.x2 };
		Vec3 r3{ mRow1.x3, mRow2.x3, mRow3.x3 };

		mRow1 = r1;
		mRow2 = r2;
		mRow3 = r3;
	}

private:
	Vec3 mRow1;
	Vec3 mRow2;
	Vec3 mRow3;
};
