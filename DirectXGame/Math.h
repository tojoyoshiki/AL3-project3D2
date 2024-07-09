#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

class Math {
public:
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
	static Matrix4x4 MatrixMultiply(const Matrix4x4& m1, const Matrix4x4& m2);
};
