#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>


class Lane
{
private:
	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;

	// Light用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> lightTexture_;

	// レーン情報
	bool isLaneKeyPressd_[4];

	// レーン用のタイマー
	int  laneLightTimer_[4];

	// 光画像のサイズ
	float textureScale_[4];

	// エフェクト用のタイマー
	int effectTimer_[4];

	// エフェクト画像のサイズ
	float effectScale_[4];

	// レーンフラグ
	bool laneFlag_;

public:
	Lane()
		:laneLightTimer_{0}
		,textureScale_{0}
		,effectTimer_{0}
	{};
	~Lane() {};

	// レーンの数
	const int lane_Num = 4;

	// 最大時間
	const int max_Time = 10;

	// Laneクラスの初期化関数
	void Initialize();

	// Laneクラスの更新関数
	virtual void Update();

	// Laneクラスの描画関数
	void Render();

	// レーンに光を描画する関数
	void DrawLightColor();

	// レーンフラグを設定する関数
	void SetLaneFlag(bool flag) { laneFlag_ = flag; }

	// キー情報を設定する関数
	void SetKeyPressd(int keys) { isLaneKeyPressd_[keys] = true; };

};