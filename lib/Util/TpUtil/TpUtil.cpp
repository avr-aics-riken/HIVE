/**
 * @file TpUtil.cpp
 * CDMデータローダー
 */

#ifndef HIVE_WITH_TPUTIL
#error "HIVE_WITH_TPUTIL must be defined when you compile TPUtil module"
#endif

#include <mpi.h>

#include <stdio.h>
#include <string.h>

#include <fstream>
#include <string>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-result"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#include "TpUtil.h"

#include "TextParser.h"

/// コンストラクタ
TPUtil::TPUtil() { }

/// デストラクタ
TPUtil::~TPUtil() { }


int TPUtil::Parse(const char *filename) {
    TextParser *tp = new TextParser;

    int ret = tp->read(filename);
    if (ret == 0) {
        // SUCCESS

    } else {
        fprintf(stderr, "Failed to read TP file\n");
        return -1;
    }

    ret = tp->remove();
    if (ret == 0) {
        // SUCCESS
    } else {
        fprintf(stderr, "Failed to remove TP instance\n");
        return -2;
    }

    delete tp;

    return 0; // OK
}
