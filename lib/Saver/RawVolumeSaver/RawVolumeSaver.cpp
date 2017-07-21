/**
 * @file RawVolumeSaver.cpp
 * RawVolumeSaverクラス
 */
#include "RawVolumeSaver.h"

#include <cstring>
#include <string>

namespace {

static void
swap4(unsigned int* val)
{
  unsigned int tmp = *val;
  unsigned char *dst = (unsigned char*)val;
  unsigned char *src = (unsigned char*)&tmp;

  dst[0] = src[3];
  dst[1] = src[2];
  dst[2] = src[1];
  dst[3] = src[0];
}

/// ビッグエンディアンかどうか返す
bool IsBigEndian(void)
{
    union {
        unsigned int i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

} // namespace

/// コンストラクタ
RawVolumeSaver::RawVolumeSaver(){}

/// デストラクタ
RawVolumeSaver::~RawVolumeSaver(){};

/**
 * RawVolumeSaverへのセーブ
 * @param filename セーブファイル名
 * @param appendFooter append footer?(20 bytes)
 * @retval true  セーブ成功
 * @retval false セーブ失敗
 */
bool RawVolumeSaver::Save(const char* filename, bool appendFooter){
    // @todo { double precision. }
    // @todo { endian. }

    assert(m_volumeData->Component() == 1 || m_volumeData->Component() == 3);

	FILE* fp = fopen(filename, "wb");
	if (!fp) {
		return false;
	}

	const unsigned char* ptr = reinterpret_cast<unsigned char*>(m_volumeData->Buffer()->GetBuffer());
	size_t dataLen = m_volumeData->Buffer()->GetNum() * sizeof(float);

	printf("[RawVolumeSaver] DataLen = %d\n", int(dataLen));

	size_t len = fwrite(ptr, 1, dataLen, fp);
	assert(len == dataLen);

	if (appendFooter) {
		int type = 8; // float
		int components = m_volumeData->Component();
		int w = m_volumeData->Width();
		int h = m_volumeData->Height();
		int d = m_volumeData->Depth();

		fwrite(&type, 1, sizeof(int), fp);
		fwrite(&components, 1, sizeof(int), fp);
		fwrite(&w, 1, sizeof(int), fp);
		fwrite(&h, 1, sizeof(int), fp);
		fwrite(&d, 1, sizeof(int), fp);
	}

	fclose(fp);

	return true;
}
