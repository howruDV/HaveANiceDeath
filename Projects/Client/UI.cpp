#include "pch.h"
#include "UI.h"

UI::UI(const string& _strName, const string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
	, m_bModal(false)
{
}

UI::~UI()
{
	Delete_Vec(m_vecChild);
}


void UI::tick()
{
}

void UI::render()
{
	if (!m_bActive)
		return;

	// case: parent UI
	if (!m_Parent)
	{
		// case: window
		if (!m_bModal)
		{
			ImGui::Begin(string(m_strName + m_strID).c_str(), &m_bActive);

			// render
			render_update();
			for (size_t i = 0; i < m_vecChild.size(); ++i)
				m_vecChild[i]->render();

			ImGui::End();
		}
		// case: modal
		else
		{
			ImGui::OpenPopup(string(m_strName + m_strID).c_str());
			if (ImGui::BeginPopupModal(string(m_strName + m_strID).c_str(), &m_bActive))
			{
				// render
				render_update();
				for (size_t i = 0; i < m_vecChild.size(); ++i)
					m_vecChild[i]->render();

				ImGui::EndPopup();
			}
			else
			{
				// check window activatation is changed
				if (m_bActive)
					m_bActive = false;
			}
		}
	}

	// case: child UI
	else
	{
		ImGui::BeginChild(string(m_strName + m_strID).c_str());
		
		render_update();
		for (size_t i=0; i<m_vecChild.size(); ++i)
			m_vecChild[i]->render();

		ImGui::EndChild();
	}
}