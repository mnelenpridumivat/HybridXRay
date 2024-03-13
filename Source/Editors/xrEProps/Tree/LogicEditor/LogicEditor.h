// Made with usage of https://github.com/thedmd/imgui-node-editor, inspired by Unreal Engine Blueprint system

#pragma once

class UICustomDataEditor;

class XREPROPS_API UILogicEditor: public xrUI
{
    UILogicEditor();

    DEFINE_MAP(shared_str, UICustomDataEditor, OpenedDatas, OpenedDatasIt);

    OpenedDatas Datas = {};

public:

    static UILogicEditor& Instance();

    void Draw() override;

    void                  LoadCustomData(shared_str Key, LPCSTR CustomData);
};
