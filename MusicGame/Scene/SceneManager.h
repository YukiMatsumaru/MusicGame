/// <summary>
/// SceneManager.h
/// </summary>

#pragma once
#include "../StepTimer.h"
#include "../System/DXTKManager.h"

class SceneBase;

// �t�F�[�h�p����
static const int FADE_TIME = 120;

// �X�R�A�̌���
static const int SCORE_NUM = 6;

// �S���l
static const int ALL_NUM = 10;

// �摜���W�̏����l
static const int TEXTURE_POSITION = 100;

// BGM�̎��� 84.0f
static const float BGM_TIME = 84.0f;

// SE�̃{�����[��
static const float SE_VOLUME = 0.3f;

// ����I�����W
static const float JUDGE_END_POSITION = 510.0f;

class SceneManager
{
private:
	SceneBase* m_base;

public:
	SceneManager();
	virtual ~SceneManager();

	//�V�[���ύX
	void ChangeScene(SceneBase* nextScene);

	//�X�V�֐�
	void Update();

	//�`��֐�
	void Render();

};

class SceneBase
{
private:
	// �X�R�A�Ǘ��ϐ�
	int score = 0;

public:
	//���z�f�X�g���N�^
	virtual ~SceneBase() {};

	virtual void Initialize() = 0;
	virtual void UpdateScene(SceneManager* scene) = 0;
	virtual void Render() = 0;

	// �I�u�W�F�N�g��j������
	virtual void Dispose() = 0;

	// �X�R�A���擾����֐�
	int GetScore() { return score; }

	// �X�R�A��ݒ肷��֐�
	void SetScore(int score) { this->score = score; }

};
