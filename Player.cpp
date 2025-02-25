#include <cassert>
#include "Player.h"
#include "Input.h"
#include "MathUtilityForText.h"

void Player::Initialize(Model* model1, Model* model2, Model* model3, Model* model4, ViewProjection* viewProjection, const Vector3& position) {
	//NULLポインタチャック
	assert(model1);

	//引数として受け取ったデータをメンバ変数に記録
	model1_ = model1;
	model2_ = model2;
	model3_ = model3;
	model4_ = model4;
	viewProjection_ = viewProjection;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Player::Update()
{
	//移動入力
	InputMove();

	//プレイヤー攻撃
	Attack();

	//弾更新
	if (bullet_) {
		bullet_->Update();
	}
	if (isGetItem_ == true) {
		SubAttack();
		if (subBullet_) {
			subBullet_->Update();
		}
	}

	//移動範囲
	MoveRange();

	//ダメージ
	Damage();
	
	//行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw(){
	//3Dモデル描画
	if (playerLife == 2) {
		model1_->Draw(worldTransform_, *viewProjection_);
	} else if (playerLife == 1) {
		model2_->Draw(worldTransform_, *viewProjection_);
	} else {
		model3_->Draw(worldTransform_, *viewProjection_);
	}

	//弾描画
	if (bullet_) {
		bullet_->Draw(*viewProjection_);
	}
	if (isGetItem_ == true) {
		model4_->Draw(worldTransform_, *viewProjection_);
		if (subBullet_) {
			subBullet_->Draw(*viewProjection_);
		}
	}
}

//自動移動
void Player::InputMove() {
	//上に上昇
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		velocity_ = {0.0f, 0.2f, 0.0f};
	} else {
		velocity_ = {0.0f, -0.2f, 0.0f};
	}
	worldTransform_.translation_ += velocity_;
}

//攻撃
void Player::Attack() { 
	if (!isBulletShot_) {
		isBulletShot_ = true;
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;
	}
	if (isBulletShot_) {
		if (bullet_->GetPositon() >= 35) {
			isBulletShot_ = false;
		}
	}
}

//複数弾
void Player::SubAttack() {
	if (!isSubBulletShot_) {
		isSubBulletShot_ = true;
		// 弾を生成し、初期化
		SubBullet* newSubBullet = new SubBullet();
		newSubBullet->Initialize(model1_, worldTransform_.translation_);

		// 弾を登録する
		subBullet_ = newSubBullet;
	}
	if (isSubBulletShot_) {
		if (subBullet_->GetPositon() >= 35) {
			isSubBulletShot_ = false;
		}
	}
}

//ダメージ
void Player::Damage() {
	if (Input::GetInstance()->TriggerKey(DIK_D)) {
		playerLife = (playerLife - damage);
	}
	if (Input::GetInstance()->TriggerKey(DIK_H)) {
		playerLife = (playerLife + damage);
	}

	if (playerLife < 0) {
		playerLife = 0;
	}
	if (playerLife > 2) {
		playerLife = 2;
	}
	if (Input::GetInstance()->TriggerKey(DIK_I)) {
		isGetItem_ = true;
	}
}

//移動範囲
void Player::MoveRange() { 
	if (worldTransform_.translation_.y <= -15) {
		worldTransform_.translation_.y = -15;
	}
	if (worldTransform_.translation_.y >= 15) {
		worldTransform_.translation_.y = 15;
	}
}

//ワールド座標を取得
Vector3 Player::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos = {};
	//ワールド行列の平行移動成文を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

//AABBを取得
AABB Player::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();

	AABB aabb = {};

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb; 
}

//衝突応用
void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// 当たったら死ぬ
	isDead_ = true;
}
