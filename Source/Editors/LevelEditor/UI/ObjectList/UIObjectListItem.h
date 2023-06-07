#pragma once
class UIObjectListItem: public UITreeItem
{
public:
    UIObjectListItem(shared_str Name);
    virtual ~UIObjectListItem();

    bool           bIsSelected;
    CCustomObject* Object;
    void           Draw();
    void           DrawRoot();
    void           ClearSelcted(UIObjectListItem* Without = nullptr);

protected:
    UITreeItem* CreateItem(shared_str Name, SLocalizedString _HintText) override;
};
