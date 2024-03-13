#pragma once

class XREPROPS_API UICustomDataEditor: public xrUI
{

public:

    UICustomDataEditor(shared_str Key, LPCSTR CustomData);

    void Draw() override;
};