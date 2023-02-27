#ifndef SLOGGER_H
#define SLOGGER_H

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

#define SFL_DEBUG 32
#define SFL_INFO 8
#define SFL_WARNING 4
#define SFL_ERROR 2

#define SFL_ALL 0xFF
#define SFLOG_BACKTRACE_SIZE 32

/**
 * @brief Инициализация с указанием имени лог-файла
 * @return false, если ошибка открытия файла на запись */
bool init_file_log(const char *path, bool need_create_log);

/**
 * @brief Инициализация файловым дискриптором (дескриптор должен быть готов для записи) */
void init_fstream(FILE *fstream);

/**
 * Очистка ресурсов. */
void clear_src();

/**
 * @brief Задает фильтр обработки по типу сообщения note По умолчанию выводятся
 * @все сообщения */
void set_verbosility(int level_flags);

/** Вывод стека функции*/
void print_backtrace();

/**
 * @brief Вывод сообщения в лог-файл (служебная функция)
 * @param level уровень/тип сообщения
 * @param level_name строка для отображения названия уровня
 * @param sync если true, то сбрасывает буфер в файл
 * @param format printf format string
 * @param ... args
 */
void print_level_msg(int level, const char *level_name, const char *format, ...);

/// Базовые макрос-функции для вывода в лог
#define sl_debug_sync(...)     print_level_msg(SFL_DEBUG, "DEBUG", __VA_ARGS__)
#define sl_info_sync(...)      print_level_msg(SFL_INFO, "INFO", __VA_ARGS__)
#define sl_warn_sync(...)      print_level_msg(SFL_WARNING, "WARN", __VA_ARGS__)
#define sl_error_sync(...)     print_level_msg(SFL_ERROR, "ERROR", __VA_ARGS__)

#endif // SLOGGER_H
