#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#define FOSC 22118400L          //ÏµÍ³ÆµÂÊ

#define TRUE                1
#define FALSE               0
#define ENABLE              1
#define DISABLE             0

#ifndef __BOOL
#define __BOOL
typedef bit         		BOOL;
#endif

#ifndef __UINT8
#define __UINT8
typedef unsigned char       UINT8;
#endif

#ifndef __SINT8
#define __SINT8
typedef signed char         SINT8;
#endif

#ifndef __UINT16
#define __UINT16
typedef unsigned int        UINT16;
#endif

#ifndef __SINT16
#define __SINT16
typedef signed int          SINT16;
#endif

#ifndef __UINT32
#define __UINT32
typedef unsigned long		UINT32;
#endif

#ifndef __SINT32
#define __SINT32
typedef signed long			SINT32;
#endif


typedef unsigned char		uint8_t;
typedef unsigned long       uint32_t;

#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) if((expr)==0)assert_failed((uint8_t *)__FILE__, __LINE__)
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) //((void)0)
#endif /* USE_FULL_ASSERT */

#endif
