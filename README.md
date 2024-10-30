# 就職作品 : **VS ファントム**
## この作品について
この作品は、「**ゼルダの伝説 風のタクト**」に登場する中ボス、「**ファントムガノン**」との戦いを再現するために開発している作品です。[^1]

![ラリー時画像](https://github.com/user-attachments/assets/d222ec35-bac4-4247-b28f-42d8221f2e35)
[作品プレイ映像はこちら](https://youtu.be/x2WuLgSkXnI)
[^1]: [ファントムガノン戦参考動画](https://www.youtube.com/watch?v=9eoUYuAO9wI)
## 作品で実装したもの
### - Z注目(ロックオン)
コントローラーの_左トリガー_を押し込むことで**Z注目**を行います。  
ロックオンできる対象が画面内にいない場合、プレイヤーの背面にカメラが回り込みます。  

![LockOn](https://github.com/user-attachments/assets/1dfe5a09-fe71-4569-b1f3-29d36c8b93d7)

注目時の角度は、ロックオン開始時に**敵がどこにいるか**で変化するようになっています。  

![LockOn](https://github.com/user-attachments/assets/01dbad26-93f9-4654-9049-83f91d2eae4d)

###  - 敵との弾のラリー
敵が発射した弾を、**通常攻撃で打ち返す**ことができ、複数回繰り返すと敵が**ダウン**します。  
ダウンした敵には、攻撃を行うことができます。  

![Rally](https://github.com/user-attachments/assets/703a423b-3174-40a0-8b9f-9247f21ffff9)

###  - コンボ作成システム
プレイヤーのコンボ攻撃をImGui上から**作成、編集、保存**ができるシステムです。  
コンボ作成や編集のためのエディターをImGuiで作成しています。  
再生するアニメーションや、攻撃段数、硬直時間等を編集することが可能で、保存するとjson形式のファイルで保存が行われます。  
コンボ攻撃中、特定の変数の条件を満たした際に指定した任意の別のコンボに切り替えられる機能を実装しています。 

![Combo-Change](https://github.com/user-attachments/assets/162303e7-e680-42fa-af59-950864a3ffb4)

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

## 実装予定のもの
### カメラワークの改善
現在のカメラワークは、Z注目をした時点で固定されるようになっていますが、原作(風のタクト)では、Z注目中も右スティックでカメラの角度を調整することができます。  
プレイヤーと弾のラリーをより原作に近いものにするために、下記GIFのようなカメラワークを実装したいと考えています。  

 - **見下ろし視点**  
![CameraWork1 (3)](https://github.com/user-attachments/assets/e3b41a69-870e-4d71-b7c3-2d5c8cdc6ab0)

 - **横向き視点**  
![CameraWork2](https://github.com/user-attachments/assets/e6be8572-8a77-49fb-aa2a-66de16fb3d00)
