/* ---------------------------------------------------------------------------
 * Copyright 2022-2023 LinkSense Co., Ltd.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @File:       ParamExporter.h
 * @Brief:      xxx Function
 *
 * @Author:     John Liu <liuhuijun@lksense.com>
 * @Coauthor:   xxxx xxx <xxxxxxxxx@lksense.com>
 * @Date:       2023/06/03
 * @History:
 *
 *
 * ---------------------------------------------------------------------------
 */

#pragma once

#if defined(_WIN32)
#if defined(LIB_LIBRARY)
#  define LIB_EXPORT __declspec(dllexport)
#else
#  define LIB_EXPORT __declspec(dllimport)
#endif
#else // !defined(_WIN32)
#  define LIB_EXPORT __attribute__((visibility("default")))
#endif
