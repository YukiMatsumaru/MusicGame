#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>

#include "../Scene/SceneManager.h"

enum judge
{
	PERFECT,
	GREAT,
	GOOD,
	MISS,
	BLANK,

	judgeNum
};

// レーンの基準値
const float NOTES_POSITION[] = { 150.0f,300.0f,450.0f,600.0f };

// 得点配列
const int SCORE_ARRAY[] = { 1000,100,10,0 };


class Notes
{
protected:
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;

	// ノーツのテクスチャハンドル
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> NotesTexture_;

	// ロングノーツのテクスチャハンドル
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> longNotesTexture_;

	// シーン情報
	SceneBase* m_base;

	//ノーツのポジション
	DirectX::SimpleMath::Vector2 notesPos_;

	// レーンの数
	int lanesNum_;

	// ノーツの個数
	int notesNum_;

	// 音符が落ちてくるスピード
	float fallSpeed_;

	// 判定範囲内に入る前にキーを押しているか
	bool isPushBefore_;
	bool isPushBeforeOld_;
	// 判定可能か
	bool canJudge_;

public:
	Notes(int lanenumber,int notestype);
	virtual ~Notes();

	// Notesクラスの初期化関数
	static void Initialize();

	// Notesクラスの更新関数
	virtual void Update();

	// Notesクラスの描画関数
	void Render();

	// ノーツ判定関数
	judge Notesjudge(SceneBase* scene);

	// ノーツを降らせる関数
	void FallNotes();

	// ノーツの座標を取得する
	DirectX::SimpleMath::Vector2 GetNotesPosition()  { return notesPos_; }

};