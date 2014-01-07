公開プログラム＆ドキュメント
======

####[test]####
     |
     |-[Program]
     |  |
     |  |-[C++]
     |  |  |
     |  |  |-[boost]
     |  |  |  |
     |  |  |  |-[install]      ... boost のインストール手順の記録
     |  |  |  |
     |  |  |  `-[serializer]   ... boost::serialization をテスト
     |  |  |
     |  |  |-[DebugPrint]      ... デバッグ用ロギング処理／boost::backtraceのテスト
     |  |  |-[DebugPrintTest]  ... デバッグ用ロギング処理の試作
     |  |  |
     |  |  |-[FloatedFraction] ... 浮動小数点小数のバイナリデータ構造確認
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
     |  |-[Prolog]
     |  |  |
     |  |  `-[Test01]          ... Prologを試しに使ってみた
     |  |
     |  |-[Scons]              ... Sconsによるスクリプトビルド環境のテスト
     |  |
     |  `-[Unity]              ... Unityを試用（起動した程度）
     |
     |-[VBAでJSONパーステスト] ... VBAを使用し、JSONテキストをパースするテスト
     |
     `-[縦書きテスト]          ... InDesign と Word を使用し、縦書きの表示具合をテスト。

####[tools]####
     |
     `-[git_erase_wincred_all] ... TortoiseGit が wincred で記憶するユーザー認証情報を
                               一括削除する為のごく簡単なプログラムのソース

####[document]####
     |
     `-[Gitマニュアル・調査資料]
        |
        |- [00]Readme.txt
        |- [01]Subversionユーザーの為のGit活用の勧め.pptx
        |- [02]TortoiseGitセットアップ手順.docx
        |- [03]TortoiseGitのhttp(s)通信およびパスワードの記憶と破棄について.docx
        |- [04]TortoiseGitのSSH通信およびパスフレーズの記憶と破棄について.docx
        |- [05]TortoiseGitによるSSH通信用秘密鍵・公開鍵生成手順.docx
        |- [06]TortoiseGitの使い方とワークフロー.docx
        |- [資料01]Gitサーバー比較.xlsx
        |- [資料02]Gitに関する推奨書籍.docx
        |
        |- [Tools]
        |   |
        |   `-[git_erase_wincred_all] ... TortoiseGit が wincred で記憶する
        |                                 ユーザー認証情報を一括削除する為の
        |                                 ごく簡単なツール
        `[PDF]
          |
          |- ... 上記のドキュメントをPDFファイル化したもの

以上
