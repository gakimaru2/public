公開プログラム＆ドキュメント
======

####[test]####
     |
     |-[Program]
     |  |
     |  |-[C#]
     |  |  |
     |  |  `-[JSON]            ... C#によるJSON解析およびC#スクリプトのランタイムコンパイル＆実行のテスト
     |  |
     |  |-[C++]
     |  |  |
     |  |  |-[boost]
     |  |  |  |
     |  |  |  |-[install]      ... boost のインストール手順の記録
     |  |  |  |
     |  |  |  `-[serializer]   ... boost::serialization をテスト
     |  |  |
     |  |  |-[CalcExpression]  ... 計算式の状況に応じた右値評価のテスト
     |  |  |
     |  |  |-[DebugPrint]      ... デバッグ用ロギング処理／boost::backtraceのテスト
     |  |  |-[DebugPrintTest]  ... デバッグ用ロギング処理の試作
     |  |  |
     |  |  |-[FloatedFraction] ... 浮動小数点小数のバイナリデータ構造確認
     |  |  |
     |  |  |-[GDConv]          ... ゲームデータ構造定義データの構成テスト
     |  |  |
     |  |  |-[OpenGL]          ... OpenGLとGLSLのごく簡単なテスト（シェーダーのテストが目的）
     |  |  |
     |  |  |-[StaticCRC32]     ... 文字列のCRC32計算をテンプレートで行うテストプログラム
     |  |  |                       コンパイル時に完結する事前計算を目指したが...無理だった。
     |  |  |
     |  |  |-[template]
     |  |  |  |
     |  |  |  |-[countOf]      ... 配列要素数取得のテンプレートとマクロ比較テスト
     |  |  |  |
     |  |  |  |-[CRTP01]       ... CRTPテンプレートのテスト
     |  |  |  |
     |  |  |  |-[Max]          ... Min/Maxテンプレート関数のテスト（アセンブラ展開の状態確認が目的）
     |  |  |  |                    ※本当は constexpr との挙動の違いを比較したかった
     |  |  |  |
     |  |  |  `-[Proxy]        ... テンプレートによる、Proxyパターンおよび自前仮想クラスの実装テスト
     |  |  |
     |  |  `-[UnitTest]        ... ユニットテスト用処理作成
     |  |
     |  |-[F#]
     |  |  |
     |  |  `-[Test01]          ... F#を試しに使ってみた
     |  |
     |  |-[JScript]
     |  |  |
     |  |  `-[JSON]            ... JScriptによるJSON解析テスト（VBA, VBScript, PowerShellのサンプルも置いている）
     |  |
     |  |-[Prolog]
     |  |  |
     |  |  `-[Test01]          ... Prologを試しに使ってみた
     |  |
     |  |-[Scons]              ... Sconsによるスクリプトビルド環境のテスト
     |  |
     |  `-[Unity]              ... Unityを試用（起動した程度）
     |
     |-[Excel]
     |  |
     |  |-[VBAでJSONパーステスト] ... VBAを使用し、JSONテキストをパースするテスト
     |  |
     |  `-[What-If分析テスト]  ... ExcelのWhat-If分析機能をテスト
     |
     `-[ドキュメント]
        |
        `-[縦書きテスト]       ... InDesign と Word を使用し、縦書きの表示具合をテスト。

####[tools]####
     |
     `-[git_erase_wincred_all] ... TortoiseGit が wincred で記憶するユーザー認証情報を
                               一括削除する為のごく簡単なプログラムのソース

####[document]####
     |
     |-[Gitマニュアル・調査資料]
     |  |
     |  |- [00]Readme.txt
     |  |- [01]Subversionユーザーの為のGit活用の勧め.pptx
     |  |- [02]TortoiseGitセットアップ手順.docx
     |  |- [03]TortoiseGitのhttp(s)通信およびパスワードの記憶と破棄について.docx
     |  |- [04]TortoiseGitのSSH通信およびパスフレーズの記憶と破棄について.docx
     |  |- [05]TortoiseGitによるSSH通信用秘密鍵・公開鍵生成手順.docx
     |  |- [06]TortoiseGitの使い方とワークフロー.docx
     |  |- [資料01]Gitサーバー比較.xlsx
     |  |- [資料02]Gitに関する推奨書籍.docx
     |  |
     |  |- [Tools]
     |  |   |
     |  |   `-[git_erase_wincred_all] ... TortoiseGit が wincred で記憶する
     |  |                                 ユーザー認証情報を一括削除する為の
     |  |                                 ごく簡単なツール
     |  `[PDF]
     |    |
     |    |- ... 上記のドキュメントをPDFファイル化したもの
     |
     `-[仕様・設計書]
        |
        |- Readme.txt
        |
        |- ゲームシステムのアーキテクチャと開発環境.docx
        |
        |-[AI]
        |  |
        |  `- 【未】プランナーのためのAIシステム考察.docx
        |
        |-[アセット管理]
        |  |
        |  `- 【未】効果的なランタイムアセット管理.docx
        |
        |-[イベントシステム]
        |  |
        |  `- 【未】効果的なイベントストリーミングシステム.docx
        |
        |-[カメラシステム]
        |  |
        |  `- 【未】カメラ処理の効率化手法.docx
        |
        |-[ゲームデータ管理]
        |  |
        |  |- ゲームデータ仕様.docx
        |  |- ゲームデータ管理DBシステム.docx
        |  `- ローカライズのためのテキスト管理構造.docx
        |
        |-[ゲームループ管理]
        |  |
        |  |- 【未】マルチスレッドによるゲームループ管理.docx
        |  `- 【未】安全性をのためのメッセージキュー管理とイベントドリブン
        |
        |-[サウンドシステム]
        |  |
        |  `- 【未】サラウンドとリソース管理を効率化するためのサウンドシステム.docx
        |
        |-[シーン管理]
        |  |
        |  `- 【未】ゲーム全体を円滑に制御するためのシーン管理.docx
        |
        |-[シリアライズ]
        |  |
        |  `- 【未】セーブデータのためのシリアライズ処理.docx
        |
        |-[スクリプト管理]
        |  |
        |  `- 【未】スクリプトの生産性向上のためのプロパティマップ
        |
        |-[デバイス管理]
        |  |
        |  `- 【未】反応性と安全性を考慮した入力デバイス管理.docx
        |
        |-[デバッグシステム]
        |  |
        |  |- 【未】デバッグ制御システム.docx
        |  |- 【未】ユニットテストと継続的ビルド.docx
        |  `- 【未】効果的なデバッグログとアサーション.docx
        |
        |-[ファイルシステム]
        |  |
        |  `- 【未】開発を効率化するためのファイルシステム.docx
        |
        |-[プログラミングTips]
        |  |
        |  |- 【未】コンパイルを効率化するためのコーディング手法.docx
        |  |- 【未】デザインパターンの活用.docx
        |  |- 【未】プレイヤーに不満を感じさせないための乱数制御.docx
        |  |- 【未】プログラミング禁則事項.docx
        |  |- 【未】効果的なテンプレートテクニック.docx
        |  `- 【未】本当にちょっとしたプログラミングTips
        |
        |-[プロジェクト管理]
        |  |
        |  |- 【未】ゲーム開発プロジェクト管理.docx
        |  |- 【未】プロジェクト管理Webシステム.docx
        |  `- 【未】技術ナレッジDBシステム.docx
        |
        |-[マルチスレッド制御]
        |  |
        |  |- 【未】「サービス」によるマルチスレッドの効率化.docx
        |  `- 【未】効率化と安全性のためのロック制御.docx
        |
        |-[メモリ管理]
        |  |
        |  |- 【未】ゲーム制御のためのメモリ管理方針.docx
        |  |- 【未】ヒープメモリとスラブアロケータを併用したメモリ管理.docx
        |  `- 【未】様々なメモリ管理手法と共通アロケータインターフェース.docx
        |
        |-[リソース管理]
        |  |
        |  `- 【未】開発の効率化と安全性のためのリソース管理.docx
        |
        |-[レベル管理]
        |  |
        |  `- 【未】オープンワールドのためのレベル管理
        |
        |-[開発環境]
        |  |
        |  `- 【未】複数タイトルにまたがる効率的なフレームワーク管理.docx
        |
        |-[_PDF]
        |  |
        |  |- ... 上記のドキュメントをPDFファイル化したもの
        |
        `-[_Template]
           |
           `- 仕様・設計書テンプレート.dotx ... Wordのドキュメントテンプレート

以上
