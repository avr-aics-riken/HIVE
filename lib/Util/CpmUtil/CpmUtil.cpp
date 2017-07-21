/**
 * @file CpmUtil.cpp
 * CDMデータローダー
 */

#ifndef HIVE_WITH_CPMLIB
#error "HIVE_WITH_CPMLIB must be defined when you compile CPMUtil module"
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

#include "CpmUtil.h"

#include "cpm_ParaManager.h"

/// コンストラクタ
CPMUtil::CPMUtil() { }

/// デストラクタ
CPMUtil::~CPMUtil() { }


int CPMUtil::Proc() { return 1; }
