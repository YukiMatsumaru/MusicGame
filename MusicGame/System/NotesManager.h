#pragma once

#include <vector>

#include "Notes.h"

class NotesManager 
{
private:

	// �V�[�����
	SceneBase* m_base;

	// �m�[�c�z��
	std::vector<Notes*> notes_;

	// �����̐i�s��
	int index_;

	// �擪�m�[�c��Y���W
	float notesPosY_;

	// �m�[�c�ԍ�
	int notesNum_;


public:
	NotesManager();
	~NotesManager();

	// �������֐�
	void Initialize();

	// �X�V�֐�
	void Update();

	// �`��֐�
	void Render();

	// �m�[�c�𔭐�����֐�
	void SpawnNotes();

	// ���Ԃ̍������v�Z����
	float CheckSpawnTime(float csvdata, float time);

	// �I�u�W�F�N�g�𐶐�����֐�
	Notes* CreateNotes(int lanenum,int notetype);

	// ���[�����Ƃɉ������L�[�𔻒������֐�
	judge PlessButton_Check(int laneNumber,int notestype);

	// �m�[�c�̓����蔻�肷��֐�
	judge Notes_Check(int laneNumber);

	// �擪�̃m�[�c��Y���W��n��
	float GetNotesPosition() { return notesPosY_; }

	// ���݂̃��[���ԍ���n��
	int GetNotesNum() { return notesNum_; }

};