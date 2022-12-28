#include "pch.h"
#include "SoundUI.h"

#include <Engine/CSound.h>

SoundUI::SoundUI()
	:ResUI("##SoundUI", RES_TYPE::SOUND)
	, m_iVolume(100.f)
{
}

SoundUI::~SoundUI()
{
}

void SoundUI::update()
{
	ResUI::update();
}

void SoundUI::render_update()
{
	ResUI::render_update();
	
	CSound* pSound = ((CSound*)GetTarget().Get());
	FMOD::Sound* pFmodSound = pSound->m_pSound;
	pFmodSound->getLength(&m_iLength, FMOD_TIMEUNIT_MS);


	ImGui::Text(WstrToStr(pSound->GetName()).c_str());
	ImGui::Text((to_string(m_iLength) + "ms").c_str());
	ImGui::NewLine();

	ImGui::Text("Volume "); ImGui::SameLine();
	ImGui::SliderInt("##Volume ", &m_iVolume, 0, 255);
	if (ImGui::Button("Play"))
	{
		pSound->Play(1, (float)m_iVolume / 255.f);
	}
}


