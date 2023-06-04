#pragma once
class XREPROPS_API UITreeItem
{
public:
    UITreeItem(shared_str _Name, std::function<const char*()> _HintFunctor);
    virtual ~UITreeItem();

    UITreeItem*            AppendItem(const char* _Path, std::function<const char*()> _HintFunctor = nullptr, char _PathChar= '\\');
    UITreeItem*            FindItem(const char* _Path, char _PathChar = '\\');
    xr_vector<UITreeItem*>       Items;
    UITreeItem*                  Owner;
    shared_str                   Name;
    std::function<const char*()> HintFunctor;

protected:
    virtual UITreeItem* CreateItem(shared_str _Name, std::function<const char*()> _HintFunctor);
    void                ShowHintIfHovered();
};
