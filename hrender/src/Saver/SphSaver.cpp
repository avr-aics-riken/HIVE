#include "SphSaver.h"
#include "SimpleSPH.h"

#include <cstring>
#include <string>

namespace {

#ifdef __GNUC__
#pragma pack(push, 1)
#define ALIGNMENT __attribute__((packed))
#elif (__FCC_VERSION >= 600)
#pragma pack(1)
#define ALIGNMENT __attribute__((packed))
#else
#pragma pack(1)
#define ALIGNMENT 
#endif // __GNUC__  

static void
swap2(unsigned short* val)
{
  unsigned short tmp = *val;
  unsigned char *dst = (unsigned char*)val;
  unsigned char *src = (unsigned char*)&tmp;

  dst[0] = src[1];
  dst[1] = src[0];
}

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

static void
swap8(unsigned char* val)
{
  unsigned char tmp[8];
  tmp[0] = val[0];
  tmp[1] = val[1];
  tmp[2] = val[2];
  tmp[3] = val[3];
  tmp[4] = val[4];
  tmp[5] = val[5];
  tmp[6] = val[6];
  tmp[7] = val[7];
  unsigned char *dst = (unsigned char*)val;

  dst[0] = tmp[7];
  dst[1] = tmp[6];
  dst[2] = tmp[5];
  dst[3] = tmp[4];
  dst[4] = tmp[3];
  dst[5] = tmp[2];
  dst[6] = tmp[1];
  dst[7] = tmp[0];
}

struct SphHeaderTop {
  unsigned int SizeTop_0;
  unsigned int svType;
  unsigned int dType;
  unsigned int SizeTop_1;

  SphHeaderTop() : SizeTop_0(8), SizeTop_1(8) {}

} ALIGNMENT;

struct SphHeaderSingle {
  unsigned int SizeSz_0;
  int size[3];
  unsigned int SizeSz_1;

  unsigned int SizeOrg_0;
  float org[3];
  unsigned int SizeOrg_1;

  unsigned int SizePitch_0;
  float pitch[3];
  unsigned int SizePitch_1;

  unsigned int SizeStep_0;
  int step;
  float time;
  unsigned int SizeStep_1;

  SphHeaderSingle()
      : SizeSz_0(12), SizeSz_1(12), SizeOrg_0(12), SizeOrg_1(12),
        SizePitch_0(12), SizePitch_1(12), SizeStep_0(8), SizeStep_1(8) {}

} ALIGNMENT;

struct SphHeaderDouble {
  unsigned int SizeSz_0;
  long size[3];
  unsigned int SizeSz_1;

  unsigned int SizeOrg_0;
  double org[3];
  unsigned int SizeOrg_1;

  unsigned int SizePitch_0;
  double pitch[3];
  unsigned int SizePitch_1;

  unsigned int SizeStep_0;
  long step;
  double time;
  unsigned int SizeStep_1;

  SphHeaderDouble()
      : SizeSz_0(24), SizeSz_1(24), SizeOrg_0(24), SizeOrg_1(24),
        SizePitch_0(24), SizePitch_1(24), SizeStep_0(16), SizeStep_1(16) {}

} ALIGNMENT;

#ifdef __GNUC__
#pragma pack(pop)
#else // __GNUC__
#pragma pack()
#endif // __GNUC__

class SphData {
public:
  // for Single Precision
  SphData(const SphHeaderTop &headerTop, const SphHeaderSingle &header,
          const void *data) {
    m_numComp = headerTop.svType == 1 ? 1 : 3;
    m_precision = headerTop.dType == 1 ? sizeof(float) : sizeof(double);

    m_size[0] = static_cast<long>(header.size[0]);
    m_size[1] = static_cast<long>(header.size[1]);
    m_size[2] = static_cast<long>(header.size[2]);

    m_org[0] = static_cast<double>(header.org[0]);
    m_org[1] = static_cast<double>(header.org[1]);
    m_org[2] = static_cast<double>(header.org[2]);

    m_pitch[0] = static_cast<double>(header.pitch[0]);
    m_pitch[1] = static_cast<double>(header.pitch[1]);
    m_pitch[2] = static_cast<double>(header.pitch[2]);

    m_step = static_cast<long>(header.step);
    m_time = static_cast<double>(header.time);

    size_t allSz = m_size[0] * m_size[1] * m_size[2] * m_precision * m_numComp;
    m_data = new unsigned char[allSz];

    memcpy(m_data, data, sizeof(unsigned char) * allSz);
  }

