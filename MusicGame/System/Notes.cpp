#include "../pch.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

#include "../System/DXTKManager.h"

#include "Notes.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �m�[�c�̃e�N�X�`���n���h��
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Notes::NotesTexture_;

// �����O�m�[�c�̃e�N�X�`���n���h��
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Notes::longNotesTexture_;

// �R���X�g���N�^
Notes::Notes(int lanenumber,int notestype)
{	
	// �m�[�c�̏�����
	notesPos_ = Vector2(NOTES_POSITION[lanenumber], 80.0f);

	// ���[�����̏������E���
	lanesNum_ = 0;
	lanesNum_ = lanenumber;

	// �m�[�c�̎�ސ��̏������E���
	notesNum_ = 0;
	notesNum_ = notestype;
	
	fallSpeed_ = 5.0f;

	isPushBeforeOld_ = false;
	isPushBefore_ = false;
	canJudge_ = false;

}

// �f�X�g���N�^
Notes::~Notes()
{

}

// Notes�N���X�̏���������
void Notes::Initialize()
{
	
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// �m�[�c�e�N�X�`���̃��[�h
	CreateWICTextureFromFile(
	dxtk.m_device, L"Resources/Notes.png",
	nullptr, NotesTexture_.GetAddressOf());

}

// Notes�N���X�̍X�V�֐�
void Notes::Update()
{
	// �m�[�c���~�点�邽�߂̊֐�
	FallNotes();

	// ��Ԃ̈ꎞ�ێ�
	isPushBeforeOld_ = isPushBefore_;
	isPushBefore_ = false;
	
}

// Notes�N���X�̕`��֐�
void Notes::Render()
{
	// DXTK���Ǘ�����C���X�^���X���擾
	DXTK::DXTKManager& dxtk = DXTK::DXTKManager::singleton();

	// �I�u�W�F�N�g��`�悷��
	dxtk.m_spriteBatch->Begin(SpriteSortMode_Deferred, dxtk.m_state->NonPremultiplied());

	dxtk.m_spriteBatch->Draw(NotesTexture_.Get(), notesPos_);

	dxtk.m_spriteBatch->End();

}

// Notes�N���X�̃m�[�c����p�֐�
judge Notes::Notesjudge(SceneBase* scene)
{
	judge result = judge::BLANK;

	m_base = scene;

	float pos = GetNotesPosition().y;

	// �L�[��������Ă���Ƃ��t���O��true�ɂ���
	isPushBefore_ = true;

	// �m�[�c���͈͊O�ł͔�������Ȃ�
	if (pos < 455.0f) return judge::BLANK;
	
	// ����ł���̂��H
	if (canJudge_ == false) {
		if (isPushBeforeOld_ == false)
			canJudge_ = true;

	}

	// �L�[��������Ă����Ԃł͔�������Ȃ�
	if (canJudge_ == false) return judge::BLANK;

	// �ǂ̃^�C�~���O�ŉ����������肷��
	result =
		  pos > 510.0f ? judge::MISS
		: pos > 490.0f ? judge::GOOD
		: pos > 485.0f ? judge::GREAT
		: pos > 475.0f ? judge::PERFECT
		: pos > 465.0f ? judge::GREAT
		: pos > 455.0f ? judge::GOOD
		: judge::BLANK;

	// ���茋�ʂ̃X�R�A�����Z����
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

	// ���茋�ʂ�Ԃ�
	return result;

}

// Notes�N���X�̃I�u�W�F�N�g���~�点��֐�
void Notes::FallNotes()
{
	// �m�[�c��Y����5�����₵�Ă���
	notesPos_.y += fallSpeed_;
	
}