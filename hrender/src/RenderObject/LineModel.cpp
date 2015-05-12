/**
 * @file LineModel.cpp
 * ラインモデル
 */
#include <string>
#include <stdio.h>
#include "LineModel.h"
#include "BufferLineData.h"

/**
 * ラインモデル
 */
class LineModel::Impl
{
public:
    
    /// コンストラクタ
    Impl()
    {
        m_line = 0;
        m_width = 1.0f;
    }
    
    /// デストラクタ
    ~Impl()
    {
        
    }

    /**
     * シェーダファイルパスの設定.
     * @param shaderfile シェーダファイルパス.
     */
    bool SetShader(const std::string& shaderfile)
    {
        m_shaderfile = shaderfile;
        return true;
    }
    
    /**
     * ライン幅の設定
     * @param width ライン幅
     */
    bool SetLineWidth(float width)
    {
        m_width = width;
        return true;
    }
    
    /**
     * ラインデータの生成
     * @param Linedata ラインデータ
     */
    bool Create(BufferLineData* Linedata)
    {
        if (!Linedata || Linedata->GetType() != BufferData::TYPE_LINE) {
			printf("Failed Create LineModel\n");
			return false;
		}
        m_line = Linedata;
        return true;
    }
    
    /**
     * ラインデータの取得
     * @return ラインデータ
     */
    BufferLineData* GetLine() const {
        return m_line;
    }
    
    /**
     * シェーダパスの取得
     * @return シェーダパス
     */
    const std::string& GetShader() const
    {
        return m_shaderfile;
    }

    /**
     * ライン幅の取得
     * @return ライン幅
     */
    float GetLineWidth() const
    {
        return m_width;
    }

private:
    RefPtr<BufferLineData> m_line;
    float m_width;
    std::string m_shaderfile;

};

/// コンストラクタ
LineModel::LineModel() : RenderObject(TYPE_LINE)
{
    m_imp = new Impl();
}

/// デストラクタ
LineModel::~LineModel()
{
    delete m_imp;
}

/**
 * シェーダファイルパスの設定.
 * @param shaderfile シェーダファイルパス.
 */
bool LineModel::SetShader(const std::string& shaderfile)
{
    return m_imp->SetShader(shaderfile);
}

/**
 * ライン幅の設定.
 * @param width ライン幅
 */
bool LineModel::SetLineWidth(float width)
{
    return m_imp->SetLineWidth(width);
}

/**
 * ラインデータの生成.
 * @param Linedata ラインデータ
 */
bool LineModel::Create(BufferLineData* Linedata)
{
    return m_imp->Create(Linedata);
}

/**
 * ラインデータの取得.
 * @return ラインデータ
 */
BufferLineData* LineModel::GetLine() const
{
    return m_imp->GetLine();
}

/**
 * シェーダパスの取得.
 * @return シェーダパス
 */
const std::string& LineModel::GetShader() const
{
    return m_imp->GetShader();
}

/**
 * ライン幅の取得.
 * @return ライン幅
 */
float LineModel::GetLineWidth() const
{
    return m_imp->GetLineWidth();
}

