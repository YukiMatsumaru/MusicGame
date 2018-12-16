#include <fstream>
#include <sstream>

#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

#include "../System/DXTKManager.h"
#include "../Sound/ADX2Le.h"
#include "../Sound/CueSheet_0.h"

#pragma comment(lib, "cri_ware_pcx86_LE_import.lib")

#include "PlayScene.h"
#include "ResultScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

using namespace std;

// キー判定用リスト
const unordered_map<int, Keyboard::Keys> PlayScene::checkKeyList_ =
{
	{ 0, Keyboard::Keys::D },
	{ 1, Keyboard::Keys::F },
	{ 2, Keyboard::Keys::J },
	{ 3, Keyboard::Keys::K }
};

SceneBase* PlayScene::m_base = nullptr;

SceneBase* PlayScene::GetInstance()
{
	if (m_base == nullptr)
		m_base = new PlayScene();
	return m_base;
}

// 初期化処理
void PlayScene::Initialize()
{

	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	spriteBatch_ = std::make_unique<SpriteBatch>(dxtk.m_context);
	spriteFont_ = std::make_unique<SpriteFont>(dxtk.m_device, L"myfile.spritefont");

	// キーボードの初期化
	keyboard_ = dxtk.m_keyboard.get();

	// エフェクトファクトリ生成
	factory_ = std::make_unique<EffectFactory>(dxtk.m_device);

	// テクスチャの読み込みパス
	factory_->SetDirectory(L"Resources");

	// サウンドの初期化
	ADX2Le::Initialize("Resources/SoundData/Sound_data.acf");
	ADX2Le::LoadAcb("Resources/SoundData/CueSheet_0.acb", "Resources/SoundData/CueSheet_0.awb");


	// NotesManagerクラスの初期化
	notesManager_ = new NotesManager();
	notesManager_->Initialize();

	// Laneクラスの初期化
	lane_ = new Lane();
	lane_->Initialize();

	// BackGroundクラスの初期化
	background_ = new BackGround();
	background_->Initialize();

	// 判定の初期化
	nowJudge_ = judge::BLANK;

	// カウントの初期化
	fallCnt_ = 0;
	displayCnt_ = 0;

	// タイマーの初期化
	gametimer_ = 0;
	bgmtimer_ = 0;

	// フェード用フラグの初期化
	fadeFlag_ = true;

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

	// スコア画像の切り出し座標を初期化
	for (int i = 0; i < ALL_NUM; i++)
	{
		int offsetX = x + (w * i);
		rect[i] = { offsetX, y, offsetX + w, y + h };
	}

	// スコア変数の初期化
	score_ = 0;

	// レーン番号を記録する変数の初期化
	saveLaneNum_ = 0;

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Fade.png",
		nullptr, this->fadeTexture_.GetAddressOf());
	
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/number.png",
		nullptr, this->scoreTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Perfect.png",
		nullptr, this->perfectTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Great.png",
		nullptr, this->greatTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Good.png",
		nullptr, this->goodTexture_.GetAddressOf());

	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Miss.png",
		nullptr, this->missTexture_.GetAddressOf());
	
	CreateWICTextureFromFile(
		dxtk.m_device, L"Resources/Star.png",
		nullptr, this->effectTexture_.GetAddressOf());

	// CSVの読み込み
	Load_CSV();

	// 音楽の再生
	ADX2Le::Play(CRI_CUESHEET_0_MUSIC1);

}

// 更新処理
void PlayScene::UpdateScene(SceneManager* scene)
{
	
	// NotesManagerクラスの更新
	notesManager_->Update();

	auto kb = keyboard_->GetState();

	for (int i = 0; i < 4; i++)
	{
		if (kb.IsKeyDown(checkKeyList_.at(i)))
		{
			lane_->SetKeyPressd(i);
		}
	}

	// Laneクラスの更新
	lane_->Update();

	// BackGroundクラスの更新
	background_->Update();

	// 当たり判定を行う
	Notesjudge();

	// スタークラスを更新
	for(std::list<Star>::iterator itr =effectStar_.begin(); itr !=effectStar_.end(); itr++)
	{
		bool result = false;
		result = itr->Update();

		if (!result)
		{
			itr = effectStar_.erase(itr);

			if (itr == effectStar_.end())
			{
				break;
			}
		}

	}

	// フェードフラグがおりていたら
	if (fadeFlag_)
	{
		fadeCnt_++;

		// Colorを白から元の色へ戻していく
		red_ -= 0.01f;
		green_ -= 0.01f;
		blue_ -= 0.01f;
		alpha_ -= 0.01f;
	}

	// フェード時間が過ぎている、かつアルファ値が0.9になっていたら
	if ((fadeCnt_ >= FADE_TIME) && (alpha_ >= 0.9f))
	{
		// フェードフラグをおろす
		fadeFlag_ = false;
	}

	if (gametimer_ >= BGM_TIME)
	{
		// Colorを段々白にしていく
		red_ -= 0.01f;
		green_ -= 0.01f;
		blue_ -= 0.01f;
		alpha_ -= 0.01f;

		// ResultSceneに切り替わる
		scene->ChangeScene(ResultScene::GetInstance());

		// 音楽の停止
		ADX2Le::Stop();

	}

	gametimer_ += 1.0f / 60.0f;

}

