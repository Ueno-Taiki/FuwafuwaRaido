#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"

class Player;

class PlayerBullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	const float GetPositon() const { return worldTransform_.translation_.x; }

	//ワールド座標を取得
	Vector3 GetWorldPosition() const;

	//AABBを取得
	AABB GetAABB() const;

	//衝突判定
	void OnCollision(const Player* player);

	//弾
	std::list<PlayerBullet*> bullets_;
	static const int32_t kLifeTime = 60 * 2;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	bool IsDead() const { return isDead_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//弾の当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};
