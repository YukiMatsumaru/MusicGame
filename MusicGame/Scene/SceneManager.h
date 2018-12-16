/// <summary>
/// SceneManager.h
/// </summary>

#pragma once
#include "../StepTimer.h"
#include "../System/DXTKManager.h"

class SceneBase;

// フェード用時間
static const int FADE_TIME = 120;

// スコアの桁数
static const int SCORE_NUM = 6;

// 全数値
static const int ALL_NUM = 10;

// 画像座標の初期値
static const int TEXTURE_POSITION = 100;

// BGMの時間 84.0f
static const float BGM_TIME = 84.0f;

// SEのボリューム
static const float SE_VOLUME = 0.3f;

// 判定終了座標
static const float JUDGE_END_POSITION = 510.0f;

class SceneManager
{
private:
	SceneBase* m_base;

public:
	SceneManager();
	virtual ~SceneManager();

	//シーン変更
	void ChangeScene(SceneBase* nextScene);

	//更新関数
	void Update();

	//描画関数
	void Render();

};

class SceneBase
{
private:
	// スコア管理変数
	int score = 0;

public:
	//仮想デストラクタ
	virtual ~SceneBase() {};

	virtual void Initialize() = 0;
	virtual void UpdateScene(SceneManager* scene) = 0;
	virtual void Render() = 0;

	// オブジェクトを破棄する
	virtual void Dispose() = 0;

	// スコアを取得する関数
	int GetScore() { return score; }

	// スコアを設定する関数
	void SetScore(int score) { this->score = score; }

};
