#include "main.h"

/**
 * _atoi - convert a string to an integer
 * @s: the string to convert
 *
 * Return: the converted integer, or 0 if no digits are found
 */
int _atoi(char *s)
{
	int i = 0;
	int sign = 1;
	int started = 0;
	int num = 0;

	/* امشِ على السلسلة كاملة */
	while (s[i] != '\0')
	{
		if (s[i] == '-')
		{
			if (!started)
				sign = -sign;     /* كل '-' يقلب الإشارة قبل بداية الرقم */
		}
		else if (s[i] == '+')
		{
			/* نتجاهل '+' قبل بداية الرقم */
		}
		else if (s[i] >= '0' && s[i] <= '9')
		{
			started = 1;
			num = num * 10 + (s[i] - '0');  /* كوّن القيمة */
		}
		else if (started)
		{
			/* إذا بدأنا نقرأ أرقام ثم وجدنا محرف غير رقمي،*
