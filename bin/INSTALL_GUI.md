
# krender for node.js
## 準備

以下のソフトウェアをインストールする.

- node.js (v0.10以上)のインストール
 
    以下のURLからインストールする: <http://nodejs.org>
    
- node-gypのインストール

    $npm install -g node-gyp
    
- libLSGLES.soのビルド

    LSGLをビルドする

- libkrender.aのビルド

    $cd app/KAnimRender/
    $make -f Makefile_lib

- krender.nodeをビルド

    $sh build.sh

- 必要なモジュールのインストール

    $cd app/KAnimRrender/krender_node
    $npm install
  
    
    
## サーバー実行

    $sh run.sh  (KVTools起動)

	もしくは
	
	$sh run_shader.sh  (シェーダエディタモードで起動)
	   
いずれの場合も 

    $open http://localhost:8082
    
としてサーバーにアクセスする.
   
## 設定

run.sh でKVToolsを実行しているが，シェルスクリプト内で以下の設定をおこなっている．

    $node kvtools.js
   
や

    $./krender scene.scn
        
として実行するには，以下を環境変数に設定する．


- LSGLのGLSL_COMPILERの設定
 
  export GLSL_COMPILER=/path/to/glslc

- libLSGLES.dylib のDYLD_LIBRARY_PATHの設定(MacOSXの場合)
  
  export DYLD_LIBRARY_PATH=/path/to/libLSGLESDirectry:$(DYLD_LIBRARY_PATH)

- libLSGLES.so のDYLD_LIBRARY_PATHの設定(Linuxの場合)
  
  export LD_LIBRARY_PATH=/path/to/libLSGLESDirectry:$(LD_LIBRARY_PATH)


- GLSLCのMESA_GLSLコンパイラのパス

  /path/to/glslc と同じディレクトリにあるglsl_config.pyを編集する
  例えばMacOSXの場合以下のように設定する
  
        mesa_glsl_compiler_path = "/path/to/glsl/bin/macosx64/glsl_compiler"
	    cxx_compiler_path = "clang++"
        additional_compiler_options = ["-O2"]


## 制約

- 通信ポートは8082と9000を利用している
- 同時セッション数は1セッションのみ対応