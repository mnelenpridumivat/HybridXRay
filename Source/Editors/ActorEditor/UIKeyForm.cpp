#include "stdafx.h"

UIKeyForm::UIKeyForm():m_AutoChange(true), m_TimeFactor(1), m_Position(0)
{
}

UIKeyForm::~UIKeyForm()
{
}

void UIKeyForm::Draw()
{
}
inline bool interval_comparer(const motion_marks::interval& i1, const motion_marks::interval& i2)
{
	return (i1.first < i2.first);
}
void UIKeyForm::SetMark(int id, int action)
{

}

void UIKeyForm::DrawMark(int id)
{

}
