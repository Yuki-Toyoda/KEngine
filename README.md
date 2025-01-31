[![DebugBuild](https://github.com/Yuki-Toyoda/KEngine/actions/workflows/DebugBuild.yml/badge.svg?branch=main)](https://github.com/Yuki-Toyoda/KEngine/actions/workflows/DebugBuild.yml)
[![ReleaseBuild](https://github.com/Yuki-Toyoda/KEngine/actions/workflows/ReleaseBuild.yml/badge.svg?branch=main)](https://github.com/Yuki-Toyoda/KEngine/actions/workflows/ReleaseBuild.yml)
# 就職作品 : **VS ファントム**
## この作品について
この作品は、「**ゼルダの伝説 風のタクト**」に登場する中ボス、「**ファントムガノン**」との戦いを再現するために開発している作品です。[^1]  
開発する上で特に意識していることとして、**ボスとの弾のラリーの気持ちよさ**を意識して開発をしています。

![ラリー画面_基ゲーム](https://github.com/user-attachments/assets/8018ed61-d699-4987-acbc-a914ad650432)
[作品プレイ映像はこちら](https://youtu.be/1xP_L53tc8o)
[^1]: [ファントムガノン戦参考動画](https://www.youtube.com/watch?v=9eoUYuAO9wI)
## 作品で実装したもの
### - Z注目(ロックオン)
コントローラーの_左トリガー_を押し込むことで**Z注目**を行います。  
ロックオンできる対象が画面内にいない場合、プレイヤーの背面にカメラが回り込みます。  

![カメラリセット](https://github.com/user-attachments/assets/a252c97a-ab88-483b-8006-35a4c475dd76)

ロックオン可能な対象が画面内にいる場合、その敵をロックオンします。
また、ロックオン中のアングルは_右スティック_を使用して**自由に調整**することができます。
![ロックオン時カメラワーク (2) (1)](https://github.com/user-attachments/assets/5d83fbed-a9b6-4526-882b-9b594f78e92b)

###  - 敵との弾のラリー
敵が発射した弾を、**通常攻撃で打ち返す**ことができ、複数回繰り返すと敵が**ダウン**します。  
ダウンした敵には、攻撃を行うことができます。  

![tama ](https://github.com/user-attachments/assets/4ab9591f-0d24-4b35-834b-7c3712183a03)

###  - 背後攻撃 → カウンター
敵がプレイヤーの背後に回り込み、攻撃してくる行動です。
![背後回り込み攻撃](https://github.com/user-attachments/assets/f634ca22-3304-4128-9a37-ad9b15c78722)
攻撃開始までに_Bボタン_を押すことで**カウンター攻撃**を繰り出し、敵を**即時にダウン**させることができます。
![カウンター演出](https://github.com/user-attachments/assets/03d2a007-1d2e-4d82-8ab3-e4502eae3d3f)

###  - コンボ作成システム
プレイヤーのコンボ攻撃をImGui上から**作成、編集、保存**ができるシステムです。  
コンボ作成や編集のためのエディターを**ImGuiで作成**しています。  
**再生するアニメーションや、攻撃段数、硬直時間等を編集することが可能**で、保存すると**json形式**のファイルで保存が行われます。  
コンボ攻撃中、特定の変数の条件を満たした際に指定した任意の別のコンボに切り替えられる機能を実装しています。 

![ヒットストップ](https://github.com/user-attachments/assets/37c2ad6f-9d63-4dc9-a832-a7ce23c6b6d6)

これには、コンボを管理する`ComboManager`に対して、**条件となる変数を追加**する必要があります。    

![KEngine - Player cpp 2024_10_27 18_23_03](https://github.com/user-attachments/assets/a0c87a32-a2d2-4406-be32-a9e5e68d9d53)

実際のゲーム内でコンボを変更する場合は、`ComboManager`に対して、**コンボ変更関数**を呼び出すことでコンボ攻撃の内容が変化するようになっています。 

![KEngine - Player cpp 2024_10_27 17_50_57](https://github.com/user-attachments/assets/30e8a49a-9cd4-41c6-890e-4b2af7feda77)
[コンボ作成システムに関するソースコードはこちら](https://github.com/Yuki-Toyoda/KEngine/tree/WPO_00_dev/App/GameObject/User/Player/Combo)

>[!TIP]
>**コンボマネージャー上で編集できる項目一覧**
>![コンボ編集画面](https://github.com/user-attachments/assets/f5c44e9f-656e-467d-af2f-ab1200d23806)
>1. 行動名 `文字列`
>2. 行動中に再生されるアニメーション名 `文字列`  
>3. アニメーション終了まで次の行動に遷移しないか `bool`  
>4. 硬直時間 `float (秒数)`  
>5. 次の行動に移るまでの受付時間 `float (秒数)`  
>6. 行動中に攻撃判定が発生するか  `bool`
>7. (攻撃判定が存在する場合のみ) ダメージ `int`  
>8. (攻撃判定が存在する場合のみ) 攻撃範囲の長さ `float`
>9. (攻撃判定が存在する場合のみ) 攻撃開始時間 `float (秒数)`
>10. (攻撃判定が存在する場合のみ) 攻撃終了時間 `float (秒数)`
>11. (硬直時間終了している場合) 移動した際にコンボを中断するか `bool`  
>12. 次の行動に移行する条件 `ラジオボタンで選択 (TRIGGER, PRESS, RELEASE)`
>13. コンボの切り替え条件名 `文字列`
>14. 切り替えるコンボ名称 `文字列`

## 実装予定、改善予定のもの
### さらなる要素の追加
粗方のゲームの流れは完成したため、これからボスの行動やプレイヤーの行動を増やし、様々なことができるボス戦にしたいと考えています。
