//
// Created by ParadoxFang on 2025/5/15.
//

#ifndef EXPORT_H
#define EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
#   if defined(VSGEXTRA_BUILD_SHARED)
#       ifdef VSGEXTRA_EXPORTS
#           define VSGEXTRA_EXPORT   __declspec(dllexport)
#       else
#           define VSGEXTRA_EXPORT   __declspec(dllimport)
#       endif
#   else
#       define VSGEXTRA_EXPORT
#   endif
#elif defined(__linux__)
#   ifdef VSGEXTRA_EXPORTS
#       define VSGEXTRA_EXPORT   __attribute__((visibility("default")))
#   else
#       define VSGEXTRA_EXPORT
#   endif
#endif

#endif //EXPORT_H
