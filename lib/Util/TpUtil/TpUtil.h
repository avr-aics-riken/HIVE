/**
 * @file TpUtil.h
 * TP util
 */
#ifndef HIVE_TPUTIL_H_
#define HIVE_TPUTIL_H_

#include "Ref.h"

/**
 * TP util
 */
class TPUtil : public RefCount
{
  public:

	TPUtil();
	~TPUtil();

  /// Parse .dfi file.
  int Parse(const char *filename);

  private:
};

#endif //HIVE_TPUTIL_H_
