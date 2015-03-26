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
ポイントプリミティブが取得可能.

    local loader = PDMLoader()
    loader:Load('input') -- ベースファイル名を指定

## HDMLoader()

HDMファイルを読み込むローダークラス. hrender が HDMlib とリンクされているときのみ利用可能.
ボリュームプリミティブが取得可能.

    local loader = HDMLoader()
    loader:Load('cellid.bcm', 'data.bcm')

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

[TODO]

## PDMSaver()

PDM 形式で点群データを保存する. hrender が PDMlib とリンクされているときのみ利用可能.
現時点では timestep の変化する点群は保存出来ない(timestep=0 として書き出し)

    local saver = PDMSaver()
    saver:SetPointData(ball)
    saver:Save('output') -- ベースファイル名を指定


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


## PointToVolume()	

ポイントデータをボリュームデータに変換する.
ToVolume メソッドにはボリュームの解像度を指定する.
ボリュームの各セルの密度は, ポイントの個数に比例する.

    local p2v = PointToVolume()
    p2v:Create(pnt:PointData())
    
    p2v:ToVolume(128,128,128)


