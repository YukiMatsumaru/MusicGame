#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

#include "../System/DXTKManager.h"

#include "Notes.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// ノーツのテクスチャハンドル
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Notes::NotesTexture_;

// ロングノーツのテクスチャハンドル
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Notes::longNotesTexture_;

// コンストラクタ
Notes::Notes(int lanenumber,int notestype)
{	
	// ノーツの初期化
	notesPos_ = Vector2(NOTES_POSITION[lanenumber], 80.0f);

	// レーン数の初期化・代入
	lanesNum_ = 0;
	lanesNum_ = lanenumber;

	// ノーツの種類数の初期化・代入
	notesNum_ = 0;
	notesNum_ = notestype;
	
	fallSpeed_ = 5.0f;

	isPushBeforeOld_ = false;
	isPushBefore_ = false;
	canJudge_ = false;

}

// デストラクタ
Notes::~Notes()
{

}

// Notesクラスの初期化処理
void Notes::Initialize()
{
	
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// ノーツテクスチャのロード
	CreateWICTextureFromFile(
	dxtk.m_device, L"Resources/Notes.png",
	nullptr, NotesTexture_.GetAddressOf());

}

// Notesクラスの更新関数
void Notes::Update()
{
	// ノーツを降らせるための関数
	FallNotes();

	// 状態の一時保持
	isPushBeforeOld_ = isPushBefore_;
	isPushBefore_ = false;
	
}

// Notesクラスの描画関数
void Notes::Render()
{
	// DXTKを管理するインスタンスを取得
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// オブジェクトを描画する
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());

	dxtk.m_spriteBatch->Draw(NotesTexture_.Get(), notesPos_);

	dxtk.m_spriteBatch->End();

}

// Notesクラスのノーツ判定用関数
judge Notes::Notesjudge(SceneBase* scene)
{
	judge result = judge::BLANK;

	m_base = scene;

	float pos = GetNotesPosition().y;

	// キーが押されているときフラグをtrueにする
	isPushBefore_ = true;

	// ノーツが範囲外では判定を取らない
	if (pos < 455.0f) return judge::BLANK;
	
	// 判定できるのか？
	if (canJudge_ == false) {
		if (isPushBeforeOld_ == false)
			canJudge_ = true;

	}

	// キーが押されている状態では判定を取れない
	if (canJudge_ == false) return judge::BLANK;

	// どのタイミングで押したか判定する
	result =
		  pos > 510.0f ? judge::MISS
		: pos > 490.0f ? judge::GOOD
		: pos > 485.0f ? judge::GREAT
		: pos > 475.0f ? judge::PERFECT
		: pos > 465.0f ? judge::GREAT
		: pos > 455.0f ? judge::GOOD
		: judge::BLANK;

	// 判定結果のスコアを加算する
	switch (result)
	{
	case judge::PERFECT:
		m_base->SetScore(m_base->GetScore() + SCORE_ARRAY[0]);
		break;
	case judge::GREAT:
		m_base->SetScore(m_base->GetScore() + SCORE_ARRAY[1]);
		break;
	case judge::GOOD:
		m_base->SetScore(m_base->GetScore() + SCORE_ARRAY[2]);
		break;
	case judge::MISS:
		m_base->SetScore(m_base->GetScore() + SCORE_ARRAY[3]);
		break;
	default:
		break;
	}

	// 判定結果を返す
	return result;

}

// Notesクラスのオブジェクトを降らせる関数
void Notes::FallNotes()
{
	// ノーツのY軸を5ずつ増やしていく
	notesPos_.y += fallSpeed_;
	
}