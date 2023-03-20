/**
 * @file      : ini.c
 * @brief     : C语言版本读写ini文件接口源文件
 * @author    : huenrong (huenrong1028@outlook.com)
 * @date      : 2023-01-18 11:20:25
 *
 * @copyright : Copyright (c) 2023 huenrong
 *
 * @history   : date       author          description
 *              2023-01-16 huenrong        创建文件
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./ini.h"

/**
 * @brief  读取ini文件中的字符串
 * @param  value    : 输出参数, 读取到的参数
 * @param  file_name: 输入参数, 待读取的ini文件
 * @param  section  : 输入参数, 待读取的ini节
 * @param  key      : 输入参数, 待读取的ini键
 * @return true : 成功
 * @return false: 失败
 */
bool get_ini_str(char *value, const char *file_name, const char *section, const char *key)
{
    char *tmp = NULL;
    FILE *fp = NULL;
    // 加上[]后的目标section
    char dst_section[SECTION_MAX_LEN] = {0};
    // 读取到的一行数据
    char read_line_data[LINE_MAX_LEN] = {0};
    // 读取到指定section标志
    bool read_section_flag = false;

    if ((!value) || (!file_name) || (!section) || (!key))
    {
        return false;
    }

    // 打开配置文件
    fp = fopen(file_name, "r");
    // 打开文件失败, 直接退出
    if (NULL == fp)
    {
        return false;
    }

    // 获取目标section串
    snprintf(dst_section, sizeof(dst_section), "[%s]", section);

    // 循环读取数据, 寻找目标section
    while (true)
    {
        // 读取一行数据
        memset(read_line_data, 0, sizeof(read_line_data));
        tmp = fgets(read_line_data, LINE_MAX_LEN, fp);

        // 读取到数据
        if (tmp)
        {
            // 读取到注释行, 空行, 则继续
            if ((';' == read_line_data[0]) || ('#' == read_line_data[0]) ||
                ('\r' == read_line_data[0]) || '\n' == read_line_data[0])
            {
                continue;
            }

            // 找到目标section
            if (!strncmp(dst_section, read_line_data, (strlen(dst_section))))
            {
                read_section_flag = true;

                break;
            }
        }
        // 读取失败, 或者到文件末尾, 直接退出
        else
        {
            fclose(fp);

            return false;
        }
    }

    // 已经找到了目标section, 继续找key
    if (read_section_flag)
    {
        // 循环读取数据(一行一行的读取)
        while (true)
        {
            memset(read_line_data, 0, sizeof(read_line_data));
            tmp = fgets(read_line_data, LINE_MAX_LEN, fp);

            // 读取到数据
            if (NULL != tmp)
            {
                // 读取到注释行, 则继续
                if ((';' == read_line_data[0]) || ('#' == read_line_data[0]))
                {
                    continue;
                }

                // 读取到的该行是键值
                tmp = strchr(read_line_data, '=');
                if (tmp)
                {
                    // 找到目标key
                    if (!strncmp(key, read_line_data, (tmp - read_line_data)))
                    {
                        fclose(fp);

                        tmp++;
                        while ((*tmp != '\r') && (*tmp != '\n'))
                        {
                            *value++ = *tmp++;
                        }

                        return true;
                    }
                }

                // 读取到下一个section, 直接退出
                if ((strchr(read_line_data, '[')) && (strchr(read_line_data, ']')))
                {
                    fclose(fp);

                    return false;
                }
            }
            // 读取失败, 或者到文件末尾, 直接退出
            else
            {
                fclose(fp);

                return false;
            }
        }
    }

    fclose(fp);

    return false;
}

/**
 * @brief  读取ini文件中的整数
 * @param  value    : 输出参数, 读取到的参数
 * @param  file_name: 输入参数, 待读取的ini文件
 * @param  section  : 输入参数, 待读取的ini节
 * @param  key      : 输入参数, 待读取的ini键
 * @return true : 成功
 * @return false: 失败
 */
bool get_ini_int(int *value, const char *file_name, const char *section, const char *key)
{
    char read_data[LINE_MAX_LEN] = {0};

    if ((!value) || (!file_name) || (!section) || (!key))
    {
        return false;
    }

    if (get_ini_str(read_data, file_name, section, key))
    {
        *value = atoi(read_data);

        return true;
    }

    return false;
}

/**
 * @brief  设置ini文件中的字符串
 * @param  file_name: 输入参数, 待设置的ini文件
 * @param  section  : 输入参数, 待设置的ini节
 * @param  key      : 输入参数, 待设置的ini键
 * @param  value    : 输入参数, 待设置的参数
 * @return true : 成功
 * @return false: 失败
 */
bool set_ini_str(const char *file_name, const char *section, const char *key, const char *value)
{
    FILE *fp_r = NULL;
    FILE *fp_w = NULL;
    int flag = 0;
    char read_line_data[LINE_MAX_LEN] = {0};
    char dst_section[SECTION_MAX_LEN] = {0};
    char *tmp = NULL;

    if ((!file_name) || (!section) || (!key) || (!value))
    {
        return false;
    }

    snprintf(dst_section, sizeof(dst_section), "[%s]", section);

    fp_r = fopen(file_name, "r");
    if (NULL == fp_r)
    {
        return false;
    }

    snprintf(read_line_data, sizeof(read_line_data), "%s.tmp", file_name);
    fp_w = fopen(read_line_data, "w");
    if (NULL == fp_w)
    {
        fclose(fp_r);

        return false;
    }

    while (NULL != fgets(read_line_data, LINE_MAX_LEN, fp_r))
    {
        if (2 != flag)
        {
            tmp = strchr(read_line_data, '=');
            if ((NULL != tmp) && (1 == flag))
            {
                // 读取到下一个section, 直接退出
                if ((NULL != strchr(read_line_data, '[')) ||
                    (NULL != strchr(read_line_data, ']')))
                {
                    fclose(fp_r);
                    fclose(fp_w);

                    return false;
                }

                if (0 == strncmp(key, read_line_data, tmp - read_line_data))
                {
                    flag = 2;
                    sprintf(tmp + 1, "%s\n", value);
                }
            }
            else
            {
                if (0 == strncmp(dst_section, read_line_data, strlen(read_line_data) - 1))
                {
                    // 找到目标section
                    flag = 1;
                }
            }
        }

        // 写入临时文件
        fputs(read_line_data, fp_w);
    }

    fclose(fp_r);
    fclose(fp_w);

    snprintf(read_line_data, sizeof(read_line_data), "%s.tmp", file_name);

    // 将临时文件更新到原文件
    return rename(read_line_data, file_name);
}

/**
 * @brief  设置ini文件中的整数
 * @param  file_name: 输入参数, 待设置的ini文件
 * @param  section  : 输入参数, 待设置的ini文件节
 * @param  key      : 输入参数, 待设置的ini文件键
 * @param  value    : 输入参数, 待设置的参数
 * @return true : 成功
 * @return false: 失败
 */
bool set_ini_int(const char *file_name, const char *section, const char *key, const int value)
{
    char set_data[VALUE_MAX_LEN] = {0};

    if ((!file_name) || (!section) || (!key))
    {
        return false;
    }

    snprintf(set_data, sizeof(set_data), "%d", value);

    if (set_ini_str(file_name, section, key, set_data))
    {
        return true;
    }

    return false;
}
