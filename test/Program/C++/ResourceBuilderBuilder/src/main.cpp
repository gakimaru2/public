//--------------------------------------------------------------------------------
//リソースビルダー処理テスト

#include <stdio.h>
#include <stdlib.h>
#include <atomic>

//--------------------------------------------------------------------------------
//【サンプル用ダミー】型宣言と共通関数

//CRC32型
typedef unsigned int CRC32;

//CRC算出関数
CRC32 CRC(const char* str)
{
	return 123;//ダミー
}

//--------------------------------------------------------------------------------
//【サンプル用ダミー】リソースマネージャクラス

//シングルトンアクセスクラス（ダミー）
template<class T>
class CSingletonUsing
{
public:
	//オペレータ
	T* operator->(){ return &m_instance; }//アロー演算子
public:
	//コンストラクタ
	CSingletonUsing(const char* name)
	{}
private:
	//フィールド
	static T m_instance;//シングルトンインスタンス
};

#define SINGLETON_INSTANCE(T) \
	T CSingletonUsing<T>::m_instance;

//--------------------------------------------------------------------------------
//リソースマネージャ共通

//----------------------------------------
//定数

//リソース属性
enum E_RES_ATTR
{
	RES_ATTR_MOVABLE = 0x00000000,//再配置可能
	RES_ATTR_FIXED = 0x00000001,//再配置不可
};

//----------------------------------------
//型

//リソースハンドル
struct HRES
{
	union
	{
		struct
		{
			unsigned int m_id;//任意の識別ID
			CRC32 m_pathCRC;//ファイルパスCRC
		};
		unsigned long long m_key;//64bitキー
	};
	//コンストラクタ
	HRES(unsigned int id, CRC32 path_crc) :
		m_id(id),
		m_pathCRC(path_crc)
	{}
	HRES(unsigned long long key) :
		m_key(key)
	{}
	//デフォルトコンストラクタ
	HRES()
	{}
};

//--------------------------------------------------------------------------------
//リソースビルダー

//----------------------------------------
//定数

//リソースビルド属性
enum E_RES_BUILD_ATTR
{
	RES_TO_COPY_ONLY = 0x00000001,//コピーのみ
	RES_TO_BUILD = 0x00000000,//構築処理必要
	
	RES_REQUIRED_FILE = 0x00000002,//リソースにはファイルが必要
	RES_NOT_REQUIRED_FILE = 0x00000000,//リソースにはファイルが不要
};

//ビルド結果
enum E_BUILD_RESULT
{
	BUILD_IMPOSSIBLE = 0,//ビルド・コピー不可
	BUILD_SUCCESS = 1,//ビルド成功
	BUILD_RETRY = -1,//ビルド失敗：リトライ要求
	BUILD_GIVEUP = -2,//ビルド失敗：諦める
};

//----------------------------------------
//リソースビルダービルダー抽象クラス

//クラス宣言
class CResBuiderProxy;
class IResBuider;

//リソースビルダービルダー抽象クラス
class CAbstractResBuilderBuilder
{
public:
	//メソッド
	//リソースビルダー構築
	virtual IResBuider* createBuilder(const int builder_type_id, CResBuiderProxy& builder) = 0;
	//リソースビルダー破棄
	virtual bool deleteBuilder(const int builder_type_id, IResBuider* p, CResBuiderProxy& builder) = 0;
protected:
	//フィールド
	CAbstractResBuilderBuilder* m_next;//次のリソースビルダービルダー
};

//----------------------------------------
//リソースビルダーインターフェースクラス

//クラス宣言
class CResBuildReqInfo;

