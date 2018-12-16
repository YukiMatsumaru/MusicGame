#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <SpriteFont.h>

#include "SceneManager.h"
#include "../System/Notes.h"
#include "../System/DXTKManager.h"

class TitleScene :public SceneBase
{
private:
	static SceneBase* m_base;

	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> factory_;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;

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

public:
	static SceneBase* GetInstance();
	 TitleScene();
	~TitleScene();

	//初期化
	void Initialize() override;

	//更新処理
	void UpdateScene(SceneManager* scene) override;

	//描画処理
	void Render() override;

	//オブジェクトを破棄する
	void Dispose() override;

};

