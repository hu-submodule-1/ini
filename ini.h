/**
 * @file      : ini.h
 * @brief     : C语言版本读写ini文件接口头文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-01-18 11:20:35
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-01-16 huenrong        创建文件
 *
 */

#ifndef __INI_H
#define __INI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

// section最大长度, 包括[]符号
#define SECTION_MAX_LEN 32
// 单行数据最大长度
#define LINE_MAX_LEN 1024
// value最大长度
#define VALUE_MAX_LEN 256

/**
 * @brief  读取ini文件中的字符串
 * @param  value    : 输出参数, 读取到的参数
 * @param  file_name: 输入参数, 待读取的ini文件
 * @param  section  : 输入参数, 待读取的ini节
 * @param  key      : 输入参数, 待读取的ini键
 * @return true : 成功
 * @return false: 失败
 */
bool get_ini_str(char *value, const char *file_name, const char *section, const char *key);

/**
 * @brief  读取ini文件中的整数
 * @param  value    : 输出参数, 读取到的参数
 * @param  file_name: 输入参数, 待读取的ini文件
 * @param  section  : 输入参数, 待读取的ini节
 * @param  key      : 输入参数, 待读取的ini键
 * @return true : 成功
 * @return false: 失败
 */
bool get_ini_int(int *value, const char *file_name, const char *section, const char *key);

/**
 * @brief  设置ini文件中的字符串
 * @param  file_name: 输入参数, 待设置的ini文件
 * @param  section  : 输入参数, 待设置的ini节
 * @param  key      : 输入参数, 待设置的ini键
 * @param  value    : 输入参数, 待设置的参数
 * @return true : 成功
 * @return false: 失败
 */
bool set_ini_str(const char *file_name, const char *section, const char *key, const char *value);

/**
 * @brief  设置ini文件中的整数
 * @param  file_name: 输入参数, 待设置的ini文件
 * @param  section  : 输入参数, 待设置的ini文件节
 * @param  key      : 输入参数, 待设置的ini文件键
 * @param  value    : 输入参数, 待设置的参数
 * @return true : 成功
 * @return false: 失败
 */
bool set_ini_int(const char *file_name, const char *section, const char *key, const int value);

#ifdef __cplusplus
}
#endif

#endif // __INI_H