//リソースビルダーインターフェースクラス
class IResBuider
{
public:
	//メソッド
	//ハンドル作成
	virtual HRES makeHandle(const CResBuildReqInfo& req_info) = 0;
	//リソース構築
	virtual E_BUILD_RESULT create(const CResBuildReqInfo& req_info) = 0;
	//リソースコピー
	virtual E_BUILD_RESULT copy(const CResBuildReqInfo& req_info, const void* src_res) = 0;
	//リソース破棄
	virtual E_BUILD_RESULT destroy(void* res) = 0;
public:
	//デストラクタ
	virtual ~IResBuider()
	{}
};

//----------------------------------------
//リソースビルダープロキシークラス
//※リソースビルダーのインスタンスを構築するためのバッファを持ち、
//　リソースビルダーとして振るまうクラス

//クラス宣言
class CAbstractResBuilderBuilder;
class CResBuildReqInfo;
class IResBuider;

//リソースビルダービルダー連結リスト
namespace
{
	std::atomic<CAbstractResBuilderBuilder*> s_head = nullptr;
}

//リソースビルダープロキシークラス
class CResBuiderProxy
{
	friend void* operator new(std::size_t size, CResBuiderProxy& proxy);
	friend void operator delete(void* p, CResBuiderProxy& proxy);
public:
	//定数
	static const size_t BUILDER_SIZE_MAX = 32;//リソースビルダーの最大サイズ（バイト）
	                                          //※リソースビルダーはスタックのメモリを使用する
	                                          //※フィールドを一つも持たないリソースビルダーのサイズは4バイト
public:
	//メソッド
	//ハンドル発行
	//HRES makeHandle(const CResBuildReqInfo& req_info)
	//{
	//	if (!m_builder)
	//		return false;
	//	return m_builder->makeHandle(req_info);
	//}
	//リソースビルド
	E_BUILD_RESULT create(const CResBuildReqInfo& req_info)
	{
		if (!m_builder)
			return BUILD_IMPOSSIBLE;
		return m_builder->create(req_info);
	}
	//リソースコピービルド
	E_BUILD_RESULT copy(const CResBuildReqInfo& req_info, const void* src_res)
	{
		if (!m_builder || !src_res)
			return BUILD_IMPOSSIBLE;
		return m_builder->copy(req_info, src_res);
	}
	//リソース破棄
	E_BUILD_RESULT destroy(void* res)
	{
		if (!m_builder)
			return BUILD_IMPOSSIBLE;
		return m_builder->destroy(res);
	}
public:
	//リソースビルダー構築
	template<class T>
	IResBuider* createBuilder()
	{
		//【静的アサーション】最大サイズを超えるリソースビルダーはコンパイルできない
		static_assert(sizeof(T) <= BUILDER_SIZE_MAX, "ResourceBuilder class T is too big!");
		//リソースビルダーのインスタンスを生成して返す
		m_builder = new(*this) T();
		return m_builder;
	}
	//リソースビルダー破棄
	template<class T>
	bool deleteBuilder(T* p)
	{
		if (!p)
			return false;
		//if (p != m_builder)//←このチェックは行わない
		//	return false;    //　クラス T が多重継承をしていると、p == m_builder になるとは限らないので注意
		p->~T();
		operator delete(p, *this);
		p = nullptr;
		m_builder = nullptr;
		return true;
	}
public:
	//コンストラクタ
	CResBuiderProxy(const int builder_type_id)
	{
		s_head.load() && s_head.load()->createBuilder(builder_type_id, *this);
	}
	//デストラクタ
	~CResBuiderProxy()
	{
		deleteBuilder(m_builder);//保険処理
	}
private:
	//フィールド
	IResBuider* m_builder;//リソースビルダー
	char m_builderBuff[BUILDER_SIZE_MAX];//リソースビルダー用バッファ
};

//オペレータ
//リソースビルダー構築用配置new
void* operator new(std::size_t size, CResBuiderProxy& proxy)
{
	return proxy.m_builderBuff;
}
//リソースビルダー構築用配置delete
void operator delete(void* p, CResBuiderProxy& proxy)
{
}

