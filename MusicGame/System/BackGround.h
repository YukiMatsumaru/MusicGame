#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>


class BackGround
{
private:
	// backeffect用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backEffectTexture_;

	// 背景用のテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backGroundTexture_;

	// エフェクトの回転軸
	float rotate_;

	// エフェクトのRGBカラー
	float red_;
	float green_;
	float blue_;

	// エフェクトの色変更用変数
	float changeColor_;

	// エフェクトの大きさ変更用の変数
	float changeScale_;
	float scale_;

public:
	// BackGroundクラスの初期化関数
	void Initialize();

	// BackGroundクラスの更新関数
	virtual void Update();

	// BackGroundクラスの描画関数
	void Render();

};