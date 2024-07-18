#include "GameScene.h"
#include "CameraController.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelBlocks_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete modelBlocks_;
	delete modelSkydome_;
	delete player_;
	delete skydome_;
	delete mapChipField_;
	delete cameraController_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	modelBlocks_ = Model::CreateFromOBJ("block", true);
	viewProjection_.Initialize();
	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの生成(モデル)
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);
	//  3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);
	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	GenerateBlocks();
	// カメラコントローラの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize(&viewProjection_, movableArea);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	/*
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);
	*/
}

void GameScene::Update() {
	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の作成
			worldTransformBlock->UpdateMatrix();
		}
	}
	// デバックカメラの更新
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}
#endif
	// カメラの処理

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
	// 天球の更新
	skydome_->Update();
	// 自キャラの更新
	player_->Update();
	// カメラコントローラの更新
	cameraController_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlocks_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	// 天球の描画
	skydome_->Draw();
	// 自キャラの描画
	player_->Draw();
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
void GameScene::GenerateBlocks() {
	{
		// 要素数
		uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
		uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
		// 要素数を変更する
		// 列数を設定(縦方向のブロック数)
		worldTransformBlocks_.resize(20);
		for (uint32_t i = 0; i < 20; ++i) {
			// 一列の要素数を設定(横ブロック数)
			worldTransformBlocks_[i].resize(numBlockHorizontal);
		}
		// ブロックの生成
		for (uint32_t i = 0; i < numBlockVirtical; ++i) {
			for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
				if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
					WorldTransform* worldTransform = new WorldTransform();
					worldTransform->Initialize();
					worldTransformBlocks_[i][j] = worldTransform;
					worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				}
			}
		}
	}
}