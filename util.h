//
// Created by mattia.meli on 18/10/2023.
//

#ifndef LOADLIBRARYINJECTOR_UTIL_H
#define LOADLIBRARYINJECTOR_UTIL_H


#include <windef.h>
 class util {
    public:
        static DWORD get_process(const char* lpProcessName);
};


#endif//LOADLIBRARYINJECTOR_UTIL_H
