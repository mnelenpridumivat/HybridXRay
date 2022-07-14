#pragma once
class UIKeyForm
{
public:
	UIKeyForm();
	virtual ~UIKeyForm();
	virtual void Draw();

private:
	void SetMark(int id, int action);
	void DrawMark(int id);
	float m_Position;
	bool m_AutoChange;
	float m_TimeFactor;
};

