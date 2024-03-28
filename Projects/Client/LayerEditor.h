#pragma once
#include "UI.h"
class LayerEditor :
    public UI
{
private:
	vector<string>			m_LayerName;

public:
	virtual void tick();
	virtual void render_update();

public:
	LayerEditor();
	~LayerEditor();

};

