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

// ���[���̊�l
const float NOTES_POSITION[] = { 150.0f,300.0f,450.0f,600.0f };

// ���_�z��
const int SCORE_ARRAY[] = { 1000,100,10,0 };


class Notes
{
protected:
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch_;

	// �m�[�c�̃e�N�X�`���n���h��
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> NotesTexture_;

	// �����O�m�[�c�̃e�N�X�`���n���h��
	static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> longNotesTexture_;

	// �V�[�����
	SceneBase* m_base;

	//�m�[�c�̃|�W�V����
	DirectX::SimpleMath::Vector2 notesPos_;

	// ���[���̐�
	int lanesNum_;

	// �m�[�c�̌�
	int notesNum_;

	// �����������Ă���X�s�[�h
	float fallSpeed_;

	// ����͈͓��ɓ���O�ɃL�[�������Ă��邩
	bool isPushBefore_;
	bool isPushBeforeOld_;
	// ����\��
	bool canJudge_;

public:
	Notes(int lanenumber,int notestype);
	virtual ~Notes();

	// Notes�N���X�̏������֐�
	static void Initialize();

	// Notes�N���X�̍X�V�֐�
	virtual void Update();

	// Notes�N���X�̕`��֐�
	void Render();

	// �m�[�c����֐�
	judge Notesjudge(SceneBase* scene);

	// �m�[�c���~�点��֐�
	void FallNotes();

	// �m�[�c�̍��W���擾����
	DirectX::SimpleMath::Vector2 GetNotesPosition()  { return notesPos_; }

};