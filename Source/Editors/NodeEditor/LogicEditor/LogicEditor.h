// Made with usage of https://github.com/thedmd/imgui-node-editor, inspired by Unreal Engine Blueprint system

#pragma once

//#include "../../xrCore/xrCore.h"

class UICustomDataEditor;

class XREPROPS_API UILogicEditor: public xrUI
{
    UILogicEditor();

    DEFINE_MAP(shared_str, UICustomDataEditor*, OpenedDatas, OpenedDatasIt);

    OpenedDatas Datas = {};

    bool        Visible = false;

public:

    static UILogicEditor& Instance();

    void Draw() override;

    void                  LoadCustomData(shared_str Key, LPCSTR CustomData);
    void                  CloseCustomData(shared_str Key);

    void                  SetVisible(bool Visible);
    bool                  GetVisible();
};
