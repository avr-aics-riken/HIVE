# HiveCore Interface

---------------------------------

任意のアプリケーションから、hrenderの機能をHiveCoreから利用することが可能である。
HiveCoreはSceneファイルまたは、SceneCommand(Luaのコマンド)を実行することができる。
実行に関する主な機能は SceneScriptクラスにまとめられている。
In-Situでのインメモリでデータの詳しい方法については、 insitu_interface.md を参照のこと。


利用出来る機能一覧は以下の通りである。

# API

## SceneScript Class

    class SceneScript
    {
    public:
        SceneScript();
        ~SceneScript();
    
        // for batch execution
        bool Execute(const char* luascript, const std::vector<std::string>& sceneargs);
        bool ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs);
  
        // for interactive execution
        void Begin(const std::vector<std::string>& sceneargs);
        bool Command(const char* scenecommand);
        void End();
    
        // in-memory-data binding
        bool CreateMemoryData(const char* dataId);
        bool IsMemoryData(const char* dataId);
	    bool DeleteMemoryData(const char* dataId);
	    bool DeleteAllMemoryData();
	    bool SetMemoryData(const char* dataId, const void* memptr, const size_t memsize);
	    size_t GetMemoryDataSize(const char* dataId) const;
	    const void* GetMemoryDataPointer(const char* dataId) const;
	    int GetMemoryDataNum() const;
	    const char* GetMemoryDataId(int i) const;
	    
	    class Impl; // to be public (for internal)
    private:
	    Impl* m_imp;
    };

## Execute Method

### bool Execute(const char* luascript, const std::vector<std::string>& sceneargs);

シーンファイル相当の文字列を実行する。実行が完了するとScript内の変数は破棄される。

- luascript : シーンファイルの中身(luaプログラム)
- sceneargs : luaプログラムに渡す引数(luaプログラムでは ```arg``` 変数でアクセス可能)
      
### bool ExecuteFile(const char* scenefile, const std::vector<std::string>& sceneargs);

シーンファイルを実行する。

- scenefile : シーンファイルのファイルパス
- sceneargs : luaプログラムに渡す引数(luaプログラムでは ```arg``` 変数でアクセス可能)

### void Begin(const std::vector<std::string>& sceneargs);

インタラクティブなシーンコマンドの実行を開始する。コマンドの実行は Command メソッドを参照。

- sceneargs : luaプログラムに渡す引数(luaプログラムでは ```arg``` 変数でアクセス可能)

### void End();

インタラクティブなシーンコマンドの実行を終了する。実行の開始は Begin メソッドを参照。

### bool Command(const char* scenecommand);

インタラクティブなシーンのコマンドを実行する。
変数のスコープは End メソッドが呼び出されるまで有効。

- scenecommand : シーンのコマンド(Luaプログラム

## In Memory Data Method

### bool CreateMemoryData(const char* dataId);

インメモリデータを定義する。

- dataId : 任意の識別文字列

### bool IsMemoryData(const char* dataId);

インメモリデータが定義されているか確認する。

- dataId : 任意の識別文字列


### bool DeleteMemoryData(const char* dataId);

インメモリデータの定義を削除する。

- dataId : 任意の識別文字列


### bool DeleteAllMemoryData();

すべてのインメモリデータの定義を削除する。

### bool SetMemoryData(const char* dataId, const void* memptr, const size_t memsize);

指定されたインメモリデータ定義に、メモリポインタの情報とデータサイズの情報をセットする。

- dataId : 任意の識別文字列


### size_t GetMemoryDataSize(const char* dataId) const;

指定されたインメモリデータ定義に、セットされたデータサイズの情報を取得する。

- dataId : 任意の識別文字列


### const void* GetMemoryDataPointer(const char* dataId) const;

指定されたインメモリデータ定義に、セットされたメモリポインタの情報を取得する。

- dataId : 任意の識別文字列


### int GetMemoryDataNum() const;

定義されているインメモリデータの数を取得する。

### const char* GetMemoryDataId(int i) const;

指定番号の定義されているインメモリデータの定義名を取得する。


## Example

サンプルコードは HiveCore/test/simpletest.cpp, HiveCore/test/solversample.cpp を参照のこと

 


