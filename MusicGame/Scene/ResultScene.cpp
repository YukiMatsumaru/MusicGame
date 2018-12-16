#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

#include "../System/DXTKManager.h"

#include "ResultScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

SceneBase* ResultScene::m_base = nullptr;

SceneBase * ResultScene::GetInstance()
{
	if (m_base == nullptr)
		m_base = new ResultScene();
	return m_base;
}

// コンストラクタ
ResultScene::ResultScene()
{
	
}

// デストラクタ
ResultScene::~ResultScene()
{

}

// 初期化処理
void ResultScene::Initialize()
{
	// キーボードの初期化
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	keyboard_ = dxtk.m_keyboard.get();

	// エフェクトファクトリ生成
	factory_ = std::make_unique<EffectFactory>(dxtk.m_device);

	// テクスチャの読み込みパス
	factory_->SetDirectory(L"Resources");

	spriteBatch_ = std::make_unique<SpriteBatch>(dxtk.m_context);
	spriteFont_ = std::make_unique<SpriteFont>(dxtk.m_device, L"myfile.spritefont");

	// フェード用フラグの初期化
	fadeFlag_ = false;

	// フェード用カウントの初期化
	fadeCnt_ = 0;

	// フェード用RGBAの初期化
	red_ = 1.0f;
	green_ = 1.0f;
	blue_ = 1.0f;
	alpha_ = 1.0f;

	// 画像用変数の初期化
	x = 0;
	y = 0;
	w = TEXTURE_POSITION;
	h = TEXTURE_POSITION;

	// スコアの初期化
	int score = 0;

	// スコア画像の切り出し座標を初期化
	for (int i = 0; i < ALL_NUM; i++)
	{
		int offsetX = x + (w * i);
		rect[i] = { offsetX, y, offsetX + w, y + h };
	}

	// テクスチャのロード
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Fade.png",
		nullptr, this->texture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/number.png",
		nullptr, this->scoreTexture_.GetAddressOf());


}

// 更新処理
void ResultScene::UpdateScene(SceneManager* scene)
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	dxtk.m_keyTracker->Update(dxtk.m_keyboard.get()->GetState());

	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::K))
	{
		// スコアの初期化
		m_base->SetScore(0);

		// TitleSceneに切り替わる
		scene->ChangeScene(TitleScene::GetInstance());
	}

	if (dxtk.m_keyTracker->IsKeyPressed(Keyboard::Keys::D))
	{
		// スコアの初期化
		m_base->SetScore(0);

		// PlaySceneに切り替わる
		scene->ChangeScene(PlayScene::GetInstance());

	}

	// フェードフラグがおりていたら
	if (!fadeFlag_)
	{
		fadeCnt_++;
		red_ -= 0.01f;
		green_ -= 0.01f;
		blue_ -= 0.01f;
		alpha_ += 0.01f;
	}
	
	if (fadeCnt_ >= FADE_TIME)
	{
		fadeFlag_ = true;
	}

}

// 描画処理
void ResultScene::Render()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(texture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(red_, green_, blue_, alpha_));
	dxtk.m_spriteBatch->End();

	// スコアの描画
	DrawScore();

	spriteBatch_->Begin();
	spriteFont_->DrawString(spriteBatch_.get(), L"D:PLAY AGAIN", XMFLOAT2(100, 400));
	spriteFont_->DrawString(spriteBatch_.get(), L"K:REUTURN TO TITLE", XMFLOAT2(500, 400));
	spriteFont_->DrawString(spriteBatch_.get(), L"PRESS SPACE KEY", XMFLOAT2(300, 320));
	spriteBatch_->End();

}

// スコアを計算して描画する関数
void ResultScene::DrawScore()
{
	// 桁数 (最大桁数)
	int division_num = 1e+5;

	// スコアの取得
	score_ = m_base->GetScore();

	// 上の桁から描画する
	for (int i = SCORE_NUM - 1; i >= 0; i--)
	{
		// division_numが示す桁の値を取り出す
		int num = score_ / division_num;

		// 数値の描画
		DrawNum(i, num);

		// スコアの描画した部分を取り除く
		if (num * division_num > 0)
			score_ %= num * division_num;

		// 桁を一つ減らす
		division_num /= 10;
	}

}

// スコアの値一つ分を描画する関数
void ResultScene::DrawNum(int digit, int num)
{

	// 画像のずれ幅
	Vector2 offset(-50.0f, 0.0f);
	Vector2 anchorPoint(490.0f, 250.0f);

	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(scoreTexture_.Get(),
		anchorPoint + (offset * digit),
		&rect[num],
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, Vector2(0.0f, 0.0f), 0.7f);
	dxtk.m_spriteBatch->End();
}

// ResultSceneクラスのオブジェクトを破棄する
void ResultScene::Dispose()
{
	if (m_base != nullptr)
	{
		delete m_base;
		m_base = nullptr;
	}
}
