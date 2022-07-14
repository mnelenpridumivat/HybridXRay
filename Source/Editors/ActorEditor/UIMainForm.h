#pragma once
class UIMainForm
{
public:
	UIMainForm();
	virtual ~UIMainForm();
	virtual void Draw();
	bool Frame();
private:
};
extern UIMainForm* MainForm;
