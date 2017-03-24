/**
 * @file VisioColorMapLoader.cpp
 * Visio TranferFunction Lutデータローダー
 */
#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include "VisioColorMapLoader.h"
#include "Buffer.h"

/// コンストラクタ
VisioColorMapLoader::VisioColorMapLoader()
{
    m_rgba = new FloatBuffer();
}

/// デストラクタ
VisioColorMapLoader::~VisioColorMapLoader(){};

/// メンバクリア
void VisioColorMapLoader::Clear()
{
    m_rgba = new FloatBuffer();
}

/**
 * STLデータのロード
 * @param filename ファイルパス
 * @retval true 成功
 * @retval false 失敗
 */
bool VisioColorMapLoader::Load(const char* filename){
	Clear();
    
    std::ifstream ifs(filename);
    if (ifs.good()) {
        m_rgba->Create(256 * 4);
        std::string str;
        for (int line = 0; getline(ifs, str) && line < 257; ++line) {
            std::string token;
            std::istringstream stream(str);
            if (line == 0) {
                m_minValue = getline(stream, token, ' ') ? atof(token.c_str()) : 0.0f;
                m_maxValue = getline(stream, token, ' ') ? atof(token.c_str()) : 0.0f;
            } else {
                float r = getline(stream, token, ' ') ? atof(token.c_str()) : 0.0f;
                float g = getline(stream, token, ' ') ? atof(token.c_str()) : 0.0f;
                float b = getline(stream, token, ' ') ? atof(token.c_str()) : 0.0f;
                float a = getline(stream, token, ' ') ? atof(token.c_str()) : 0.0f;
                m_rgba->GetBuffer()[(line-1) * 4 + 0] = r;
                m_rgba->GetBuffer()[(line-1) * 4 + 1] = g;
                m_rgba->GetBuffer()[(line-1) * 4 + 2] = b;
                m_rgba->GetBuffer()[(line-1) * 4 + 3] = a;
            }
        }
        printf("min %f\n", m_minValue);
        printf("max %f\n", m_maxValue);
        return true;
    }
	return false;
}

int VisioColorMapLoader::RGBASize() const
{
    return m_rgba->GetNum();
}

float VisioColorMapLoader::RGBAValue(int pos) const
{
    if (pos < RGBASize()) {
        return m_rgba->GetBuffer()[pos];
    }
    return 0.0f;
}