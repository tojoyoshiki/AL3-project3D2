#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	static inline const float kWalkSpeed = 0.03f;
	static inline const float kWalkMotionAngleStart = 5.0f;
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = -5.0f;
	// アニメーションの周期時間
	static inline const float kWalklMotionTime = 1.0f;
	// 経過時間
	float walkTimer_ = 0.0f;

	// 速度
	Vector3 velocity_ = {};
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
};
