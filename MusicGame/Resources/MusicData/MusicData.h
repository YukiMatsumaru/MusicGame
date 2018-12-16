#pragma once

#include <iostream>

// 楽譜を管理するクラス
struct MusicData
{
	// レーン番号
	int LaneNumber;

	// 音符が落ちてくる時間
	float FallTime;

	// ノーツの種類
	int NotesNumber;

};