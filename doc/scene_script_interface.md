# Scene Script Interface

---------------------------------

# Basic function

## render()
指定されたレンダーオブジェクトでレンダリングを行う

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
レンダラが動作するプラットフォームのエンディアンのタイプを取得する
取得可能な値は以下のいずれかの文字列。

- little
- big

# Generator
## PrimitiveGenerator

プリミティブを作成するためのジェネレータークラス

---------------------------------

# Loader
## OBJLoader()

Objファイルを読み込むローダークラス

    local loader = OBJLoader()
    loader:Load('bunny.obj')
    

## STLLoader()

STLファイルを読み込むローダークラス

    local loader = STLLoader()
    loader:Load('bunny.stl')

## SPHLoader()

SPHファイルを読み込むローダークラス

    local loader = SPHLoader()
    loader:Load('data.sph')

## VOLLoader()

VOLファイルを読み込むローダークラス

    local loader = VOLLoader()
    loader:Load('data.vol')

## PDBLoader()

PDB(Protein Data Bank)ファイルを読み込むローダークラス.
第二引数で bond(line) を生成するかどうかを指定する.
デフォルトは false.

    local loader = PDBLoader()
    loader:Load('input.pdb')
    loader:Load('input.pdb', true) -- generate bond


## CDMLoader()

CDMファイルを読み込むローダークラス. hrender が CDMlib とリンクされているときのみ利用可能.
ボリュームプリミティブが取得可能.

    local loader = CDMLoader()
    loader:Load('input.dfi')

## PDMLoader()

PDMファイルを読み込むローダークラス. hrender が PDMlib とリンクされているときのみ利用可能.
Load() でファイル名と timestep 番号を指定し, 該当の timestep のときのデータをロードする.
ファイル名に相対パスが含まれていてはならない.

    local loader = PDMLoader()
    local timestep = 0
    loader:Load('input.dfi', timestep)

## HDMLoader()

HDMファイルを読み込むローダークラス. hrender が HDMlib とリンクされているときのみ利用可能.
Sparse ボリュームプリミティブが取得可能.
HDMlib の制約により, 1 シーン内で 1 HDMlib 形式のファイルしか読む事ができない.
(Init() を呼べるのは一回のみ)

    local loader = HDMLoader()
    loader:Init('cellid.bcm', 'data.bcm')

    # 指定されたフィールドと timestep の sparse volume プリミティブを取得
    # fieldname, fieldtype, components, stepNo
    local svolumedata = loader:LoadField('Tmp32', 'Float32', 1, 0)

## BCMLoader()

BDMファイルを読み込むローダークラス. hrender が BCMTools とリンクされているときのみ利用可能.
ボリュームプリミティブが取得可能.

    local loader = BCMLoader()
    loader:Load('cellid.bcm', 'data.bcm')

## ImageLoader()

[TODO]

---------------------------------
# Saver

## ImageSaver()

[TODO]

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
# Generator

## PrimitiveGenerator()

[TODO]

---------------------------------

# Analyzer

## PolygonAnalyzer

	local analyzer = PolygonAnalyzer()
    
[TODO]

## VolumeAnalyzer

	local analyzer = VolumeAnalyzer()

[TODO]

---------------------------------
# Network

## Connection

	local con = Connection()

[TODO]

---------------------------------
# Builder

## VolumeToVector()

[TODO]

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


