//
// Created by ParadoxFang on 2025/5/23.
//

#ifndef IMBUTTON_H
#define IMBUTTON_H

#include <ui/ImWidget.h>

class ImButtonParams : public vsg::Inherit<ImParams, ImButtonParams>
{
public:
    ImButtonParams();
    
    struct Position
    {
        float x;
        float y;

        Position(std::initializer_list<float> init) {
            auto it = init.begin();
            x = (it != init.end()) ? *it++ : 0.0f;
            y = (it != init.end()) ? *it++ : 0.0f;
        }

        operator std::pair<float, float>() const
        {
            return {x, y}; 
        }
    } position;

    std::string text;
    
    std::function<void()> active;
    std::function<void()> execute;
};

class ImButton :public vsg::Inherit<ImWidget, ImButton>{
public:
    IM_WIDGET_DECLARATION(ImButton)

    static void Draw(const vsg::ref_ptr<ImParams>& in_params);
};



#endif //IMBUTTON_H
