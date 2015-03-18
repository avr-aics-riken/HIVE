/**
 * @file Camera.h
 * カメラ
 */
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "RenderObject.h"

class BufferImageData;

/**
 * カメラ.
 */
class Camera : public RenderObject {
    
public:
    /**
     * カメラ情報.
     */
    struct CameraInfo {
        float eye[3];
        float tar[3];
        float up [3];
        float fov;
    };
    
    /// クリアカラーを返す.
    const float*       GetClearColor() const;
    /// スクリーン幅を返す.
    int                GetScreenWidth() const;
    /// スクリーン高さを返す.
    int                GetScreenHeight() const;
    /// カメラ情報を返す.
    const CameraInfo*  GetCameraInfo() const;
    /// 出力ファイルパスを返す.
    const std::string& GetOutputFile() const;
    /// 画像バッファを返す.
    BufferImageData*   GetImageBuffer();
    /// 深度バッファを返す.
    BufferImageData*   GetDepthBuffer();

    /// 位置を返す
    VX::Math::vec3 GetPosition() const;
    /// ターゲットを返す
    VX::Math::vec3 GetTarget() const;
    /// アップベクトルを返す
    VX::Math::vec3 GetUp() const;
    
private:
    class Impl;
    Impl* m_imp;
    
protected:
    // Script Access Only
	Camera();
    ~Camera();
    
    /**
     * スクリーンサイズの設定.
     * @param width 幅
     * @param height 高さ
     */
    bool SetScreenSize(unsigned int width, unsigned int height);
    
    /**
     * 結果画像のファイルパスの設定.
     * @param filename ファイルパス
     */
	bool SetFilename(const std::string& outputfile);
    
    /**
     * 視線の設定.
     * @param eye_x 視点ベクトルx
     * @param eye_y 視点ベクトルy
     * @param eye_z 視点ベクトルz
     * @param tar_x ターゲットベクトルx
     * @param tar_y ターゲットベクトルy
     * @param tar_z ターゲットベクトルz
     * @param up_x アップベクトルx
     * @param up_y アップベクトルy
     * @param up_z アップベクトルz
     * @param fov 縦方向視野角
     */
	bool LookAt(float eye_x, float eye_y, float eye_z,
				float tar_x, float tar_y, float tar_z,
				float up_x,  float up_y,  float up_z,
				float fov);
    
    /**
     * クリアカラーの設定.
     * @param red 赤
     * @param green 緑
     * @param blue 青
     * @param alpha アルファ
     */
    bool ClearColor(float red, float green, float blue, float alpha);
    /// 視野角を返す
    float GetFov() const;
};

#endif //_CAMERA_H_
