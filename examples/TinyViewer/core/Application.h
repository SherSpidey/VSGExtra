//
// Created by ParadoxFang on 2025/5/20.

#ifndef VIEWER_H
#define VIEWER_H

#include <cstdint>

/**
 *  Main class to hold vsg window, viewer and options
 */
class Application {
public:
    enum ExitCode : std::uint8_t
    {
        SUCCESS = 0,
        ERROR_VSG_FAILED = 1,
        ERROR_GENERAL_FAILED = 2
    };
    
    Application();

    int exec();

private:
    ExitCode _result {};
    class VSGApplication* _vsg_app;
};



#endif //VIEWER_H
