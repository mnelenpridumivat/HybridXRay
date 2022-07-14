#pragma once
class UIBoneForm
{
public:
	UIBoneForm();
	virtual ~UIBoneForm();
	virtual void Draw();
public:
	static void Update();
	static void Show();
private:
	static UIBoneForm* Form;
private:
private:
private:
	void Move(int to);
	void FillBoneParts();
	void Save();
	void SaveTo();
	void LoadFrom();
	void ToDefault();
};

