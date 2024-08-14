#pragma once
class ESceneAIMapTool;
class UIAIMapTool: public UIToolCustom
{
public:
    UIAIMapTool();
    virtual ~UIAIMapTool();
    virtual void     Draw();
    virtual void     OnDrawUI();
    ESceneAIMapTool* tool;
    IC bool          IsIgnoreConstraints() const
    {
        return m_IgnoreConstraints;
    }
    IC bool IsAutoLink() const
    {
        return m_AutoLink;
    }
    IC bool IsIgnoreSceneObjects() const
    {
        return m_IgnoreSceneObjects;
    }

private:
    enum EMode
    {
        mdAppend,
        mdRemove,
        mdInvert,
    };
    EMode                m_Mode;
    bool                 m_IgnoreConstraints;
    bool                 m_AutoLink;
    bool                 m_IgnoreSceneObjects;
    xr_vector<xr_string> m_IgnoreMaterialsList;
    int                  m_IgnoreMaterialsListSelected;
    bool                 m_ChooseIgnoreMaterials;

private:
    void SideClick(int tag);
};