//----------------------------------------
//リソースビルダービルダークラス
//※リソースビルダーを構築するためのクラス。
//　「ビルダーパターン」の実装。
//※ゲーム中で使用されるリソースビルダービルダーは
//　全てシングルトンとして保持し、連結リストでつながっている。
//※リソースビルダーを生成する際は、連結リストをたどって
//　タイプIDに該当するインスタンスを生成して返す。
//　「チェインオブレスポンシビリティパターン」の応用。

//クラス宣言
template<class T>
class CResBuildReq;
class CResBuiderProxy;
class IResBuider;
class CAbstractResBuilderBuilder;

//リソースビルダービルダーテンプレートクラス
template<class T>
class CResBuilderBuilder : CAbstractResBuilderBuilder
{
	friend class CResBuildReq<T>;
public:
	//定数
	static const int RES_TYPE_ID = T::RES_TYPE_ID;//リソース種別ID
	static const E_RES_ATTR RES_ATTR = T::RES_ATTR;//リソース属性
	static const int BUILDER_TYPE_ID = T::BUILDER_TYPE_ID;//リソースビルダー種別ID
	static const E_RES_BUILD_ATTR TO_BUILD = T::TO_BUILD;//構築にはビルド処理が必要か？
	static const E_RES_BUILD_ATTR REQUIRED_FILE = T::REQUIRED_FILE;//構築にはファイルが必要か？
public:
	//メソッド
	//リソースビルダー構築
	//※オーバーライド
	IResBuider* createBuilder(const int builder_type_id, CResBuiderProxy& proxy) override
	{
		if (builder_type_id != BUILDER_TYPE_ID)
		{
			if (!m_next)
				return nullptr;
			//リソースビルダー種別IDが異なる場合、次の連結リストにたくす
			//※チェインオブレスポンシビリティパターン
			return m_next->createBuilder(builder_type_id, proxy);
		}
		return proxy.createBuilder<T>();
	}
	//リソースビルダー破棄
	//※オーバーライド
	bool deleteBuilder(const int builder_type_id, IResBuider* p, CResBuiderProxy& proxy) override
	{
		if (builder_type_id != BUILDER_TYPE_ID)
		{
			if (!m_next)
				return nullptr;
			//リソースビルダー種別IDが異なる場合、次の連結リストにたくす
			//※チェインオブレスポンシビリティパターン
			return m_next->deleteBuilder(builder_type_id, p, proxy);
		}
		return proxy.deleteBuilder<T>(dynamic_cast<T*>(p));
	}
private:
	//オペレータ
	//自クラスインスタンス生成用配置new
	void* operator new(const std::size_t size)
	{
		return m_buff;
	}
	//自クラスインスタンス破棄用配置delete（ダミー）
	void operator delete(void* p)
	{}
private:
	//メソッド
	//自クラスのシングルトンインスタンス生成
	static void createSingleton()
	{
		if (m_this)
			return;

		//インスタンス生成
		m_this = new(CResBuilderBuilder<T>)();
		
		//連結リストに連結
		//※スレッドセーフかつロックフリーな連結手法
		m_this->m_next = s_head.load();
		while (!s_head.compare_exchange_weak(m_this->m_next, m_this));
	}
private:
	//フィールド
	static CResBuilderBuilder<T>* m_this;//自クラスシングルトンインスタンスのポインタ
	static char m_buff[];//自クラスシングルトンインスタンスのバッファ
};

//リソースビルダービルダーテンプレートクラスのstaticインスタンス生成用
#define RESOURCE_BUILDER_BUILDER_INSTANCE(T) \
	CResBuilderBuilder<T>* CResBuilderBuilder<T>::m_this = nullptr; \
	char CResBuilderBuilder<T>::m_buff[sizeof(CResBuilderBuilder<T>)];

