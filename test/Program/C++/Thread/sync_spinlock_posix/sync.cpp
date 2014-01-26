#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <pthread.h> 
#include <unistd.h>

#include <sys/time.h>

//スピンロック
static pthread_spinlock_t s_lock;

//共有データ
static int s_commonData = 0;

//スレッド固有データ
__thread int s_tlsData = 0;

//スレッド
void* threadFunc(void* param_p)
{
	//パラメータ受け取り
	const char* name = static_cast<const char*>(param_p);

	//開始
	printf("- begin:%s -\n", name);
	fflush(stdout);

	//処理
	for (int i = 0; i < 3; ++i)
	{
		//スピンロック取得
		pthread_spin_lock(&s_lock);
	//	pthread_spin_trylock(&s_lock);//取得できない時に他の処理を行いたい場合は pthread_spin_trylock() を使用する
		
		//データ表示（前）
		printf("%s: [BEFORE] commonData=%d, tlsData=%d\n", name, s_commonData, s_tlsData);
		fflush(stdout);
		
		//データ取得
		int common_data = s_commonData;
		int tls_data = s_tlsData;

		//データ更新
		++common_data;
		++tls_data;

		//若干ランダムでスリープ（0～4 msec）
		usleep((rand() % 5) * 1000);
		
		//データ書き戻し
		s_commonData = common_data;
		s_tlsData = tls_data;

		//データ表示（後）
		printf("%s: [AFTER]  commonData=%d, tlsData=%d\n", name, s_commonData, s_tlsData);
		fflush(stdout);

		//スピンロック解放
		pthread_spin_unlock(&s_lock);
		
		//スレッド切り替えのためのスリープ
		usleep(0);
	//	//スレッド切り替え
	//	sched_yield();//同じ優先度の他のスレッドに切り替え
	}

	//終了
	printf("- end:%s -\n", name);
	fflush(stdout);
	return 0;
}

//テスト
int main(const int argc, const char* argv[])
{
	//スピンロック生成
//	pthread_spin_init(&s_lock, PTHREAD_PROCESS_SHARED);
	pthread_spin_init(&s_lock, PTHREAD_PROCESS_PRIVATE);//単独プロセス専用
	
	//スレッド作成
	static const int THREAD_NUM = 3;
	pthread_t pth[THREAD_NUM];
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, 1024);//スタックサイズ指定
		pthread_create(&pth[0], &attr, threadFunc, (void*)"Taro  ");
		pthread_create(&pth[1], &attr, threadFunc, (void*)"Jiro  ");
		pthread_create(&pth[2], &attr, threadFunc, (void*)"Saburo");
	}
	
	//スレッド終了待ち
	pthread_join(pth[0], NULL);
	pthread_join(pth[1], NULL);
	pthread_join(pth[2], NULL);
	
	//スピンロックの取得と解放を大量に実行して時間を計測
	{
		struct timeval begin;
		gettimeofday(&begin, NULL);
		static const int TEST_TIMES = 10000000;
		for (int i = 0; i < TEST_TIMES; ++i)
		{
			pthread_spin_lock(&s_lock);
			pthread_spin_unlock(&s_lock);
		}
		struct timeval end;
		gettimeofday(&end, NULL);
		struct timeval prog;
		if( end.tv_usec >= begin.tv_usec)
		{
			prog.tv_sec = end.tv_sec - begin.tv_sec;
			prog.tv_usec = end.tv_usec - begin.tv_usec;
		}
		else
		{
			prog.tv_sec = end.tv_sec - begin.tv_sec - 1;
			prog.tv_usec = 1000000 - begin.tv_usec + end.tv_usec;
		}
		printf("Mutex * %d = %d.%06d sec\n", TEST_TIMES, prog.tv_sec, prog.tv_usec);
	}
	
	//スピンロック破棄
	pthread_spin_destroy(&s_lock);
	
	return EXIT_SUCCESS;
}

// End of file
