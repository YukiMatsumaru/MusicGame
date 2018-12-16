#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "../System/DXTKManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneBase* TitleScene::m_base = nullptr;

//TitleSceneクラスのインスタンスを取得する
SceneBase* TitleScene::GetInstance()
{
	if (m_base == nullptr)
		m_base = new TitleScene();
	return m_base;
}

//コンストラクタ
TitleScene::TitleScene()
{

}

//デストラクタ
TitleScene::~TitleScene()
{

}

//TitleSceneクラスの初期化関数
void TitleScene::Initialize()
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// エフェクトファクトリ生成
	factory_ = std::make_unique<EffectFactory>(dxtk.m_device);

	// テクスチャの読み込みパス
	factory_->SetDirectory(L"Resources");

	spriteBatch_ = std::make_unique<SpriteBatch>(dxtk.m_context);
	spriteFont_ = std::make_unique<SpriteFont>(dxtk.m_device, L"myfile.spritefont");

	// テクスチャのロード
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Fade.png",
		nullptr, this->texture_.GetAddressOf());

	// フェード用RGBAの初期化
	red_ = 0.0f;
	green_ = 0.0f;
	blue_ = 0.0f;
	alpha_ = 1.0f;

	// フェード用フラグの初期化
	fadeFlag_ = false;

	// フェード用カウントの初期化
	fadeCnt_ = 0;
}

//TitleSceneクラスの更新関数
void TitleScene::UpdateScene(SceneManager* scene)
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	dxtk.m_keyTracker->Update(dxtk.m_keyboard.get()->GetState());

	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::Space))
	{
		// フェードフラグを立てる
		fadeFlag_ = true;
	}

	// フェードフラグが立っていたら
	if (fadeFlag_)
	{
		fadeCnt_++;
		red_ += 0.01f;
		green_ += 0.01f;
		blue_ += 0.01f;
		alpha_ -= 0.001f;
	}

	if (fadeCnt_ >= FADE_TIME)
	{
		//PlaySceneに切り替わる
		scene->ChangeScene(PlayScene::GetInstance());
	}

}

//TitleSceneクラスの描画関数
void TitleScene::Render()
{

	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(texture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(red_, green_, blue_, alpha_));
	dxtk.m_spriteBatch->End();

	spriteBatch_->Begin();
	spriteFont_->DrawString(spriteBatch_.get(), L"MUSIC GAME", XMFLOAT2(315, 280));
	spriteFont_->DrawString(spriteBatch_.get(), L"PRESS SPACE KEY", XMFLOAT2(300, 320));
	spriteBatch_->End();
}

//TitleSceneオブジェクトを破棄する
void TitleScene::Dispose()
{
	if (m_base != nullptr)
	{
		delete m_base;
		m_base = nullptr;
	}
}
