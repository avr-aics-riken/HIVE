# volume rendering shader and ModuleSystem UI 

Volume rendering logic was ported from V-isio shaderVolren.

To use this example, please first replace assert paths(e.g. `/home/user/tools/HIVE/hrender/test/fvel_0000001470.sph`) in volren.json.

## UIs

* Transfer function(color map)
* Gradient map(monochrome version of transfer function)
* Lighting UI

## Rendering parameters

These are shading/rendering parameters(textures/uniforms) passed to `shader/volume_raymarch_rvr.frag`

* sampler2D tf_tex ... transfer function
* float tf_min
* float tf_max
* vec3 offset ... ボリュームの位置のオフセット
* float u_enableGradientMap ... gradient mapの使用の切り替え (0 or 1)
* sampler2D u_gradTex ... gradient map
* float u_samples ... レイマーチングのサンプル数
* float u_volumeDensity ... 粒子の濃度 [0, 1]
* float u_enableLighting ... ライティングの切り替え(0 or 1)
* vec3 u_light ... 光源の位置と強さ(posX, posY, posZ, intensity)
* vec3 u_attenuation ... 光の減衰(const, linear, quadratic)
* vec3 u_ambient ... 環境光成分 (R, G, B)
* vec3 u_specular ... スペキュラ (R, G, B)

EoL.

