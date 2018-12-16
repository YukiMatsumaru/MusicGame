#include "../pch.h"
#include "Lane.h"

#include "WICTextureLoader.h"
#include "../System/DXTKManager.h"

#include "../Scene/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

// Laneクラスの初期化処理
void Lane::Initialize()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// テクスチャのロード
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/background.png",
		nullptr, this->texture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Light.png",
		nullptr, this->lightTexture_.GetAddressOf());

	// レーンフラグの初期化
	laneFlag_ = false;

}

// Laneクラスの更新処理
void Lane::Update()
{
	for (int i = 0; i < lane_Num; i++)
	{
		if (isLaneKeyPressd_[i] == true)
		{
			laneLightTimer_[i] = max_Time;
			isLaneKeyPressd_[i] = false;
		}

		laneLightTimer_[i]--;

		if (laneLightTimer_[i] <= 0)
		{
			laneLightTimer_[i] = 0;
			textureScale_[i] = 1.0f;
		}
		else
		{
			textureScale_[i] = (float)laneLightTimer_[i] / max_Time;
		}

	}
}

// Laneクラスの描画処理
void Lane::Render()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	
	// レーンの描画
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(texture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 0.6f));
	dxtk.m_spriteBatch->End();
}

// レーンに光を描画する関数
void Lane::DrawLightColor()
{

	RECT rect = { 0,0,150,500 };

	for (int i = 0; i < lane_Num; i++)
	{
		float pos = NOTES_POSITION[i];

		if (laneLightTimer_[i] > 0)
		{
			DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
			dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
			dxtk.m_spriteBatch->Draw(lightTexture_.Get(),
				Vector2(pos - 50.0f + 75.0f,-5.0f + 500.0f), 
				&rect,
				Vector4(1.0f, 1.0f, 1.0f, 0.5f),
				0.0f,Vector2(75,500),Vector2(1.0f,textureScale_[i]));
			dxtk.m_spriteBatch->End();

		}

	}
	
}
