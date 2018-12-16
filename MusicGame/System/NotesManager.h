#pragma once

#include <vector>

#include "Notes.h"

class NotesManager 
{
private:

	// シーン情報
	SceneBase* m_base;

	// ノーツ配列
	std::vector<Notes*> notes_;

	// 音符の進行状況
	int index_;

	// 先頭ノーツのY座標
	float notesPosY_;

	// ノーツ番号
	int notesNum_;


public:
	NotesManager();
	~NotesManager();

	// 初期化関数
	void Initialize();

	// 更新関数
	void Update();

	// 描画関数
	void Render();

	// ノーツを発生する関数
	void SpawnNotes();

	// 時間の差分を計算する
	float CheckSpawnTime(float csvdata, float time);

	// オブジェクトを生成する関数
	Notes* CreateNotes(int lanenum,int notetype);

	// レーンごとに押したキーを判定をする関数
	judge PlessButton_Check(int laneNumber,int notestype);

	// ノーツの当たり判定する関数
	judge Notes_Check(int laneNumber);

	// 先頭のノーツのY座標を渡す
	float GetNotesPosition() { return notesPosY_; }

	// 現在のレーン番号を渡す
	int GetNotesNum() { return notesNum_; }

};