#ifndef ZIPINFO_H
#define ZIPINFO_H

#include <stdint.h>

// Источник: https://en.wikipedia.org/wiki/ZIP_(file_format)
#define VALID_ZIP_SIGNATURE (0x04034b50)
#define CENTRAL_DIRECTORY_EOCD (0x06054b50) //signature
#define CENTRAL_DIRECTORY_ENTRY_SIGNATURE (0x02014b50)

// Источник: https://blog2k.ru/archives/3391
#pragma pack(push, 1)
struct CentralDirectoryFileHeader
{
	// Обязательная сигнатура, равна 0x02014b50
	uint32_t signature;
	// Версия для создания
	uint16_t versionMadeBy;
	// Минимальная версия для распаковки
	uint16_t versionToExtract;
	// Битовый флаг
	uint16_t generalPurposeBitFlag;
	// Метод сжатия (0 - без сжатия, 8 - deflate)
	uint16_t compressionMethod;
	// Время модификации файла
	uint16_t modificationTime;
	// Дата модификации файла
	uint16_t modificationDate;
	// Контрольная сумма
	uint32_t crc32;
	// Сжатый размер
	uint32_t compressedSize;
	// Несжатый размер
	uint32_t uncompressedSize;
	// Длина название файла
	uint16_t filenameLength;
	// Длина поля с дополнительными данными
	uint16_t extraFieldLength;
	// Длина комментариев к файлу
	uint16_t fileCommentLength;
	// Номер диска
	uint16_t diskNumber;
	// Внутренние аттрибуты файла
	uint16_t internalFileAttributes;
	// Внешние аттрибуты файла
	uint32_t externalFileAttributes;
	// Смещение до структуры LocalFileHeader
	uint32_t localFileHeaderOffset;
//	// Имя файла (длиной filenameLength)
//	uint8_t *filename;
//	// Дополнительные данные (длиной extraFieldLength)
//	uint8_t *extraField;
//	// Комментарий к файла (длиной fileCommentLength)
//	uint8_t *fileComment;
};

struct EOCD // End of central directory record
{
	// Обязательная сигнатура, равна 0x06054b50
	uint32_t signature;
	// Номер диска
	uint16_t diskNumber;
	// Номер диска, где находится начало Central Directory
	uint16_t startDiskNumber;
	// Количество записей в Central Directory в текущем диске
	uint16_t numberCentralDirectoryRecord;
	// Всего записей в Central Directory
	uint16_t totalCentralDirectoryRecord;
	// Размер Central Directory
	uint32_t sizeOfCentralDirectory;
	// Смещение Central Directory
	uint32_t centralDirectoryOffset;
	// Длина комментария
	uint16_t commentLength;
	// Комментарий (длиной commentLength)
	//uint8_t *comment;
};
#pragma pack(pop)

#endif // ZIPINFO_H
