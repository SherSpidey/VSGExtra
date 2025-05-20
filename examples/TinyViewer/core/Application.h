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
    enum exit_code : std::uint8_t
    {
        SUCCESS = 0,
        ERROR_VSG_FAILED = 1,
        ERROR_GENERAL_FAILED = 2
    };
    
    Application();

    int exec();

private:
    exit_code _result {};
    class AppPimpl* _m_pimpl;
};



#endif //VIEWER_H
