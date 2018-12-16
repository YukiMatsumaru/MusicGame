#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"

#include "NotesManager.h"
#include "../System/Lane.h"
#include "../Scene/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// コンストラクタ
NotesManager::NotesManager()
{
	m_base = PlayScene::GetInstance();
}

// デストラクタ
NotesManager::~NotesManager()
{
	
}

// 初期化処理
void NotesManager::Initialize()
{
	Notes::Initialize();

	// 進行状況の初期化
	index_ = 0;

	// ノーツ番号の初期化
	notesNum_ = 0;

}

// 更新処理
void NotesManager::Update()
{
	// ノーツを発生させる
	SpawnNotes();

	// ノーツを更新する
	for (std::vector<Notes*>::iterator itr = notes_.begin(); itr != notes_.end();itr++)
	{
		if ((*itr) != nullptr)
		{
			(*itr)->Update();

			// ノーツの先頭の座標を渡す
			notesPosY_ = itr[0]->GetNotesPosition().y;
		}
	}

	// 不要なノーツを削除する
	for (std::vector<Notes*>::iterator itr = notes_.begin(); itr != notes_.end();)
	{
		if ((*itr) != nullptr)
		{
			if ((*itr)->GetNotesPosition().y >= 620.0f)
			{
				delete (*itr);
				itr = notes_.erase(itr);
				continue;
			}			
			itr++;
		}
	}
	
}

// 描画処理
void NotesManager::Render()
{
	for (std::vector<Notes*>::iterator itr = notes_.begin(); itr != notes_.end(); itr++)
	{
		if ((*itr) != nullptr)
		{
			(*itr)->Render();
		}
	}
}

// ノーツを発生する
void NotesManager::SpawnNotes()
{
	// CSVデータをもらう
	PlayScene* instance = dynamic_cast<PlayScene*>(PlayScene::GetInstance());
	const std::vector<MusicData>& musicdata = instance->GetNotesData();
	const float time = instance->GetTimer();

	std::vector<int> laneNums;
	std::vector<int> notesNums;

	// 指定の時間のノーツを調べる
	while (true)
	{
		// 全てのノーツがあるか
		if (musicdata.size() < index_ + 1) break;

		// 差分の許容範囲
		const float safeLevel = 0.1f;
		if (std::fabsf(CheckSpawnTime(musicdata[index_].FallTime, time)) < safeLevel)
		{
			laneNums.emplace_back(musicdata[index_].LaneNumber);
			notesNums.emplace_back(musicdata[index_].NotesNumber);
			index_++;
			continue;
		}

		break;
	}

	// 発生させる
	for (auto it = laneNums.begin(); it != laneNums.end();it++)
	{
		for (auto itr = notesNums.begin(); itr !=notesNums.end();itr++)
		{
			CreateNotes(*it,*itr);	
		}
	}
	
}

// 時間の差分を計算する
float NotesManager::CheckSpawnTime(float csvdata, float time)
{
	return csvdata - time;
}

// オブジェクトを生成する
Notes* NotesManager::CreateNotes(int lanenum,int notetype)
{	

	// ノーツ個数を代入
	notesNum_ = index_;
	
	// ノーツ変数
	Notes* note;

	// ノーツの初期化
	note = new Notes(lanenum, notetype);
	notes_.push_back(note);

	return note;

}

judge NotesManager::PlessButton_Check(int laneNumber,int notestype)
{
	// 判定結果
	judge result = BLANK;

	// 判定
	switch (notestype)
	{
	case 0:
		result = Notes_Check(laneNumber);
		break;
	default:
		assert(!"レーン番号が不正値");
		break;
	}

	return result;
}

// ノーツの当たり判定する関数
judge NotesManager::Notes_Check(int laneNumber)
{
	judge result = BLANK;

	float pos = NOTES_POSITION[laneNumber];
	
	for (std::vector<Notes*>::iterator itr = notes_.begin(); itr != notes_.end();)
	{	
		if ((*itr)->GetNotesPosition().x <= pos)
		{

			result = (*itr)->Notesjudge(m_base);
				
			if (result != judge::BLANK)
			{
				delete (*itr);
				itr = notes_.erase(itr);
				continue;	
			}
			itr++;
				
		}
	}

	return result;
}


