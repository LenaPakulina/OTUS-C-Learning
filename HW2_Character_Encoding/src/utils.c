#include "utils.h"
#include "chars.h"

/* Первые 128 символов в разных кодировках совпадают с utf-8
 * Остальные символы нужно изменять
 *
 * Пример перевода символа П (русская):
 * Юникод = U+041F, это у нас равно 100 0001 1111 (11 бит)
 * Значит в UTF будет занимать 2 байта
 *
 * Есть обозначающие знаки:
 * 110 (для первого байта) и (10) для второго байта + записать саму букву
 * Получаем:
 * 110 10000 (1 байт)
 * 10 011111 (2 байт)
 */

void printUtf8Char(uint16_t symbol, FILE* dst) {
	if (symbol < 0x800) {
		// от 8 до 11 бит могут поместиться в 2 байта
		int8_t firstByte = symbol >> 6; // 6 последних бита пойдут в 2й байт
		firstByte = firstByte & 0x1F; // Сохраним 1 для последних 5 бит
		firstByte = firstByte | 0xC0; // установим 11 в начало
		fputc(firstByte, dst);

		int8_t secondByte = symbol;
		secondByte = secondByte & 0x3F; // Сохраним 1 для последних 6 бит
		secondByte = secondByte | 0x80; // 1 в начало
		fputc(secondByte, dst);
	} else {
		// от 12 до 16 бит могут поместиться в 3 байта (по аналогии)
		fputc(((symbol >> 12) & 0x0F) | 0xE0, dst);
		fputc(((symbol >> 6) & 0x3F) | 0x80, dst);
		fputc((symbol & 0x3F) | 0x80, dst);
	}
}

int16_t *getCurrCode(char *str)
{
	*str = tolower(*str);

	if (strcmp(str, "cp1251") == 0) {
		return charsets[0];
	} else if (strcmp(str, "koi8") == 0) {
		return charsets[1];
	} else if (strcmp(str, "iso8859") == 0) {
		return charsets[2];
	}

	return NULL;
}

