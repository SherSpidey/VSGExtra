//
// Created by ParadoxFang on 2025/5/21.
//

#ifndef IMFILEDIALOG_H
#define IMFILEDIALOG_H

#include <ui/ImWidget.h>

class ImFileDialogParams : public vsg::Inherit<ImParams, ImFileDialogParams>
{
public:
    std::string selected_path;
};


class ImFileDialog : public vsg::Inherit<ImWidget, ImFileDialog>
{
public:
    WIDGET_DECLARATION(ImFileDialog)
    
    static void Draw(const vsg::ref_ptr<ImParams>& in_params);
};


#endif //IMFILEDIALOG_H
