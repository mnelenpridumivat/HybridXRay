#include "stdafx.h"

UITopBarForm::UITopBarForm()
{

#define ADD_BUTTON_IMAGE_T1(Class,Name)
#define ADD_BUTTON_IMAGE_T2(Class,Name)
#define ADD_BUTTON_IMAGE_S(Name)	m_t##Name = EDevice->Resources->_CreateTexture("ed\\bar\\"#Name);m_t##Name->Load();m_time##Name = 0;
#define ADD_BUTTON_IMAGE_D(Name) 	m_t##Name = EDevice->Resources->_CreateTexture("ed\\bar\\"#Name);m_t##Name->Load();m_b##Name = false;
#include "UITopBarForm_ButtonList.h"
	RefreshBar();
}

UITopBarForm::~UITopBarForm()
{
	
}

void UITopBarForm::Draw()
{
}
void UITopBarForm::RefreshBar()
{
}

void UITopBarForm::ClickUndo()
{
	ExecCommand(COMMAND_UNDO);
}

void UITopBarForm::ClickRedo()
{
	ExecCommand(COMMAND_REDO);
}