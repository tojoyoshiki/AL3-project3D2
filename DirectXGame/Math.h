#pragma once
#include <GameScene.h>
#include <Matrix4x4.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

// アフィン変換行列作成
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
