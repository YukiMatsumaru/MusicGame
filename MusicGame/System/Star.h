#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>

#include "../System/Notes.h"

class Star
{
private:
	// Star用テクスチャハンドル
	ID3D11ShaderResourceView* starTexture_;

	// StarのRGBAカラー
	float red_;
	float green_;
	float blue_;
	float alpha_;

	// Starの大きさ
	float starScale_;

	// レーンの座標
	float pos_;

	// レーンの数
	const int lane_Num = 4;

public:
	Star(){};

	~Star() {};

	// Starクラスの初期化関数
	void Initialize(ID3D11ShaderResourceView* starTexture,int laneNum,judge judge);

	// Starクラスの更新関数
	bool Update();

	// Starクラスの描画関数
	void Render();
};