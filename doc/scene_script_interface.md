# Scene Script Interface

---------------------------------

# Basic function

## render()
指定されたRenderObjectでレンダリングを行う

     render({renderobject1, renderobject2, renderobject3 })

第2引数にレンダリング中のコールバック関数を指定可能

     render({renderobject1, renderobject2, renderobject3 }, funciton (progress) 
     	print('Progress = ' .. progress)
     end)



## clearCache()
レンダラにキャッシュされた内部データを削除する

## mpiMode()

MPIモードで起動しているかどうかを取得する。
取得可能な値は以下のいずれかのブール値。

- true
- false


## mpiRank()

MPIモードで起動している場合、MPIのランク番号を取得する。

## platform()
レンダラが動作するプラットフォーム環境を取得する。取得可能な値は以下のいずれかの文字列。

- Windows
- MacOSX
- Linux

## dllExtenstion()	
レンダラが動作するプラットフォームのダイナミックリンクライブラリの拡張子を取得する
取得可能な値は以下のいずれかの文字列。

- dll
- so

## endian()	
レンダラが動作するプラットフォームのエンディアンのタイプを取得する。
取得可能な値は以下のいずれかの文字列。

- little - リトルエンディアン
- big - ビッグエンディアン

## screenParallelRendering(enable)
MPIモードでのみ有効。SURFACEの画面分割レンダリング機能を有効にする。以下のいずれかの値が設定可能

- true - 有効
- false - 無効
 
初期値はfalse。



--------------

#RenderObject

HIVEではレンダリング対象となるオブジェクトはRenderObjectと呼ばれる.
RenderObjectは以下の種類がある。

- Camera
- PolygonModel
- VolumeModel
- PointModel
- LineMode
- SparseVolumeModel
- TetraModel
- VectorModel

レンダリングし、画像を保存するには１つ以上のCameraが必要となる。
以下は何もないシーンをレンダリングし、image.jpgに保存する例


     local camera = Camera()
     camera:SetScreenSize(1024, 1024)
     camera:SetFilename('image.jpg')
     camera:LookAt(
     	0,0,300,
     	0,0,0,
     	0,1,0,
     	60
     }
     render {camera}


RenderObjectは共通のインターフェースを持つ
      
- オブジェクトのトランスフォームの平行移動値を設定
 
      SetTranslate(translate_x, translate_y, translate_z)

- オブジェクトのトランスフォームの回転値を設定
 
      SetRotate(rotate_x, translate_y, translate_z)

- オブジェクトのトランスフォームの拡大値を設定
 
	  SetScale(scale_x, scale_y, scale_z)
	  
- オブジェクトのトランスフォームマトリックスを直接設定する
 
	  SetTransformMatrix(LuaTable matrix)
	
- オブジェクトのシェーダのUniform変数(vec4)の値を設定
 
	  SetVec4(uniform_name, x, y, z, w)

- オブジェクトのシェーダのUniform変数(vec3)の値を設定

	  SetVec3(uniform_name, x, y, z)

- オブジェクトのシェーダのUniform変数(vec2)の値を設定

	  SetVec2(uniform_name, x, y)

- オブジェクトのシェーダのUniform変数(float)の値を設定

	  SetFloat(uniform_name, x)

- オブジェクトのシェーダのUniform変数(sampler2D)の値を設定

	  SetTexture(uniform_name, texture)
	  
- テクスチャのフィルタを設定

	  boolean SetTextureFiltering(string, boolean)
	  
- テクスチャのラップ処理を設定

      boolean SetTextureWrapping(string, boolean, boolean, boolean)
      
- オブジェクトの平行移動量を取得

      LuaTable GetTranslate()
      
- オブジェクトの回転量を取得

      LuaTable GetRotate()
      
- オブジェクトの拡大量を取得

      LuaTable GetScale()
      
- シェーダのUniform変数(Vec4)の値をすべて取得

      LuaTable GetVec4Table()

- シェーダのUniform変数(Vec3)の値をすべて取得      

      LuaTable GetVec3Table()
      
- シェーダのUniform変数(Vec2)の値をすべて取得

      LuaTable GetVec2Table()
      
- シェーダのUniform変数(float)の値をすべて取得

      LuaTable GetFloatTable()
      
- シェーダのUniform変数(sampler2D)の値をすべて取得

      LuaTable GetTextureTable()
      
