//
// Created by ParadoxFang on 2025/5/15.
//

#ifndef EXPORT_H
#define EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#   if defined(VSGEXTRA_BUILD_SHARED)
#       ifdef VSGEXTRA_EXPORTS
#           define VSGEXTRA_DECLSPEC   __declspec(dllexport)
#       else
#           define VSGEXTRA_DECLSPEC   __declspec(dllimport)
#       endif
#   else
#       define VSGEXTRA_DECLSPEC
#   endif
#elif defined(__linux__)
#   ifdef VSGEXTRA_EXPORTS
#       define VSGEXTRA_DECLSPEC  __attribute__((visibility("default")))
#   else
#       define VSGEXTRA_DECLSPEC
#   endif
#endif

#endif //EXPORT_H