//----------------------------------------
//リソース構築要求情報クラス
//※リソースファイルを指定して構築を要求するためのクラス
//※構築先のメモリの指定も可能（通常はリソースビルダーによって規定される）
//※本来はリソースファイルを伴わないメモリ上のデータや、
//　リソースの依存関係、リソース優先度、構築リトライのタイムアウト、
//　メモリカテゴリといった情報も扱うが、このサンプルではそこまで対応しない

//リソース構築要求情報クラス
class CResBuildReqInfo
{
public:
	//定数
	static const int MEM_INFO_MAX = 2;//メモリ情報の最大数
public:
	//データ型
	//メモリ情報型
	struct T_MEM_INFO
	{
		int m_memId;//メモリID
		bool m_isDenyShare;//共有禁止（メモリIDが違う同名リソースの禁止）
		bool m_isLocked;//ロック（破棄禁止）
		void* m_loadedBuff;//読み込み後のバッファ
		//コンストラクタ
		T_MEM_INFO(const int mem_id, const bool is_deny_share = true, bool is_locked = false) :
			m_memId(mem_id),
			m_isDenyShare(is_deny_share),
			m_isLocked(is_locked),
			m_loadedBuff(nullptr)
		{}
		//デフォルトコンストラクタ
		T_MEM_INFO(){}
	};
public:
	//アクセッサ
	int getResTypeId() const { return m_resTypeId; }//リソース種別ID取得
	int getBuilderTypeId() const { return m_builderTypeId; }//リソースビルダー種別ID取得
public:
	//メソッド
	//メモリ情報追加
	bool addMemInfo(const T_MEM_INFO&& info)
	{
		if (m_memInfoNum >= MEM_INFO_MAX)
			return false;
		m_memInfo[m_memInfoNum++] = info;
		return true;
	}
	//メモリ情報取得
	int getMemInfoNum() const { return m_memInfoNum; }
	T_MEM_INFO* getMemInfo(const int index)
	{
		if (index < 0 || index >= m_memInfoNum)
			return nullptr;
		return &m_memInfo[index];
	}
	//メモリ情報取得
	const T_MEM_INFO* getMemInfo(const int index) const
	{
		if (index < 0 || index >= m_memInfoNum)
			return nullptr;
		return &m_memInfo[index];
	}
	//メモリ情報更新
	T_MEM_INFO* setMemInfo(const int index, const T_MEM_INFO&& info)
	{
		if (index < 0 || index >= m_memInfoNum)
			return nullptr;
		m_memInfo[index] = info;
		return &m_memInfo[index];
	}
	//一般的なファイルリソースハンドル作成
	HRES makeNormalHandle() const
	{
		HRES handle(0, m_pathCRC);
		return handle;
	}
	////ハンドル作成
	//HRES makeHandle() const
	//{
	//	CResBuiderProxy proxy(m_builderTypeId);
	//	return proxy.makeHandle(*this);
	//}
public:
	//コンストラクタ
	//※構築するリソースのリソース種別ID、リソースビルダー種別IDとリソースのパスを渡す
	CResBuildReqInfo(const int res_type_id, const int builder_type_id, const char* path) :
		m_resTypeId(res_type_id),
		m_builderTypeId(builder_type_id),
		m_pathCRC(CRC(path)),
		m_memInfoNum(0)
	{
	}
	//デストラクタ
	~CResBuildReqInfo()
	{
	}
private:
	//フィールド
	int m_resTypeId;//リソース種別ID
	int m_builderTypeId;//リソースビルダー種別ID
	CRC32 m_pathCRC;//リソースパス（CRC）
	T_MEM_INFO m_memInfo[MEM_INFO_MAX];//リソース構築用のメモリ情報
	int m_memInfoNum;
};

//----------------------------------------
//リソース構築要求クラス

//クラス宣言
class CResBuildReqInfo;

