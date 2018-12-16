/// <summary>
/// SceneState.h
/// </summary>

#pragma once
#include "../StepTimer.h"
#include "../System/DXTKManager.h"

class SceneBase;

// フェード用時間
static const int FADE_TIME = 120;

class SceneState
{
private:
	SceneBase* m_base;

	// タイマー
	const DX::StepTimer& timer;

public:
	SceneState(const DX::StepTimer& timer);
	virtual ~SceneState();

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
	int Score = 0;

public:
	//仮想デストラクタ
	virtual ~SceneBase() {};

	virtual void Initialize() = 0;
	virtual void UpdateScene(SceneState* scene) = 0;
	virtual void Render() = 0;

	// スコアを取得する関数
	int GetScore() { return Score; }

	// スコアを設定する関数
	void SetScore(int score) { Score = score; }

};
