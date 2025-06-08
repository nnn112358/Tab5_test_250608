# M5Tab5 シンプルデモ

[M5Stack Tab5](https://docs.m5stack.com/en/products/sku/k145) デバイス（ESP32-P4）用のシンプル化された組み込みアプリケーションです。抽象化レイヤーを使わずに直接ハードウェアとやり取りするため、理解しやすく改変も容易です。

## 特徴

- **直接ハードウェア制御**: HAL抽象化なし、M5Stack Tab5 BSPとの直接統合
- **シンプルなUI**: システムステータス表示付きの基本的なLVGLベースインターフェース
- **ハードウェアサポート**: カメラ、IMU、電源監視、RTC、ディスプレイ、タッチ、USB、オーディオ
- **最小限の依存関係**: LVGLとログライブラリのみ
- **単一ファイル実装**: 全ロジックを`main/simple_main.cpp`に統合

## ハードウェア要件

- **M5Stack Tab5**: 1280x720ディスプレイ搭載のESP32-P4
- **コンポーネント**: INA226電源モニター、RX8130 RTC、各種カメラセンサー

## ビルド手順

### 前提条件

- [ESP-IDF v5.4.1](https://docs.espressif.com/projects/esp-idf/en/v5.4.1/esp32p4/index.html)

### セットアップ

1. **依存関係の取得:**
```bash
python ./fetch_repos.py
```

2. **ビルド:**
```bash
idf.py build
```

3. **フラッシュ:**
```bash
idf.py flash
```

4. **モニター:**
```bash
idf.py monitor
```

### 設定

```bash
idf.py menuconfig  # プロジェクト設定の変更
```

## 開発ツール

### WiFi C6ファームウェア更新
```bash
cd wifi_c6_fw
./flash.sh
```

### コンポーネントテスト
```bash
# カメラセンサー検出テスト
cd components/esp_cam_sensor/test_apps/detect
idf.py build flash monitor

# LVGLパフォーマンステスト
cd components/espressif__esp_lvgl_port/test_apps/simd
idf.py build flash monitor
```

## プロジェクト構造

```
M5Tab5-UserDemo/
├── main/
│   ├── simple_main.cpp          # メインアプリケーションエントリーポイント
│   └── hal/utils/rx8130/        # RTCユーティリティライブラリ
├── components/                  # ESP-IDFコンポーネントライブラリ
│   ├── esp_cam_sensor/          # カメラセンサードライバー
│   ├── esp_video/               # ビデオ処理
│   ├── esp_lvgl_port/           # LVGL統合
│   ├── m5stack_tab5/            # ボードサポートパッケージ
│   ├── ina226/                  # 電源監視
│   └── ...
├── dependencies/                # 外部ライブラリ
│   ├── lvgl/                    # LVGL UIフレームワーク
│   └── mooncake_log/            # ログシステム
└── wifi_c6_fw/                  # WiFiコプロセッサーファームウェア
```

## 主要コンポーネント

- **LVGL v9.2.2**: ディスプレイとタッチ入力用のUIフレームワーク
- **Mooncake Log v1.0.0**: 軽量ログシステム
- **M5Stack Tab5 BSP**: ハードウェアアクセス用ボードサポートパッケージ
- **INA226**: 電源監視ICドライバー
- **RX8130**: リアルタイムクロックドライバー
- **ESP Camera Sensor**: 複数カメラセンサーサポート

## コード品質

このプロジェクトはコードフォーマットにclang-format-13を使用しています。シンプル化されたアーキテクチャにより、複雑な抽象化を削除しながらも、クリーンで読みやすいコードを維持しています。

## 謝辞

このプロジェクトは以下のオープンソースライブラリとリソースを参考にしています：

- [LVGL](https://github.com/lvgl/lvgl) - グラフィックスライブラリ
- [Mooncake Log](https://github.com/Forairaaaaa/mooncake_log) - ログシステム
- [ESP Camera Sensor](https://components.espressif.com/components/espressif/esp_cam_sensor) - カメラドライバー
- [ESP Video](https://components.espressif.com/components/espressif/esp_video) - ビデオ処理
- [ESP LVGL Port](https://components.espressif.com/components/espressif/esp_lvgl_port) - LVGL統合
- [INA226 Library](https://github.com/jarzebski/Arduino-INA226) - 電源監視
- [BMI270 Sensor API](https://github.com/boschsensortec/BMI270_SensorAPI) - IMUサポート
- [RX8130 Driver](https://github.com/alexreinert/piVCCU/blob/master/kernel/rtc-rx8130.c) - RTCサポート

## 元プロジェクト

これは元の[M5Stack Tab5 User Demo](https://github.com/m5stack/M5Tab5-UserDemo)のシンプル化バージョンです。