//リソース構築要求クラス
template<class T>
class CResBuildReq : public CResBuildReqInfo
{
public:
	//メソッド
	//ハンドル発行
	HRES makeHandle() const
	{
		T builder;
		return builder.makeHandle(*this);
	}
public:
	//コンストラクタ
	//※構築するリソースのパスを渡す
	CResBuildReq(const char* path):
		CResBuildReqInfo(T::RES_TYPE_ID, T::BUILDER_TYPE_ID, path)
	{
		//リソースビルダービルダーシングルトン生成
		CResBuilderBuilder<T>::createSingleton();
		//リソースビルダービル
		T::setDefaultMemInfo(*this);
	}
	//デストラクタ
	~CResBuildReq()
	{
	}
};

//--------------------------------------------------------------------------------
//【サンプル用ダミー】リソースマネージャクラス

#include <vector>

//リソースマネージャクラス（ダミー）
class CResManager
{
public:
	//メソッド
	//ビルド要求
	//※リソースにアクセスするためのハンドルを返す
	template<class T>
	HRES request(T& req)
	{
		HRES handle = req.makeHandle();
		requestCore(req);
		return handle;
	}
	//全要求をビルド（ダミー）
	//※本来このようなメソッドではなく、一つ一つのビルド処理をジョブとして投入する
	void createAll()//ダミー
	{
		for (CResBuildReqInfo& build_info : m_reqList)
		{
			CResBuiderProxy proxy(build_info.getBuilderTypeId());
			proxy.create(build_info);
		}
	}
	//全要求をコピーでビルド（ダミー）
	//※本来このようなメソッドではなく、一つ一つのビルド処理をジョブとして投入する
	void copyAll()//ダミー
	{
		for (CResBuildReqInfo& build_info : m_reqList)
		{
			CResBuiderProxy proxy(build_info.getBuilderTypeId());
			void* src_res_dummy = "";
			proxy.copy(build_info, src_res_dummy);
		}
	}
	//全リソースを破棄（ダミー）
	//※本来このようなメソッドではなく、一つ一つの破棄処理をジョブとして投入する
	void destroyAll()//ダミー
	{
		for (CResBuildReqInfo& build_info : m_reqList)
		{
			CResBuiderProxy proxy(build_info.getBuilderTypeId());
			void* res_dummy = "";
			proxy.destroy(res_dummy);
		}
	}
private:
	void requestCore(CResBuildReqInfo& req)
	{
		m_reqList.push_back(req);
	}
private:
	//フィールド
	std::vector<CResBuildReqInfo> m_reqList;//ビルド要求リスト
};

//リソースマネージャシングルトンインスタンス
SINGLETON_INSTANCE(CResManager);

//--------------------------------------------------------------------------------
//【サンプル】リソースビルダークラス

//----------------------------------------
//定数：リソース種別ID
enum E_RES_TYPE_ID
{
	RES_UNKNOWN = 0,
	RES_MODEL,//モデル
	RES_TEXTURE,//テクスチャ
};

//----------------------------------------
//定数：リソースビルダー種別ID
enum E_BUILDER_TYPE_ID
{
	BUILDER_UNKNOWN = 0,
	BUILDER_MODEL,//モデル構築
	BUILDER_TEXTURE,//テクスチャ構築
};

//----------------------------------------
//定数：メモリID
enum E_MEM_ID
{
	MEM_MAIN_MODEL,//メインメモリ：モデル情報用
	MEM_GPU_MODEL,//GPUメモリ：モデル構築用
	MEM_GPU_TEX,//GPUメモリ：テクスチャ構築用
};

