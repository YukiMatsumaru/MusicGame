#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>


class Lane
{
private:
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture_;

	// Light�p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> lightTexture_;

	// ���[�����
	bool isLaneKeyPressd_[4];

	// ���[���p�̃^�C�}�[
	int  laneLightTimer_[4];

	// ���摜�̃T�C�Y
	float textureScale_[4];

	// �G�t�F�N�g�p�̃^�C�}�[
	int effectTimer_[4];

	// �G�t�F�N�g�摜�̃T�C�Y
	float effectScale_[4];

	// ���[���t���O
	bool laneFlag_;

public:
	Lane()
		:laneLightTimer_{0}
		,textureScale_{0}
		,effectTimer_{0}
	{};
	~Lane() {};

	// ���[���̐�
	const int lane_Num = 4;

	// �ő厞��
	const int max_Time = 10;

	// Lane�N���X�̏������֐�
	void Initialize();

	// Lane�N���X�̍X�V�֐�
	virtual void Update();

	// Lane�N���X�̕`��֐�
	void Render();

	// ���[���Ɍ���`�悷��֐�
	void DrawLightColor();

	// ���[���t���O��ݒ肷��֐�
	void SetLaneFlag(bool flag) { laneFlag_ = flag; }

	// �L�[����ݒ肷��֐�
	void SetKeyPressd(int keys) { isLaneKeyPressd_[keys] = true; };

};