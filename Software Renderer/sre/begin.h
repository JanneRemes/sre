#ifndef SRE_BEGIN_H
#define SRE_BEGIN_H

#include <stdint.h>
#include "jmp_math.h"

#define SRE_API
#define SRE_BYTES_PER_PIXEL 4

#ifdef __cplusplus
#define SRE_EXTERN_C_BEGIN extern "C" {
#define SRE_EXTERN_C_END }
#else
#define SRE_EXTERN_C_BEGIN
#define SRE_EXTERN_C_END
#endif

#endif // SRE_BEGIN_H
