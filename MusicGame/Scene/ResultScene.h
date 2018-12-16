#pragma once

#include <CommonStates.h>
#include <Effects.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include <SpriteFont.h>

#include "SceneManager.h"
#include "ResultScene.h"

class ResultScene :public SceneBase
{
private:
	static SceneBase* m_base;

	// キーボード
	DirectX::Keyboard* keyboard_;
	
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> factory_;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;

	// Score用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scoreTexture_;

	// フォント
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;


	// フェード用RGBA
	float red_;
	float green_;
	float blue_;
	float alpha_;

	// フェード用フラグ
	bool fadeFlag_;

	// フェード用カウント
	int fadeCnt_;

	// スコア
	int score_;

	// 画像用変数
	int x, y, w, h;

	// スコア用変数
	RECT rect[ALL_NUM];

public:
	static SceneBase* GetInstance();

	ResultScene();
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新処理
	void UpdateScene(SceneManager* scene) override;

	// 描画処理
	void Render() override;

	// スコアを計算して描画する関数
	void  DrawScore();

	// スコアの値一つ分を描画する関数
	void DrawNum(int digit, int num);

	//オブジェクトを破棄する
	void Dispose() override;

};