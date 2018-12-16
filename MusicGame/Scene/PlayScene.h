#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include <SpriteFont.h>
#include <unordered_map>


#include "../System/NotesManager.h"
#include "../System/Lane.h"
#include "../System/Star.h"
#include "../System/BackGround.h"
#include "../Resources/MusicData/MusicData.h"

class PlayScene :public SceneBase
{
private:
	static SceneBase* m_base;

	// キー判定用リスト
	static const std::unordered_map<int, DirectX::Keyboard::Keys> checkKeyList_;

	// キーボード
	DirectX::Keyboard* keyboard_;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;

	// フォント
	std::unique_ptr<DirectX::SpriteFont> spriteFont_;

	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> factory_;

	// Fade用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> fadeTexture_;

	// Score用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> scoreTexture_;

	// effect用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> effectTexture_;

	// 判定表示用テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> perfectTexture_;	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> greatTexture_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> goodTexture_;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> missTexture_;

	// 楽譜管理用変数
	std::vector<MusicData> musicdata_;

	// ノーツマネージャー
	NotesManager* notesManager_;

	// レーンクラス
	Lane* lane_;

	// スタークラス
	std::list<Star> effectStar_;

	// 背景クラス
	BackGround* background_;

	// 判定用変数
	judge nowJudge_;

	//判定表示用変数
	judge displayJudge_;

	// スコア用変数
	RECT rect[ALL_NUM];

	int outputWidth_;
	int outputHeight_;

	// 判定表示を消すためのカウント変数
	int displayCnt_;

	// ノーツを降らせるためのカウント変数
	int fallCnt_;

	// 画像用変数
	int x, y, w, h;

	// フェード用カウント
	int fadeCnt_;

	// スコア用変数
	int score_;

	// レーン番号を記録する変数
	int saveLaneNum_;

	// 判定表示タイマー
	const int DISPLAY_TIME = 30;

	// ＢＧＭタイマー
	float bgmtimer_;

	// ゲームタイマー
	float gametimer_;

	// フェード用RGBA
	float red_;
	float green_;
	float blue_;
	float alpha_;

	// フェード用フラグ
	bool fadeFlag_;

private:
	// CSVデータを読み込む
	void Load_CSV();

	// スコアを計算して描画する関数
	void  DrawScore();

	// 判定のテクスチャを描画する関数
	void  DrawJudgeTexture();

	// スコアの値一つ分を描画する関数
	void DrawNum(int digit, int num);

	// 当たり判定を行う
	void Notesjudge();

public:
	static SceneBase* GetInstance();
	PlayScene() {};
	~PlayScene() {};

	// 初期化
	void Initialize() override;

	// 更新処理
	void UpdateScene(SceneManager* scene) override;

	// 描画処理
	void Render() override;

	// オブジェクトを破棄する
	void Dispose() override;

	// ノーツの配列データを取得する
	const std::vector<MusicData>& GetNotesData() { return musicdata_; }

	// タイマーを取得する
	float GetTimer() { return gametimer_; }

};