//----------------------------------------
//独自リソースビルダークラス：モデル用
//※ビルド実行時に、ビルドスレッド処理がインスタンスを生成して
//　build() メソッドを実行する。
//※buiild() メソッドのパラメータ CResBuildReqInfo に、
//　読み込んだファイルのバッファや、構築先のメモリ情報などが格納されている
//※デフォルトコンストラクタしか使えない
//※フィールドは好きに追加して良いが、
//　CResBuiderProxy::BUILDER_SIZE_MAX を超えるサイズは不可。
//※リソースビルダーは実行の都度インスタンスを生成するが、
//　ローカル変数として生成される。
//　（CResBuiderProxy::BUILDER_SIZE_MAX のサイズのバッファが
//　ローカル変数に作られ、そこにインスタンスが生成される）
class CModelResourceBuilder : public IResBuider
{
public:
	//定数
	//※必須
	static const E_RES_TYPE_ID RES_TYPE_ID = RES_MODEL;//リソース種別ID
	static const E_RES_ATTR RES_ATTR = RES_ATTR_MOVABLE;//リソース属性：再配置可能
	static const E_BUILDER_TYPE_ID BUILDER_TYPE_ID = BUILDER_MODEL;//リソースビルダー種別ID
	static const E_RES_BUILD_ATTR TO_BUILD = RES_TO_BUILD;//構築にはビルド処理が必要か？
	static const E_RES_BUILD_ATTR REQUIRED_FILE = RES_REQUIRED_FILE;//構築にはファイルが必要か？
public:
	//メソッド
	//メモリ情報を作成（デフォルト設定）
	//※必須
	static void setDefaultMemInfo(CResBuildReqInfo& req_info)
	{
		req_info.addMemInfo(CResBuildReqInfo::T_MEM_INFO(MEM_MAIN_MODEL));
		req_info.addMemInfo(CResBuildReqInfo::T_MEM_INFO(MEM_GPU_MODEL));
	}
public:
	//メソッド
	//ハンドル発行
	//※オーバーライド
	HRES makeHandle(const CResBuildReqInfo& req_info) override
	{
		return req_info.makeNormalHandle();
	}
	//リソース構築
	//※オーバーライド
	E_BUILD_RESULT create(const CResBuildReqInfo& req_info) override
	{
		printf("CModelResourceBuilder::create(%d, %d)\n", req_info.getResTypeId(), req_info.getBuilderTypeId());
		return BUILD_SUCCESS;
	}
	//リソースコピー
	//※オーバーライド
	E_BUILD_RESULT copy(const CResBuildReqInfo& req_info, const void* src_res) override
	{
		printf("CModelResourceBuilder::copy(%d, %d, %p)\n", req_info.getResTypeId(), req_info.getBuilderTypeId(), src_res);
		return BUILD_SUCCESS;
	}
	//リソース破棄
	//※オーバーライド
	E_BUILD_RESULT destroy(void* res) override
	{
		printf("CModelResourceBuilder::destroy(%p)\n", res);
		return BUILD_SUCCESS;
	}
public:
	//コンストラクタ
	CModelResourceBuilder()
	{
	}
	//デストラクタ
	~CModelResourceBuilder() override
	{
	}
};

//リソースビルダービルダーシングルトンのstaticインスタンス化
RESOURCE_BUILDER_BUILDER_INSTANCE(CModelResourceBuilder);

