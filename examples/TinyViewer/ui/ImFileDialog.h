//
// Created by ParadoxFang on 2025/5/21.
//

#ifndef IMFILEDIALOG_H
#define IMFILEDIALOG_H

#include <ui/ImWidget.h>

class ImFileDialogParams : public vsg::Inherit<ImParams, ImFileDialogParams>
{
public:
    enum  FileDialogStatus : std::uint8_t
    {
        Picking = 0,
        Picked
    } status;
    
    std::string selected_path;
};


class ImFileDialog : public vsg::Inherit<ImWidget, ImFileDialog>
{
public:
    IM_WIDGET_DECLARATION(ImFileDialog)
    
    static void Draw(const vsg::ref_ptr<ImParams>& in_params);
};


#endif //IMFILEDIALOG_H
