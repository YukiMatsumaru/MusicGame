/// <summary>
/// SceneState.h
/// </summary>

#pragma once
#include "../StepTimer.h"
#include "../System/DXTKManager.h"

class SceneBase;

// �t�F�[�h�p����
static const int FADE_TIME = 120;

class SceneState
{
private:
	SceneBase* m_base;

	// �^�C�}�[
	const DX::StepTimer& timer;

public:
	SceneState(const DX::StepTimer& timer);
	virtual ~SceneState();

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
	int Score = 0;

public:
	//���z�f�X�g���N�^
	virtual ~SceneBase() {};

	virtual void Initialize() = 0;
	virtual void UpdateScene(SceneState* scene) = 0;
	virtual void Render() = 0;

	// �X�R�A���擾����֐�
	int GetScore() { return Score; }

	// �X�R�A��ݒ肷��֐�
	void SetScore(int score) { Score = score; }

};