- シェーダのUniform変数(Vec4)の指定変数の値を取得

      LuaTable GetVec4(string)

- シェーダのUniform変数(Vec3)の指定変数の値を取得      

      LuaTable GetVec3(string)
      
- シェーダのUniform変数(Vec2)の指定変数の値を取得

      LuaTable GetVec2(string)
      
- シェーダのUniform変数(float)の指定変数の値を取得

      float GetFloat(string)
      
- シェーダのUniform変数(sampler2D)の指定変数の値を取得

      BufferImageData GetTexture(string)

- 拡張バッファを設定

      Number AddExtraBuffer(string, BufferExtraData)
      
- 拡張バッファを取得

	  Number RemoveExtraBuffer(string)
	
	 
	  

## Camera

任意視点でレンダリングし、画像に保存するためのオブジェクト
複数のカメラの同時設定が可能.
以下は複数のカメラの視点を設定し、それぞれの背景色を変更する例


     local camera1 = Camera()
     camera1:SetScreenSize(1024, 1024)
     camera1:ClearColor(1,0,0,1);
     camera1:SetFilename('render_camera2_image1.jpg')
     camera1:LookAt(
     	0,0,300,
     	0,0,0,
     	0,1,0,
     	60
     )

     local camera2 = Camera()
     camera2:SetScreenSize(512, 512)
     camera2:ClearColor(0,1,0,1);
     camera2:SetFilename('render_camera2_image2.jpg')
     camera2:LookAt(
     	0,300,300,
     	0,0,0,
     	0,1,0,
     	45
     )
     render {camera1, camera2}


インタフェース一覧


- スクリーンサイズの設定.
  * w 幅
  * h 高さ

        boolean SetScreenSize(w, h)
    
- 結果画像のファイルパスの設定.
  * filename ファイルパス

        boolean SetFilename(filename)
    

- 結果デプス画像のファイルパスの設定.
  * filename ファイルパス

        boolean SetDepthFilename(filename)
    

- 視線の設定.
  * eye_x 視点ベクトルx
  * eye_y 視点ベクトルy
  * eye_z 視点ベクトルz
  * tar_x ターゲットベクトルx
  * tar_y ターゲットベクトルy
  * tar_z ターゲットベクトルz
  * up_x アップベクトルx
  * up_y アップベクトルy
  * up_z アップベクトルz
  * fov 縦方向視野角

         boolean LookAt(eye_x, eye_y, eye_z,
                        tar_x, tar_y, tar_z,
                        up_x,  up_y,  up_z,
                        fov)
    

- クリアカラーの設定.
  * red 赤 (0.0 ~ 1.0)
  * green 緑 (0.0 ~ 1.0)
  * blue 青 (0.0 ~ 1.0)
  * alpha アルファ (0.0 ~ 1.0)

        boolean ClearColor(red, green, blue, alpha)
    
- 位置を返す

        Table GetPosition()
    
- ターゲットを返す

        Table GetTarget()
    
- アップベクトルを返す

        Table GetUp()
    
- 視野角を返す

        Number GetFov()
    
- スクリーン幅を返す.

        Number GetScreenWidth()
    
- スクリーン高さを返す.

        Number GetScreenHeight()
        
- 出力ファイルパスを返す.

        string GetOutputFile()
    
- 出力デプスファイルパスを返す.

        string GetDepthOutputFile()
    
- 画像バッファを返す.

        BufferImageData GetImageBuffer()
   	
- 深度バッファを返す.
    
        BufferImageData GetDepthBuffer()


## PolygonModel

ポリゴンをレンダリングするためのオブジェクト

    local model = PolygonModel()

インターフェース一覧

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- meshデータからポリゴンモデルを作成する

        boolean Create(BufferMeshData)


## VolumeModel

ボリュームデータをレンダリングするためのオブジェクト

    local model = VolumeModel()

インターフェース一覧

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- volumeデータからポリゴンモデルを作成する

        boolean Create(BufferVolumeData)
        
- ボリュームサンプリングのエッジのクランプ処理を設定する
  * xclamp Z軸方向のクランプ処理をするかどうか
  * yclamp Y軸方向のクランプ処理をするかどうか
  * zclamp Z軸方向のクランプ処理をするかどうか
  
 
        bool SetClampToEdge(xclamp, yclamp, zclamp)
        
  


