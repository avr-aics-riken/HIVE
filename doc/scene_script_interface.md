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

## CDMLoader()

	/* TODO */

## PDMLoader()

	/* TODO */

## HDMLoader()

	/* TODO */

## BCMLoader()

	/* TODO */

## ImageLoader()

---------------------------------
# Saver

## ImageSaver()

## SPHSaver()

## PDMLoader()

---------------------------------
# Generator

## PrimitiveGenerator()

---------------------------------

# Analyzer

## PolygonAnalyzer

	local analyzer = PolygonAnalyzer()

## VolumeAnalyzer

	local analyzer = VolumeAnalyzer()

---------------------------------
# Network

## Connection

	local con = Connection()


---------------------------------
# Builder

## VolumeToVector()

## VolumeToMeshData()

## PointToVolume()	