  // for Double Precision
  SphData(const SphHeaderTop &headerTop, const SphHeaderDouble &header,
          const void *data) {
    m_numComp = headerTop.svType == 1 ? 1 : 3;
    m_precision = headerTop.dType == 1 ? sizeof(float) : sizeof(double);

    m_size[0] = static_cast<long>(header.size[0]);
    m_size[1] = static_cast<long>(header.size[1]);
    m_size[2] = static_cast<long>(header.size[2]);

    m_org[0] = static_cast<double>(header.org[0]);
    m_org[1] = static_cast<double>(header.org[1]);
    m_org[2] = static_cast<double>(header.org[2]);

    m_pitch[0] = static_cast<double>(header.pitch[0]);
    m_pitch[1] = static_cast<double>(header.pitch[1]);
    m_pitch[2] = static_cast<double>(header.pitch[2]);

    m_step = static_cast<long>(header.step);
    m_time = static_cast<double>(header.time);

    size_t allSz = m_size[0] * m_size[1] * m_size[2] * m_precision * m_numComp;
    m_data = new unsigned char[allSz];

    memcpy(m_data, data, sizeof(unsigned char) * allSz);
  }

  ~SphData() { DeleteBuffer(); }

  bool isSinglePrecision() const {
    return m_precision == sizeof(float) ? true : false;
  }

  bool GetSphHeaderTop(SphHeaderTop &htop) const {
    htop.svType = m_numComp == 1 ? 1 : 2;
    htop.dType = isSinglePrecision() == true ? 1 : 2;

    return true;
  }

  bool GetSphHeader(SphHeaderSingle &header) const {
    if (!isSinglePrecision()) {
      return false;
    }

    header.size[0] = static_cast<int>(m_size[0]);
    header.size[1] = static_cast<int>(m_size[1]);
    header.size[2] = static_cast<int>(m_size[2]);
    header.org[0] = static_cast<float>(m_org[0]);
    header.org[1] = static_cast<float>(m_org[1]);
    header.org[2] = static_cast<float>(m_org[2]);
    header.pitch[0] = static_cast<float>(m_pitch[0]);
    header.pitch[1] = static_cast<float>(m_pitch[1]);
    header.pitch[2] = static_cast<float>(m_pitch[2]);
    header.step = static_cast<int>(m_step);
    header.time = static_cast<float>(m_time);

    return true;
  }

  bool GetSphHeader(SphHeaderDouble &header) const {
    if (isSinglePrecision()) {
      return false;
    }

    header.size[0] = static_cast<long>(m_size[0]);
    header.size[1] = static_cast<long>(m_size[1]);
    header.size[2] = static_cast<long>(m_size[2]);
    header.org[0] = static_cast<double>(m_org[0]);
    header.org[1] = static_cast<double>(m_org[1]);
    header.org[2] = static_cast<double>(m_org[2]);
    header.pitch[0] = static_cast<double>(m_pitch[0]);
    header.pitch[1] = static_cast<double>(m_pitch[1]);
    header.pitch[2] = static_cast<double>(m_pitch[2]);
    header.step = static_cast<long>(m_step);
    header.time = static_cast<double>(m_time);

    return true;
  }

  const void *GetDataPtr() const { return m_data; }

  void print() {
    const char *precision[2] = {"single", "double"};
    const char *svType[2] = {"scalar", "vector"};

    int prcIdx = isSinglePrecision() == true ? 0 : 1;
    int svIdx = m_numComp == 1 ? 0 : 1;

    printf("PRECISION : %s\n", precision[prcIdx]);
    printf("SV_TYPE   : %s\n", svType[svIdx]);
    printf("SIZE      : (%15ld, %15ld, %15ld)\n", m_size[0], m_size[1],
           m_size[2]);
    printf("ORIGIN    : (%15.5lf, %15.5lf, %15.5lf)\n", m_org[0], m_org[1],
           m_org[2]);
    printf("PITCH     : (%15.5lf, %15.5lf, %15.5lf)\n", m_pitch[0], m_pitch[1],
           m_pitch[2]);
    printf("STEP      :  %15ld\n", m_step);
    printf("TIME      :  %15.5lf\n", m_time);
  }

  void printBuffer() {
    if (isSinglePrecision()) {
      _printBuffer<float>();
    } else {
      _printBuffer<double>();
    }
  }

private:
  void DeleteBuffer() { delete[] m_data; }

  template <typename T> void _printBuffer() {
    const T *buf = reinterpret_cast<const T *>(GetDataPtr());
    for (size_t z = 0; z < m_size[2]; z++) {
      for (size_t y = 0; y < m_size[1]; y++) {
        for (size_t x = 0; x < m_size[0]; x++) {

          printf(" %7.3f", buf[x + (y + z * m_size[1]) * m_size[0]]);
        }
        printf("\n");
      }
      printf("\n");
    }
    printf("\n");
  }

  size_t m_numComp;
  size_t m_precision;
  long m_size[3];
  double m_org[3];
  double m_pitch[3];
  long m_step;
  double m_time;

