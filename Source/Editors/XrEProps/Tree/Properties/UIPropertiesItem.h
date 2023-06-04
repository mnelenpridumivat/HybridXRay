#pragma once
class XREPROPS_API UIPropertiesItem: public UITreeItem
{
public:
    UIPropertiesItem(shared_str _Name, UIPropertiesForm* _PropertiesFrom, std::function<const char*()> _HintFunctor);
    virtual ~UIPropertiesItem();
    PropItem*         PItem;
    UIPropertiesForm* PropertiesFrom;
    void              Draw();
    void              DrawRoot();
    void              DrawItem();
    void              DrawProp();

protected:
    virtual UITreeItem* CreateItem(shared_str _Name, std::function<const char*()> _HintFunctor);

private:
    void RemoveMixed();
};
