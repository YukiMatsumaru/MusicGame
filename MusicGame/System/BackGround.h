#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>


class BackGround
{
private:
	// backeffect�p�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backEffectTexture_;

	// �w�i�p�̃e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> backGroundTexture_;

	// �G�t�F�N�g�̉�]��
	float rotate_;

	// �G�t�F�N�g��RGB�J���[
	float red_;
	float green_;
	float blue_;

	// �G�t�F�N�g�̐F�ύX�p�ϐ�
	float changeColor_;

	// �G�t�F�N�g�̑傫���ύX�p�̕ϐ�
	float changeScale_;
	float scale_;

public:
	// BackGround�N���X�̏������֐�
	void Initialize();

	// BackGround�N���X�̍X�V�֐�
	virtual void Update();

	// BackGround�N���X�̕`��֐�
	void Render();

};