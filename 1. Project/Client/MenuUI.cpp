#include "pch.h"
#include "MenuUI.h"

MenuUI::MenuUI()
	: UI("##MenuUI")
{
}

MenuUI::~MenuUI()
{

}

void MenuUI::render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Level"))
		{

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void MenuUI::render_update()
{
}


