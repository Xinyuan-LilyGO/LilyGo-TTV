#include "common.h"
#include "oled.h"

//---------------------------------------------------------------------------
void _Memset(void* s, u8 c, s16 size)
{
	volatile char* s1 = (char*)s;

	if(size == 0)
	{
		goto End;
	}

	size++;
	while(--size > 0)
	{
		*s1++ = c;
	}

End: ;
}
//---------------------------------------------------------------------------
char* _Sprintf(char* buf, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(buf, fmt, ap);
	va_end(ap);

	return buf;
}
//---------------------------------------------------------------------------
void _SprintfDo(char* str, const char* fmt, va_list ap)
{
	s16   val;
	char* val2;
	char  val3;

	char c;
	s16  col = 0;
	char colChr = ' ';
	bool isCol;

	for(;;)
	{
		c = *fmt++;

		if(c == '\0')
		{
			*str++ = '\0';
			return;
		}

		if(c != '%')
		{
			*str++ = c;
			continue;
		}

		c = *fmt++;
		if(c == '0')
		{
			colChr = '0';
			c = *fmt++;
		}

		if(_IsDigit(c) == TRUE)
		{
			col   = c - '0';
			isCol = TRUE;
			c = *fmt++;
		}
		else
		{
			isCol = FALSE;
		}

		switch(c)
		{
		case 'd':
			val  = va_arg(ap, int);

			if(val < 0)
			{
				val *= -1;
				*str++ = '-';
			}

			str = (isCol == TRUE) ? _SprintfNumCol(val, 10, str, col, colChr, TRUE) : _SprintfNum(val, 10, str); 
			break;

		case 'x':
			str = (isCol == TRUE) ? _SprintfHexCol((u16)va_arg(ap, int), str, col, colChr, TRUE, 'a') : _SprintfHex((u16)va_arg(ap, int), str, 'a'); 
			break;

		case 'l':
			str = (isCol == TRUE) ? _SprintfHexCol((u32)va_arg(ap, long), str, col, colChr, TRUE, 'a') : _SprintfHex((u32)va_arg(ap, long), str, 'a'); 
			break;

		case 's':
			val2 = va_arg(ap, char*);
			str = _SprintfStr(val2, str);
			break;

		case 'c':
			val3  = va_arg(ap, int);
			*str++ = val3;
			break;

		case '\0':
		default:
			*str++ = '\0';
			return;
		}
	}
}
//---------------------------------------------------------------------------
char* _SprintfNum(s16 val, s16 base, char* s)
{
	s16 c = val % base;
	val = val / base;

	if(val > 0)
	{
		s = _SprintfNum(val, base, s);
	}

	*s++ = c+'0';

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfNumCol(s16 val, s16 base, char* s, s16 col, char colChr, bool isTop)
{
	s16 c = val % base;
	val = val / base;

	if(val > 0 || col > 1)
	{
		s = _SprintfNumCol(val, base, s, col-1, colChr, FALSE);
	}

	if(c != 0 || val != 0 || isTop == TRUE)
	{
		*s++ = c+'0';
	}
	else
	{
		*s++ = colChr;
	}

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfHexCol(u32 val, char* s, s16 col, char colChr, bool isTop, char hex)
{
	if(val >= 0x10 || col > 1)
	{
		s = _SprintfHexCol(val >> 4, s, col-1, colChr, FALSE, hex);
	}

	u16 c = val & 0xf;

	if(c != 0 || val != 0 || isTop == TRUE)
	{
		*s++ = (c>9) ? c-10+hex : c+'0';
	}
	else
	{
		*s++ = colChr;
	}

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfHex(u32 val, char* s, char hex)
{
	if(val >= 0x10)
	{
		s = _SprintfHex(val >> 4, s, hex);
	}

	u16 c = val & 0xf;
	*s++ = (c>9) ? c-10+hex : c+'0';

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfStr(char* val, char* s)
{
	while(*val != '\0')
	{
		*s++ = *val++;
	}

	return s;
}
//---------------------------------------------------------------------------
bool _IsDigit(char c)
{
	return (c >= '0') && (c <= '9') ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
void _SystemError(const char* fmt, ...)
{
	char s[40];

	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(s, fmt, ap);
	va_end(ap);


	u16 i, x = 0;

	for(i=0; s[i] != '\0'; i++)
	{
		OledDrawChr(x++, 0, s[i]);
	}

	OledDisplay();

	for(;;)
	{
		// EMPTY
	}
}
