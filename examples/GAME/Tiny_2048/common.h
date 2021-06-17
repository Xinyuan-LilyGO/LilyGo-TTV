#ifndef COMMON_H
#define COMMON_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

//---------------------------------------------------------------------------
typedef unsigned char           u8;
typedef unsigned short          u16;
typedef unsigned long           u32;
typedef signed char             s8;
typedef signed short            s16;
typedef signed long             s32;
typedef volatile u8             vu8;
typedef volatile u16            vu16;
typedef volatile u32            vu32;
typedef volatile s8             vs8;
typedef volatile s16            vs16;
typedef volatile s32            vs32;

#define TRUE                    true
#define FALSE                   false
#define NUM2FIX(N)              ((N) << 8)
#define FIX2NUM(F)              ((F) >> 8)
#define ASSERT(x)               if(!(x)) _SystemError("%s(%d):"#x"\n", __FILE__, __LINE__)

//---------------------------------------------------------------------------
void  _Memset(void *s, u8 c, s16 size);
char *_Sprintf(char *buf, const char *fmt, ...);
void  _SprintfDo(char *str, const char *fmt, va_list ap);
char *_SprintfNum(s16 val, s16 base, char *s);
char *_SprintfNumCol(s16 val, s16 base, char *s, s16 col, char colChr, bool isTop);
char *_SprintfHex(u32 val, char *s, char hex);
char *_SprintfHexCol(u32 val, char *s, s16 col, char colChr, bool isTop, char hex);
char *_SprintfStr(char *val, char *s);
bool  _IsDigit(char c);

void  _SystemError(const char *f, ...);


#ifdef __cplusplus
}
#endif
#endif