//----------------------------------------
//独自リソースビルダークラス：テクスチャ用
class CTextureResourceBuilder : public IResBuider
{
public:
	//定数
	//※必須
	static const E_RES_TYPE_ID RES_TYPE_ID = RES_TEXTURE;//リソース種別ID
	static const E_RES_ATTR RES_ATTR = RES_ATTR_MOVABLE;//リソース属性：再配置可能
	static const E_BUILDER_TYPE_ID BUILDER_TYPE_ID = BUILDER_TEXTURE;//リソースビルダー種別ID
	static const E_RES_BUILD_ATTR TO_BUILD = RES_TO_COPY_ONLY;//構築にはビルド処理が必要か？
	static const E_RES_BUILD_ATTR REQUIRED_FILE = RES_REQUIRED_FILE;//構築にはファイルが必要か？
public:
	//メソッド
	//メモリ情報を作成（デフォルト設定）
	//※必須
	static void setDefaultMemInfo(CResBuildReqInfo& req_info)
	{
		req_info.addMemInfo(CResBuildReqInfo::T_MEM_INFO(MEM_GPU_TEX));
	}
public:
	//メソッド
	//ハンドル発行
	//※オーバーライド
	HRES makeHandle(const CResBuildReqInfo& req_info) override
	{
		return req_info.makeNormalHandle();
	}
	//リソース構築
	//※オーバーライド
	E_BUILD_RESULT create(const CResBuildReqInfo& req_info) override
	{
		printf("CTextureResourceBuilder::create(%d, %d)\n", req_info.getResTypeId(), req_info.getBuilderTypeId());
		return BUILD_SUCCESS;
	}
	//リソースコピー
	//※オーバーライド
	E_BUILD_RESULT copy(const CResBuildReqInfo& req_info, const void* src_res) override
	{
		printf("CTextureResourceBuilder::copy(%d, %d, %p)\n", req_info.getResTypeId(), req_info.getBuilderTypeId(), src_res);
		return BUILD_SUCCESS;
	}
	//リソース破棄
	//※オーバーライド
	E_BUILD_RESULT destroy(void* res) override
	{
		printf("CTextureResourceBuilder::destroy(%p)\n", res);
		return BUILD_SUCCESS;
	}
public:
	//コンストラクタ
	CTextureResourceBuilder()
	{
	}
	//デストラクタ
	~CTextureResourceBuilder() override
	{
	}
};

//リソースビルダービルダーシングルトンのstaticインスタンス化
RESOURCE_BUILDER_BUILDER_INSTANCE(CTextureResourceBuilder);

//--------------------------------------------------------------------------------
//【サンプル】ビルド要求処理

//----------------------------------------
//ビルドリクエスト�@
void func1()
{
	//モデルリソースのビルド要求情報を構築
	CResBuildReq<CModelResourceBuilder> req("/data/chara/x0010.mdl");

	//ビルド要求
	CSingletonUsing<CResManager> res_man("func1");//リソースマネージャシングルトンにアクセス
	HRES handle = res_man->request(req);
}

//----------------------------------------
//ビルドリクエスト�A
void func2()
{
	//テクスチャリソースのビルド要求情報を構築
	CResBuildReq<CTextureResourceBuilder> req("/data/chara/x0010.tex");
	req.getMemInfo(0)->m_isLocked = true;//メモリ設定変更

	//ビルド要求
	CSingletonUsing<CResManager> res_man("func2");//リソースマネージャシングルトンにアクセス
	HRES handle = res_man->request(req);
}

//--------------------------------------------------------------------------------
//【サンプル】ビルド処理

//リソース構築処理（ダミー）
void createAll()
{
	//全要求をビルド
	CSingletonUsing<CResManager> res_man("createAll");//リソースマネージャシングルトンにアクセス
	res_man->createAll();
}

//リソースコピー処理（ダミー）
void copyAll()
{
	//全要求をビルド
	CSingletonUsing<CResManager> res_man("copyAll");//リソースマネージャシングルトンにアクセス
	res_man->copyAll();
}

//リソース破棄処理（ダミー）
void destroyAll()
{
	//全要求をビルド
	CSingletonUsing<CResManager> res_man("destroyAll");//リソースマネージャシングルトンにアクセス
	res_man->destroyAll();
}

//--------------------------------------------------------------------------------
//【サンプル】テストメイン
int main(const int argc, const char* argv[])
{
	//ビルド要求
	printf("----- ビルド要求 -----\n");
	func1();
	func2();
	
	//ビルド実行
	printf("----- ビルド実行 -----\n");
	createAll();

	//コピー実行
	printf("----- コピー実行 -----\n");
	copyAll();

	//リソースは黄
	printf("----- リソース破棄 -----\n");
	destroyAll();

	//終了
	return EXIT_SUCCESS;
}

// End of file
