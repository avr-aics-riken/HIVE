/**
 * @file VisioColorMapSaver.cpp
 * Visio TranferFunction Lutデータローダー
 */
#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include "VisioColorMapSaver.h"
#include "Buffer.h"

namespace {
    std::string to_string(float num) {
        std::ostringstream s;
        s << num;
        return s.str();
    }
}

/// コンストラクタ
VisioColorMapSaver::VisioColorMapSaver()
{
    m_rgba = new FloatBuffer();
    m_rgba->Create(256 * 4);
}

/// デストラクタ
VisioColorMapSaver::~VisioColorMapSaver(){};

/// メンバクリア
void VisioColorMapSaver::Clear()
{
    m_rgba = new FloatBuffer();
    m_rgba->Create(256 * 4);
}

/**
 * STLデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool VisioColorMapSaver::Save(const char* filename)
{
    std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
    if (ofs.good() && RGBASize() == 256 * 4) {

        ofs << to_string(m_minValue) << " " << to_string(m_maxValue) << std::endl;
        for (int i = 0; i < 256; ++i) {
            std::string r = to_string(m_rgba->GetBuffer()[i * 4 + 0]);
            std::string g = to_string(m_rgba->GetBuffer()[i * 4 + 1]);
            std::string b = to_string(m_rgba->GetBuffer()[i * 4 + 2]);
            std::string a = to_string(m_rgba->GetBuffer()[i * 4 + 3]);
            ofs << r << " " << g << " " << b << " " << a << std::endl;
        }
        return true;
    }
	return false;
}

int VisioColorMapSaver::RGBASize() const
{
    return m_rgba->GetNum();
}

bool VisioColorMapSaver::SetRGBAValue(int pos, float value)
{
    if (pos < RGBASize()) {
        m_rgba->GetBuffer()[pos] = value;
        return true;
    }
    return false;
}