// 描画処理
void PlayScene::Render()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// BackGroundクラスの描画
	background_->Render();

	// Laneクラスの描画
	lane_->Render();

	// レーンに光を描画する
	lane_->DrawLightColor();

	// スターの描画
	for (std::list<Star>::iterator itr = effectStar_.begin(); itr != effectStar_.end(); itr++)
	{
		itr->Render();
	}

	// フェード用テクスチャの描画
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(fadeTexture_.Get(),
		Vector2(0.0f, 0.0f), Vector4(red_, green_, blue_, alpha_));
	dxtk.m_spriteBatch->End();

	// 判定のテクスチャを描画
	DrawJudgeTexture();

	// スコアを描画
	DrawScore();

	// NotesManagerクラスの描画
	notesManager_->Render();

}

// PlaySceneクラスのオブジェクトを破棄する
void PlayScene::Dispose()
{
	if (m_base != nullptr)
	{
		delete m_base;
		m_base = nullptr;
	}
}

// CSVデータを読み込む
void PlayScene::Load_CSV()
{
	ifstream ifs("Resources/MusicData/MusicData.csv");

	string line;

	// 音符データを読み込む
	while (getline(ifs, line))
	{

		// カンマを空白に変換
		std::replace(line.begin(), line.end(),',',' ');

		// 1行分を文字列ストリームに変換
		istringstream stream(line);

		MusicData LineNumber;

		// レーン番号を渡す
		stream >> LineNumber.LaneNumber;

		// 音符が落ちてくる時間を渡す
		stream >> LineNumber.FallTime;

		// ノーツの種類を渡す
		stream >> LineNumber.NotesNumber;

		// 配列にレーン番号と音符の落ちてくる時間を格納する
		musicdata_.push_back(LineNumber);

	}
}

// スコアを計算して描画する関数
void PlayScene::DrawScore()
{
	// 桁数 (最大桁数)
	int division_num = 1e+5;

	// 上の桁から描画する
	for (int i = SCORE_NUM - 1; i >= 0; i--)
	{
		// division_numが示す桁の値を取り出す
		int num = score_  / division_num;

		// 数値の描画
		DrawNum(i, num);

		// スコアの描画した部分を取り除く
		if (num * division_num > 0)
			score_ %= num * division_num;
				
		// 桁を一つ減らす
		division_num /= 10;
	}

}

// 判定のテクスチャを描画する関数
void PlayScene::DrawJudgeTexture()
{
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// 判定がMISSの場合
	if ((notesManager_->GetNotesPosition() >= JUDGE_END_POSITION)||  (displayJudge_ == judge::MISS))
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(missTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();

	}

	// 判定がGOODの場合
	if (displayJudge_ == judge::GOOD)
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(goodTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();

	}

	// 判定がGREATの場合
	if (displayJudge_ == judge::GREAT) 
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(greatTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();

	}

	// 判定がPERFECTの場合
	if (displayJudge_ == judge::PERFECT)
	{
		dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
		dxtk.m_spriteBatch->Draw(perfectTexture_.Get(),
			Vector2(280.0f, 200.0f), Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		dxtk.m_spriteBatch->End();
	}

	// 判定がBLANK以外なら
	if (nowJudge_ != judge::BLANK)
	{
		displayCnt_ = 0;

	}

	// 表示時間が終了したら
	if (displayCnt_ == 0)
	{
		// 判定表示の一時保持
		displayJudge_ = nowJudge_;


	}

	// 表示カウントが2秒経過したら
	if (displayCnt_ >= DISPLAY_TIME)
	{
		// 表示変数をBLANKにする
		displayJudge_ = judge::BLANK;
	}

	// カウントを進める
	displayCnt_++;

}

// スコアの値一つ分を描画する関数
void PlayScene::DrawNum(int digit, int num)
{

	// 画像のずれ幅
	Vector2 offset(-50.0f, 0.0f);
	Vector2 anchorPoint(730.0f, 0.0f);

	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());
	dxtk.m_spriteBatch->Draw(scoreTexture_.Get(),
		anchorPoint + (offset * digit),
		&rect[num],
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, Vector2(0.0f, 0.0f), 0.7f);
	dxtk.m_spriteBatch->End();
}

// ノーツの当たり判定を行う
void PlayScene::Notesjudge()
{

	auto kb = keyboard_->GetState();

	// ノーツが何番目かを取得する
	int notesNums = 0;
	notesNums = notesManager_->GetNotesNum();

	if (notesNums > 0)
	{	

		// レーン番号を取得する
		int laneNumber = musicdata_[notesNums - 1].LaneNumber;

		// ノーツの種類を取得する
		int notesType = musicdata_[notesNums - 1].NotesNumber;

		// 判定を初期化する
		nowJudge_ = BLANK;

		// キーの判定
		if (kb.IsKeyDown(checkKeyList_.at(laneNumber)))
		{
			// 判定を行う
			nowJudge_ = notesManager_->PlessButton_Check(laneNumber, notesType);

			if (nowJudge_ != BLANK || nowJudge_ != MISS)
			{
				// スタークラスを格納
				Star star = Star();
				star.Initialize(effectTexture_.Get(),laneNumber,nowJudge_);
				effectStar_.push_back(star);
			}

			// レーン番号を記録する
			saveLaneNum_ = laneNumber;

			// レーンフラグを立てる
			lane_->SetLaneFlag(true);

		}
		else
			// レーンフラグを降ろす
			lane_->SetLaneFlag(false);

			// スコアを取得する
			score_ = m_base->GetScore();		

	}
}