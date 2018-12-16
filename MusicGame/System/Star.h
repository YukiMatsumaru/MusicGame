#pragma once

#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>

#include "../System/Notes.h"

class Star
{
private:
	// Star�p�e�N�X�`���n���h��
	ID3D11ShaderResourceView* starTexture_;

	// Star��RGBA�J���[
	float red_;
	float green_;
	float blue_;
	float alpha_;

	// Star�̑傫��
	float starScale_;

	// ���[���̍��W
	float pos_;

	// ���[���̐�
	const int lane_Num = 4;

public:
	Star(){};

	~Star() {};

	// Star�N���X�̏������֐�
	void Initialize(ID3D11ShaderResourceView* starTexture,int laneNum,judge judge);

	// Star�N���X�̍X�V�֐�
	bool Update();

	// Star�N���X�̕`��֐�
	void Render();
};