#include "../pch.h"
#include "SceneState.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "d3d11.h"
#include "SimpleMath.h"
#include "WICTextureLoader.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneState::SceneState(const DX::StepTimer& timer)
	:timer(timer)
{
	m_base = nullptr;
}

SceneState::~SceneState()
{

}

//シーン変更
void SceneState::ChangeScene(SceneBase* nextScene)
{
	// 一時的にスコアを管理する変数
	int score = 0;

	if (m_base != nullptr)
	{
		score = m_base->GetScore();
	}

	m_base = nextScene;
	m_base->Initialize();
	m_base->SetScore(score);
}

//更新関数
void SceneState::Update()
{
	m_base->UpdateScene(this);
}

//描画関数
void SceneState::Render()
{
	
	m_base->Render();
}
