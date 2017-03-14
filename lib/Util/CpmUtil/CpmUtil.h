/**
 * @file CpmUtil.h
 * CPMLib binding
 */
#ifndef HIVE_CPMUTIL_H_
#define HIVE_CPMUTIL_H_

#include "Ref.h"

/**
 * CPMLib binding
 */
class CPMUtil : public RefCount
{
  public:

	CPMUtil();
	~CPMUtil();

  int Proc(); // TODO(IDS): Implement

  private:
};

#endif //HIVE_CPMUTIL_H_