## PointModel

ポイントデータをレンダリングするためのオブジェクト

    local model = PointModel()

インターフェース一覧

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- pointデータからポリゴンモデルを作成する

        boolean Create(BufferPointData)

## LineMode

ラインデータをレンダリングするためのオブジェクト

    local model = LineModel()

インターフェース一覧

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- lineデータからポリゴンモデルを作成する

        boolean Create(BufferLineData)

- lineの太さを設定する

        boolean SetLineWidth(Number)
        


## SparseVolumeModel

スパースボリュームデータをレンダリングするためのオブジェクト

    local model = SparseVolumeModel()

インターフェース一覧

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- volumeデータからポリゴンモデルを作成する

        boolean Create(BufferSparseVolumeData)
        
- ボリュームサンプリングのエッジのクランプ処理を設定する
  * xclamp Z軸方向のクランプ処理をするかどうか
  * yclamp Y軸方向のクランプ処理をするかどうか
  * zclamp Z軸方向のクランプ処理をするかどうか
  
 
        bool SetClampToEdge(xclamp, yclamp, zclamp)
        
  



## TetraModel

テトラ構造のデータをレンダリングするためのオブジェクト

     local model = TetraModel()

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- tetraデータからポリゴンモデルを作成する

        boolean Create(BufferTetraData)


## VectorModel

ベクターアローデータをレンダリングするためのオブジェクト

     local model = VectorModel()

- shaderファイルを設定する
  * shaderfile シェーダファイルパス

        boolean SetShader(shaderfile)

- shaderファイルを取得する

        string GetShader()

- vectorデータからポリゴンモデルを作成する

        boolean Create(BufferVectorData)
        
- lineの太さを設定する

        boolean SetLineWidth(Number)
        
- 矢印の大きさを設定する

        boolean SetArrowSize(Number)

- 矢印の長さを設定する

        boolean SetLengthScale(Number)



-----------------

# Generator
## PrimitiveGenerator

プリミティブを作成するためのジェネレータークラス.

- Quad(四角ポリゴン)を生成する例

      local gen = PrimitiveGenerator()
      local width  = 2
      local height = 1.5
      local model = PolygonModel()
      local meshdata = gen:Quad(width, height)
      model:Create(meshdata)

- Cube(立方体)を生成する例

      local gen = PrimitiveGenerator()
      local width  = 5
      local height = 5
      local depth  = 5
      local model = PolygonModel()
      local meshdata = gen:Cube(width, height, depth)
      model:Create(meshdata)


- teapotを生成する例
 
	  local gen = PrimitiveGenerator()
	  local model = PolygonModel()
	  local meshdata = gen:Teapot(3.0)
	  model:Create(meshdata)

- sphereを生成する例


      local gen = PrimitiveGenerator()
      local radius = 2
      local model = PointModel()
      local spheredata = gen:Sphere(radius)
      model:Create(spheredata)

- テーブルから複数のSphereを生成する例
  
      local gen = PrimitiveGenerator()
      local model = PointModel()
      local spheredata = gen:SphereList(spherelist_table, num, radius)
      model:Create(spheredata)

     
- テーブルから複数のLine(cylinder形状)を生成する例

      local gen = PrimitiveGenerator()
      local model = PolygonModel()
      local linedata = gen:LineList(linelist_table, num, radius)
      model:Create(linedata)

- テーブルから複数の３角形形状を生成する例

      local gen = PrimitiveGenerator()
      local model = PolygonModel()
      local tridata = gen:TriangleList(trianglelist_table, num)
      model:Create(tridata)
 


---------------------------------

# Loader
## OBJLoader()

Objファイルを読み込むローダークラス.
以下はObjファイルを読み込みPolygonModelを作成し、データをセットする例

    local loader = OBJLoader()
    loader:Load('bunny.obj')
    local model = PolygonModel()
    local meshdata = loader:MeshData()
    model:Create(meshdata)

## STLLoader()

STLファイルを読み込むローダークラス
以下はSTLファイルを読み込みPolygonModelを作成し、データをセットする例

    local loader = STLLoader()
    loader:Load('bunny.stl')
    local model = PolygonModel()
    local meshdata = loader:MeshData()
    model:Create(meshdata)

## SPHLoader()

