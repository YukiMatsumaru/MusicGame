#include "../pch.h"
#include "BackGround.h"

#include "WICTextureLoader.h"
#include "../System/DXTKManager.h"

#include "../Scene/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

// BackGroundクラスの初期化処理
void BackGround::Initialize()
{

	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// テクスチャのロード
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/BackEffect.png",
		nullptr, this->backEffectTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/back.png",
		nullptr, this->backGroundTexture_.GetAddressOf());

	// エフェクトの回転軸の初期化
	rotate_ = 0.0f;

	// エフェクトのRGBの初期化
	red_ = 1.0f;
	green_ = 1.0f;
	blue_ = 1.0f;

	// エフェクトの色変更用変数の初期化
	changeColor_ = 1.0f;

	// エフェクトの大きさ変更用変数の初期化
	scale_ = 1.0f;

	changeScale_ = 1.0f;

}

// BackGroundクラスの更新処理
void BackGround::Update()
{
	// エフェクトの回転軸を動かす
	rotate_ += 0.01f;

	// エフェクトの色を変更する
	changeColor_ += 0.05f;
	
	red_ = 1.0f;
	green_ = sinf(changeColor_ / 4.0f);
	blue_ = sinf(changeColor_ / 2.0f);

	// エフェクトの大きさを変更する
	changeScale_ += 0.05f;
	scale_ = sinf(changeScale_);

	// Scaleが0以下になったら
	if (scale_ <= 0)
	{
		// 強制的に大きさを1にする
		scale_ = 1.0f;
		changeScale_ = 1.0f;
	}
}

// BackGroundクラスの描画処理
void BackGround::Render()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// 背景の描画
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(backGroundTexture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	dxtk.m_spriteBatch->End();

	RECT rect = { 0,0,800,600 };

	// 背景エフェクト用テクスチャの描画
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(backEffectTexture_.Get(),
		Vector2(400.0f, 300.0f), &rect, Vector4(red_, green_, blue_, 0.5f), rotate_, Vector2(400, 300),Vector2(scale_));
	dxtk.m_spriteBatch->End();
}
