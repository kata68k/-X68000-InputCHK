# -X68000-InputCHK.x  
入力確認用プログラム（IOCS経由Only）  
 <img width="640" alt="title" src="../../X68000/Material/202403190013560842.png">

概要：  
純正以外の入力装置を実機もしくはX68000Zに接続したとき  
どの入力がどれに対応しているか確認できます。  

起動方法：  
InputCheck.x  
　（オプション等の引数はありません）  

または、InputCheck.XDF FDイメージを用意しました。  

対象：  
キーボード  
マウス  
ジョイスティック１とジョイスティック２  

必要環境：  
FLOAT2.X　または、その互換ドライバ  
IOCS.X　または、その互換ドライバ  

履歴：  
ver.1.0.4　COPYキーで白帯が出ないように対策しました。  
ver.1.0.3　ジョイスティックのボタンがAとBの反応が逆でした。(BELLさんありがとう！)  
ver.1.0.2　ジョイスティックのボタンを押したときのグラフィックを綺麗な円にした。  
ver.1.0.1　128倍高速化した。  
ver.1.0.0　初回リリース（激遅い。100MHz推奨）  
