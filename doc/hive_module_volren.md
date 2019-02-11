# Hive Volrenモジュール

Volrenモジュールを使ったレンダリングについて記載する。

----

## lib/Util/Volren(モジュール)

 - Volrenモジュールは、VolumeModelを継承しており、RenderObjectとして扱うことができる。
 - シェーダーは、volume_raymarch_rvr.frag に対応している。
 - 初期値として、HIVE/ModuleSystem/examples/volren/doc/ReadMe.md に記載している、default値に対応している。
 - 以下の関数により volume_raymarch_rvr.frag 用のパラメータを設定することができる。
 
| 関数                         | Uniform変数名          | 引数                       |
| ---------------------------- |:----------------------------:|:---------------------------:|
| SetTransferFunctionTexture   | tf_tex                       |BufferImageData*             |
| SetTransferFunctionMin       | tf_min                       |float                        |
| SetTransferFunctionMax       | tf_max                       |float                        |
| EnableGradientMap            | u_enableGradientMap          |bool                         |
| SetGradientTexure            | u_gradTex                    |BufferImageData*             |
| EnableLighting               | u_enableLighting             |bool                         |
| SetLight                     | u_light                      |float, float, float          |
| SetAttenuation               | u_attenuation                |float, float, float          |
| SetAmbient                   | u_ambient                    |float, float, float          |
| SetSpecular                  | u_specular                   |float, float, float          |
| SetSamples                   | u_samples                    |int                          |
| SetOffset                    | u_offset                     |float, float, float          |

 - このモジュールを使用した場合、解放のために、renderを行った後に、以下のようにclearCache() を行う必要がある。

```
-- HIVE/hrender/test/render_volren.scnより一部抜粋。

local volume = LoadModule("Volren")
local volumedata = sph:VolumeData()
volume:Create(volumedata)
volume:SetShader('volume_raymarch_rvr.frag')

local analy = LoadModule("Analyzer").VolumeAnalyzer()
analy:Execute(volumedata)
print(analy:MinX())
print(analy:MaxX())

volume:SetTransferFunctionTexture(tftex:ImageData())
volume:SetTransferFunctionMin(analy:MinX())
volume:SetTransferFunctionMax(analy:MaxX())
volume:SetClampToEdge(true, true, true)
volume:SetTextureWrapping('tf_tex', true, true, true) -- CLAMP_TO_EDGE wrap mode for transfer function texture.

volume:EnableLighting(true);
volume:SetLight(0.0, 0.0, 50.0, 5.0);
volume:SetAttenuation(1.0, 0.0, 0.0);
volume:SetAmbient(0.2, 0.2, 0.22);
volume:SetSpecular(1.0, 1.0, 1.0);
volume:SetSamples(100.0)
volume:EnableGradientMap(false)
volume:SetOffset(0.0, 0.0, 0.0);

render {camera, volume}

clearCache() --for volren


## Volren(ノード)
 - HIVE/ModuleSystem/examples/volren/doc/ReadMe.md を参照


```
    