  unsigned char *m_data;
};

bool SaveSphFile(const SphData *sph, const std::string &filename, bool byteSwap) {

    FILE *fp = NULL;
    if ((fp = fopen(filename.c_str(), "wb")) == NULL) {
      fprintf(stderr, "cannot open file (%s) [%s:%d]\n", filename.c_str(),
              __FILE__, __LINE__);
      return false;
    }

    SphHeaderTop htop;
    sph->GetSphHeaderTop(htop);

    if (byteSwap) {
      swap4(&htop.SizeTop_0);
      swap4(&htop.svType);
      swap4(&htop.dType);
      swap4(&htop.SizeTop_1);
    }

    fwrite(&htop, sizeof(SphHeaderTop), 1, fp);

    size_t numComp = htop.svType == 1 ? 1 : 3;
    if (sph->isSinglePrecision()) {
        SphHeaderSingle header;
        sph->GetSphHeader(header);

        if (byteSwap) { 
            swap4((unsigned int*)&header.SizeSz_0);
            swap4((unsigned int*)&header.size[0]);
            swap4((unsigned int*)&header.size[1]);
            swap4((unsigned int*)&header.size[2]);
            swap4((unsigned int*)&header.SizeSz_1);

            swap4((unsigned int*)&header.SizeOrg_0);
            swap4((unsigned int*)&header.org[0]);
            swap4((unsigned int*)&header.org[1]);
            swap4((unsigned int*)&header.org[2]);
            swap4((unsigned int*)&header.SizeOrg_1);

            swap4((unsigned int*)&header.SizePitch_0);
            swap4((unsigned int*)&header.pitch[0]);
            swap4((unsigned int*)&header.pitch[1]);
            swap4((unsigned int*)&header.pitch[2]);
            swap4((unsigned int*)&header.SizePitch_1);

            swap4((unsigned int*)&header.SizeStep_0);
            swap4((unsigned int*)&header.step);
            swap4((unsigned int*)&header.time);
            swap4((unsigned int*)&header.SizeStep_1);
        }

        fwrite(&header, sizeof(SphHeaderSingle), 1, fp);

        size_t allSz = header.size[0] * header.size[1] * header.size[2] *
                       sizeof(float) * numComp;
        unsigned int Size = static_cast<unsigned int>(allSz);
        fwrite(&Size, sizeof(unsigned int), 1, fp);
        fwrite(sph->GetDataPtr(), sizeof(unsigned char), allSz, fp);
        fwrite(&Size, sizeof(unsigned int), 1, fp);
    } else {
        SphHeaderDouble header;
        sph->GetSphHeader(header);

        if (byteSwap) { 
            swap4((unsigned int*)&header.SizeSz_0);
            swap8((unsigned char*)&header.size[0]);
            swap8((unsigned char*)&header.size[1]);
            swap8((unsigned char*)&header.size[2]);
            swap4((unsigned int*)&header.SizeSz_1);

            swap4((unsigned int*)&header.SizeOrg_0);
            swap8((unsigned char*)&header.org[0]);
            swap8((unsigned char*)&header.org[1]);
            swap8((unsigned char*)&header.org[2]);
            swap4((unsigned int*)&header.SizeOrg_1);

            swap4((unsigned int*)&header.SizePitch_0);
            swap8((unsigned char*)&header.pitch[0]);
            swap8((unsigned char*)&header.pitch[1]);
            swap8((unsigned char*)&header.pitch[2]);
            swap4((unsigned int*)&header.SizePitch_1);

            swap4((unsigned int*)&header.SizeStep_0);
            swap8((unsigned char*)&header.step);
            swap8((unsigned char*)&header.time);
            swap4((unsigned int*)&header.SizeStep_1);
        }

        fwrite(&header, sizeof(SphHeaderDouble), 1, fp);

        size_t allSz = header.size[0] * header.size[1] * header.size[2] *
                       sizeof(double) * numComp;
        unsigned int Size = static_cast<unsigned int>(allSz);
        fwrite(&Size, sizeof(unsigned int), 1, fp);
        fwrite(sph->GetDataPtr(), sizeof(unsigned char), allSz, fp);
        fwrite(&Size, sizeof(unsigned int), 1, fp);
    }

    fclose(fp);

    return true;
}

bool IsBigEndian(void)
{
    union {
        unsigned int i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

} // namespace

SPHSaver::SPHSaver(){}
SPHSaver::~SPHSaver(){};

bool SPHSaver::Save(const char* filename){
    // @todo { double precision. }

    SphHeaderTop headerTop;
    SphHeaderSingle headerSingle;

    assert(m_volumeData->Component() == 1 || m_volumeData->Component() == 3);

    headerTop.svType = m_volumeData->Component() == 1 ? 1 : 2;
    headerTop.dType = 1; // float

    headerSingle.size[0] = static_cast<int>(m_volumeData->Width());
    headerSingle.size[1] = static_cast<int>(m_volumeData->Height());
    headerSingle.size[2] = static_cast<int>(m_volumeData->Depth());
    headerSingle.org[0]   = 0.0f;
    headerSingle.org[1]   = 0.0f;
    headerSingle.org[2]   = 0.0f;
    headerSingle.pitch[0] = 1.0f;
    headerSingle.pitch[1] = 1.0f;
    headerSingle.pitch[2] = 1.0f;
    headerSingle.step = 0;
    headerSingle.time = 0.0f;

    SphData sph(headerTop, headerSingle, m_volumeData->Buffer()->GetBuffer());

    std::string fname(filename);
    bool byteSwap = IsBigEndian();
    bool ret = SaveSphFile(&sph, fname, byteSwap);

	return ret;
}
