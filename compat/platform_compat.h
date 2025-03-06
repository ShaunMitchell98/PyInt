#ifndef PYINT_PLATFORM_COMPAT_H
#define PYINT_PLATFORM_COMPAT_H

// Cross-platform compatibility for Windows-specific functions used in PyInt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
    // Using native Windows functions
    #define PYINT_STRCPY(dest, dest_size, src) strcpy_s((dest), (dest_size), (src))
    #define PYINT_STRCAT(dest, dest_size, src) strcat_s((dest), (dest_size), (src))
    #define PYINT_ITOA(value, buffer, buffer_size, radix) _itoa_s((value), (buffer), (buffer_size), (radix))
    #define PYINT_FOPEN(pFile, filename, mode) fopen_s(&(pFile), (filename), (mode))
#else
    // Implementations for non-Windows platforms
    #define PYINT_STRCPY(dest, dest_size, src) \
        (strncpy((dest), (src), (dest_size) - 1), (dest)[(dest_size) - 1] = '\0')

    #define PYINT_STRCAT(dest, dest_size, src) \
        (strncat((dest), (src), (dest_size) - strlen(dest) - 1))
    
    #define PYINT_ITOA(value, buffer, buffer_size, radix) \
        snprintf((buffer), (buffer_size), "%d", (value))
    
    #define PYINT_FOPEN(pFile, filename, mode) \
        ((pFile) = fopen((filename), (mode)))
#endif

#endif // PYINT_PLATFORM_COMPAT_H