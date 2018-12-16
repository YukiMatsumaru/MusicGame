#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"

#include "NotesManager.h"
#include "../System/Lane.h"
#include "../Scene/PlayScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �R���X�g���N�^
NotesManager::NotesManager()
{
	m_base = PlayScene::GetInstance();
}

// �f�X�g���N�^
NotesManager::~NotesManager()
{
	
}

// ����������
void NotesManager::Initialize()
{
	Notes::Initialize();

	// �i�s�󋵂̏�����
	index_ = 0;

	// �m�[�c�ԍ��̏�����
	notesNum_ = 0;

}

// �X�V����
void NotesManager::Update()
{
	// �m�[�c�𔭐�������
	SpawnNotes();

	// �m�[�c���X�V����
	for (std::vector<Notes*>::iterator itr = notes_.begin(); itr != notes_.end();itr++)
	{
		if ((*itr) != nullptr)
		{
			(*itr)->Update();

			// �m�[�c�̐擪�̍��W��n��
			notesPosY_ = itr[0]->GetNotesPosition().y;
		}
	}

	// �s�v�ȃm�[�c���폜����
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

// �`�揈��
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

// �m�[�c�𔭐�����
void NotesManager::SpawnNotes()
{
	// CSV�f�[�^�����炤
	PlayScene* instance = dynamic_cast<PlayScene*>(PlayScene::GetInstance());
	const std::vector<MusicData>& musicdata = instance->GetNotesData();
	const float time = instance->GetTimer();

	std::vector<int> laneNums;
	std::vector<int> notesNums;

	// �w��̎��Ԃ̃m�[�c�𒲂ׂ�
	while (true)
	{
		// �S�Ẵm�[�c�����邩
		if (musicdata.size() < index_ + 1) break;

		// �����̋��e�͈�
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

	// ����������
	for (auto it = laneNums.begin(); it != laneNums.end();it++)
	{
		for (auto itr = notesNums.begin(); itr !=notesNums.end();itr++)
		{
			CreateNotes(*it,*itr);	
		}
	}
	
}

// ���Ԃ̍������v�Z����
float NotesManager::CheckSpawnTime(float csvdata, float time)
{
	return csvdata - time;
}

// �I�u�W�F�N�g�𐶐�����
Notes* NotesManager::CreateNotes(int lanenum,int notetype)
{	

	// �m�[�c������
	notesNum_ = index_;
	
	// �m�[�c�ϐ�
	Notes* note;

	// �m�[�c�̏�����
	note = new Notes(lanenum, notetype);
	notes_.push_back(note);

	return note;

}

judge NotesManager::PlessButton_Check(int laneNumber,int notestype)
{
	// ���茋��
	judge result = BLANK;

	// ����
	switch (notestype)
	{
	case 0:
		result = Notes_Check(laneNumber);
		break;
	default:
		assert(!"���[���ԍ����s���l");
		break;
	}

	return result;
}

// �m�[�c�̓����蔻�肷��֐�
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