SPHファイルを読み込むローダークラス
STLファイルはバイナリ形式のみ. 
ビッグエンディアン・リトルエンディアンの判定は自動で行われる.
以下はSPHファイルを読み込みVolumeModelを作成し、データをセットする例

    local loader = SPHLoader()
    loader:Load('data.sph')
    local volume = VolumeModel()
    local volumedata = loader:VolumeData()
    volume:Create(volumedata)

## VOLLoader()

VOLファイルを読み込むローダークラス
以下はVOLファイルを読み込みVolumeModelを作成し、データをセットする例

    local loader = VOLLoader()
    loader:Load('data.vol')
    local volume = VolumeModel()
    local volumedata = loader:VolumeData()
    volume:Create(volumedata)

## RawVolumeLoader()

RAW 形式のボリュームデータを読み込む. 他のボリュームツールから出力されたデータなどを読み込むときに利用する.
(e.g. http://ospray.github.io )

    local loader = RawVolumeLoader()
    -- width, height, depth, component, type
    loader:Load('input-256x256x256-float.raw', 256, 256, 256, 1, 'float')
    local volume = VolumeModel()
    local volumedata = loader:VolumeData()
    volume:Create(volumedata)
    
type には現状 'float' のみ指定可能.

[render_rawvolume.scn](hrender/test/render_rawvolume.scn) 参考例

## PDBLoader()

PDB(Protein Data Bank)ファイルを読み込むローダークラス.
第二引数で bond(line) を生成するかどうかを指定する.
デフォルトは false.

    local loader = PDBLoader()
    loader:Load('input.pdb')
    loader:Load('input.pdb', true) -- generate bond

    local ballModel = pdb:BallData() -- Atom をポイントプリミティブとして取得
    local stickModel = pdb:StickData() -- Bond 情報を Line プリミティブとして取得(generate bond で bond 情報を構築している場合のみ取得可能.

[render_pdb.scn](hrender/test/render_pdb.scn) 参考例

## CDMLoader()

CDMファイルを読み込むローダークラス. hrender が CDMlib とリンクされているときのみ利用可能.
(一様/非一様)ボリュームプリミティブが取得可能.
データが非一様で読み込まれるかは .dfi ファイルでの指定に従う.
timeStepIndex には 0 からのインデックス番号を指定する(timeStep の時刻ではないことに注意. 省略可能. デフォルトは 0)

    local loader = CDMLoader()
    local timeStepIndex = 0
    local virtualCellSize = 2
    loader:Load('input.dfi', timeStepIndex)
    local volumeData = loader:VolumeData() -- volume プリミティブを取得

[render_cdm.scn](hrender/test/render_cdm.scn) 参考例
[render_cdm_nonuni.scn](hrender/test/render_cdm_nonuni.scn) 参考例(非一様)

## PDMLoader()

PDMファイルを読み込むローダークラス. hrender が PDMlib とリンクされているときのみ利用可能.
Load() でファイル名と timestep 番号を指定し, 該当の timestep のときのデータをロードする.
ファイル名に相対パスが含まれていてはならない.

    local loader = PDMLoader()
    local timestep = 0
    loader:Load('input.dfi', timestep)
    
    -- 点データのコンテナ名と, 点の半径を指定(半径は省略可能. 省略時は 1.0 に設定)
    local pointData = pdm:PointData('Coordinate', 0.2)

    -- 任意形式のコンテナデータを取得.
    -- コンテナ名に対するデータ形式(float, vec3, etc)はユーザが既知とする.
    local extraData = pdm:ExtraData('velocity')

[render_pdm.scn](hrender/test/render_pdm.scn) 参考例

## HDMLoader()

HDMファイルを読み込むローダークラス. hrender が HDMlib とリンクされているときのみ利用可能.
Sparse ボリュームプリミティブが取得可能.
HDMlib の制約により, 1 シーン内で 1 HDMlib 形式のファイルしか読む事ができない.
(Init() を呼べるのは .scn 内で一回のみ)

    local loader = HDMLoader()
    loader:Init('cellid.bcm', 'data.bcm')

    # 指定されたフィールドと timestep の sparse volume プリミティブを取得
    # フールド名, 型, コンポーネント数はユーザがあらかじめ .bcm ファイルなどから
    # 情報を取得して既知であるとする.
    # fieldname, fieldtype, components(1=scalar, 3=vector), timeStepNo, virtualCellSize
    # timeStepNo と virtualCelLSize は省略可能. デフォルトではそれぞれ 0, 2 となる.
    local svolumedata = loader:LoadField('Tmp32', 'Float32', 1, 0, 2)

[render_hdm.scn](hrender/test/render_hdm.scn) 参考例

## UDMLoader()

UDMファイルを読み込むローダークラス. hrender が UDMlib とリンクされているときのみ利用可能.
非構造プリミティブ(三角形, テトラ, 六面体)が取得可能.
六面体はポリゴンへと変換される.

    local loader = UDMLoader()
    loader:Load('index.dfi')

    local tetra = loader:TetraData() -- テトラプリミティブを取得

[render_udm_tetra.scn](hrender/test/render_udm_tetra.scn) 参考例

## ImageLoader()

画像データを読み込む.
オブジェクトにテクスチャとして画像を設定したい場合は以下のように行う.


    local loader = ImageLoader()
    local isloaded = loader:Load("image.jpg")
    local img = loader:ImageData()
    model:SetTexture('mytex', img) --> uniform sampler2D mytex0;

---------------------------------
# Saver

## ImageSaver()

イメージデータを保存できる.
以下は読み込んだ画像データを別形式で保存する例.

    local loader = ImageLoader()
	loader:Load("input_image.hdr")

	local saver = ImageSaver()
	saver:Save("output_image.jpg", loader:ImageData())


## SPHSaver()

SPH 形式でボリュームデータを保存する.
float 形式の SPH の書き出しのみに対応する.

    local sphSaver = SPHSaver()
    sphSaver:SetVolumeDatta(volumedata)
    sphSaver:Save('output.sph')


## RawVolumeSaver()

RAW 形式でボリュームデータを保存する. 他のボリュームデータビューアなどで閲覧するときに利用する.
データはメモリ上の生の形式で(endian 変換は行われない), `width * height * depth * sizeof(type)` bytes のデータが保存される.
Save() にはファイル名と, footer を付けるかどうか(HIVE 独自形式)を指定する.

    local volSaver = RawVolumeSaver()
    local appendFooter = false
    volSaver:SetVolumeData(volumedata)
    volSaver:Save("output.raw", appendFooter)

footer を付ける場合, 20 bytes(4 int x 5)のデータ `(width, height, depth, type, components)` がフッターとしてファイルの最後に追記される.

## PDMSaver()

PDM 形式で点群データを保存する. hrender が PDMlib とリンクされているときのみ利用可能.
現時点では timestep の変化する点群は保存出来ない(timestep=0 として書き出し)

    local saver = PDMSaver()
    saver:SetPointData(ball)
    saver:Save('output') -- ベースファイル名を指定.


---------------------------------

# Analyzer

## PolygonAnalyzer

PolygonModelから情報を取得する

	local analyzer = PolygonAnalyzer()


インターフェース一覧
    
- 解析結果の最小X座標を取得
    
        Number MinX()

- 解析結果の最大Y座標を取得
       
        Number MinY()
    
- 解析結果の最小Z座標を取得

        Number MinZ()

- 解析結果の最大X座標を取得

        Number MaxX()

- 解析結果の最大Y座標を取得

        Number MaxY()
    
- 解析結果の最大Z座標を取得

        Number MaxZ()

- 解析を実行する

        boolean Execute(PolygonModel)


## VolumeAnalyzer

VolumeModelから情報を取得する

	local analyzer = VolumeAnalyzer()

インターフェース一覧

- 解析結果の最小X座標を取得
    
        Number MinX()

- 解析結果の最大Y座標を取得
       
        Number MinY()
    
- 解析結果の最小Z座標を取得

        Number MinZ()

- 解析結果の最大X座標を取得

        Number MaxX()

- 解析結果の最大Y座標を取得

        Number MaxY()
    
- 解析結果の最大Z座標を取得

        Number MaxZ()

- 解析結果のヒストグラムを取得する

        Table GetHistgram()

- 解析結果の指定値範囲のヒストグラムを取得する

        Table GetHistgramInRange(VolumeModel, Number, Number)

- 解析を実行する

        boolean Execute(VolumeModel)
	

---------------------------------
# Network

## Connection

ネットワークの接続処理を行う。

	local con = Connection()

インターフェース一覧

- 接続する

        boolean Connect(string)

- テキストを送信する

        boolean SendText(string)
    
- JSONを送信する
    
        boolean SendJSON(string)

- バイナリデータを送信する
  * buffer 送信データ
  * size 送信データサイズ 
    
        boolean SendBinary(buffer, size)

- 画像データを送信する
    
        boolean SendImage(string)

- データを受信する
    
        string  Recv()

- 接続の状態を取得する
    
        string  GetState()

- タイムアウト時間を設定する
    
        boolean SetTimeout(Number)

- 切断する
    
        boolean Close()



---------------------------------
# Builder

## VolumeToVector()

ボリュームデータからベクトルアローに変換する.
ベクトルアローの間隔を設定し、ベクトルアローを生成する

     local vtv = VolumeToVector()
	 vtv:DivideNumber(divX, divY, divZ)
	 vtv:Create(volumedata)
	 local vectordata = vtv:VectorData()
	 local vm = VectorModel();
	 vm:Create(vectordata);
	
## VolumeToMeshData()

marching cubes 法を用いて, ボリュームデータをメッシュ(triangle)に変換する. 
等値面を生成する基準となる値(isovalue)をセットしてからメッシュを生成する.

    local surfacer = VolumeToMeshData()
    local isovalue = 0.0005
    surfacer:Create(volumedata)
    surfacer:SetIsoValue(isovalue)
    surfacer:IsoSurface()
    local isosurface = surfacer:MeshData()

入力のボリュームデータは scalar ボリューム(`Component() = 1`) でなければならない.
vector ボリュームは一度 VolumeFilter や FloatsToFloat を介して scalar ボリュームに変換する必要がある.

## PointToVolume()	

ポイントデータをボリュームデータに変換する.
ToVolume メソッドにはボリュームの解像度を指定する.
ボリュームの各セルの密度は, ポイントの個数に比例する.

    local p2v = PointToVolume()
    p2v:Create(pnt:PointData())
    
    p2v:ToVolume(128,128,128)

## SparseVolumeToVolume()	

疎ボリュームを(一様)ボリュームデータに変換する.
Create メソッドにはリサンプリングレートを指定する.
たとえば 0.25 だと疎ボリュームの元データの 1/4 の解像度で一様ボリュームを生成する.
2.0 だと 2 倍となる.

    local s2v = SparseVolumeToVolume()
    local resampleRate = 0.25
    s2v:Create(volumedata, resampleRate)
    

## VolumeFilter()

ボリュームデータに対してフィルタ処理を行う. 疎ボリュームに対しては適用できない.
ボリュームの型は `float` のみとする.

`Expr` では C 言語でのフィルタコードを走らせて処理を行うことができる.

    local filter = VolumeFilter()
    filter:Laplacian(volumedata)    -- Laplacian を計算する
    filter:Norm(volumedata)         -- Norm を計算する

    filter:VolumeData() -- 処理されたボリュームデータを取得する
    
    -- カスタムのフィルタを実行する(C コンパイラが必要)
    -- 最大で 4 つのボリュームデータを入力として指定できる. ボリュームはすべて同じ大きさとする.
    -- 利用しない場合は nil を指定する.
    -- targetNumberOfComonents には生成するボリュームのコンポーネント数を指定する.
    -- source にはフィルタ文字列を指定する.
    filter:SetCompoleOption(compiler, compileOpts) -- コンパイラ名とコンパイルオプションを指定する.
    filter:Expr(vol0, vol1, vol2, vol4, targetNumberOfComonents, source)

カスタムフィルタでは以下の変数とマクロを利用できる.

    float* dst : destination ボリュームへのポインタ
    const flooat* src0, src1, src2, src3 : 入力ボリュームへのポインタ
    size_t x, y, z : ボクセル位置
    size_t width, height, depth : ボリュームの大きさ
    IDX : ボクセル位置からメモリアドレス(配列インデックス)を算出するマクロ
          引数は number of components, x, y, z, current component index, w, h, d となる.
   
    -- example
    local filterCode = [[
      dst[IDX(1,x,y,z,0,width,height,depth)] = src0[IDX(1,x,y,z,0,width,height,depth)];
    ]]

    filter:SetCompoleOption('gcc', '-O2')



