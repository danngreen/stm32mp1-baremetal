#if defined(__GNUC__) /* GNU Compiler */
#ifndef __ALIGN4k_END
#define __ALIGN4k_END __attribute__((aligned(64U)))
#endif /* __ALIGN_END */
#ifndef __ALIGN4k_BEGIN
#define __ALIGN4k_BEGIN
#endif /* __ALIGN_BEGIN */
#else
#ifndef __ALIGN4k_END
#define __ALIGN4k_END
#endif /* __ALIGN_END */
#ifndef __ALIGN4k_BEGIN
#if defined(__CC_ARM) /* ARM Compiler */
#define __ALIGN4k_BEGIN __align(64U)
#elif defined(__ICCARM__) /* IAR Compiler */
#define __ALIGN4k_BEGIN
#else
#define __ALIGN4k_BEGIN
#endif /* __CC_ARM */
#endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */
