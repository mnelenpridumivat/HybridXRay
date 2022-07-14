#pragma once
class UILeftBarForm
{
public:
	enum RenderMode
	{
		Render_Editor,
		Render_Engine
	};
	UILeftBarForm();
	virtual ~UILeftBarForm();
	virtual void Draw();
private:
	int m_PickMode;
};

