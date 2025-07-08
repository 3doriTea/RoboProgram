# RoboProgram

このリポジトリは、学校のゲーム制作課題として作ったゲームのソースコード+リソースです。

## 使用ライブラリとフレームワーク

- [DxLib](https://dxlib.xsrv.jp)
- [ImGui](https://github.com/ocornut/imgui)
- CoG ※先生が用意してくださったDxLibのフレームワーク
  - `Library/*`
  - `Source/SceneFactory.h`と`Source/SceneFactory.cpp`の一部
- [MTBinLibrary](https://github.com/3doriTea/MTBinLibrary)  ※ 自作
  - `Source/MemoryStream.h`と`Source/MemoryStream.cpp`

※ `Source/*`のファイルは全て私が書きました。

## 環境

- IDE: Visual Studio 2022
- 言語バージョン: C++ 17

## 注力した点

- コンパイラ: [`Source/Compiler/*`](https://github.com/3doriTea/RoboProgram/tree/proto-v1/Source/Compiler/)
  - 授業では習わないので、アセンブラの本と既存のコンパイラの仕組みを調べながら作りました。
  - コンパイラ制作にのめり込み過ぎてゲームが完成しない、というのが最悪のパターンであると考えているため、</br>
    機能は最初に設計した最小限に抑えるよう、気を付けました。

## 制作期間

- 記述途中
