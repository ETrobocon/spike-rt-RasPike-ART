# アプリケーションの開発方法
アプリケーション開発における一連の流れを説明する．

## ソースコードの入手
以下により，ソースコードをクローンする．
```bash
git clone https://github.com/shimojima/spike-rt.git -b etrobo
cd spike-rt
git submodule update --init ./external/
```
以下，特に断りの無い限りトップディレクトリが`spike-rt` のディレクトリであるとする．
また，アプリケーション・ソースコードのディレクトリが `spike-rt/$appdir`に配置されているとする．


## 開発環境の構築
[Env.md](Env.md)に従って，開発環境を構築する．

## 必要なパッケージのインストール（Ubuntu/WSL2）

```bash
sudo apt update
sudo apt install git ruby make gcc-arm-none-eabi
```

## 必要なパッケージのインストール（Windows）

① python3
- python3.exe -- https://pythonlinks.python.jp/ja から python-3.14.0-amd64.exe など
- ※必ず、「Add python.exe to PATH」にチェックを入れること

②　pyusb
― Windows PowerShellでpyusbをインストールしておく
```bash
pip install pyusb
```
- ※pyusbを入れ忘れると、後々「No module named usb」というエラーで落ちる

③libusb-win32ドライバー（一度変更しておけばOK）
- https://zadig.akeo.ie から zadig-2.9.exe をダウンロードし、インストールする
- 最初にSPIKE-RTをDFUモードで接続したら、zadigを起動し、「Options」→「List All Devices」→ 「LEGO Technic Large Hub in DFU Mode」を選ぶ
 - ドライバーを「WinUSB」から「libusb-win32」へ変更し、「Install Driver」を押す
- ※ドライバーが正しくないと、ファイル転送が「No backend available」で失敗する

## ビルド

### カーネルライブラリの生成
```bash
./scripts/build-kernel.sh
```

### コンフィグ

ビルドの前にテンプレートからビルド用 Makefile の生成を行う．初回時のみ必要である．
カーネルとアプリケーションを分割して，ビルドすることを想定している．

アプリケーション用 Makefile の生成
```bash
mkdir -p build/obj-primehub_$appname
cd build/obj-primehub_$appname
../../asp3/configure.rb -T primehub_gcc -L ../obj-primehub_kernel -a ../../$appdir/ -A $appname -m ../../common/app.mk
cd ../..
```

### ビルド
ホスト側で以下を実行し，カレントディレクトリをアプリのビルドディレクトに移動する．
```bash
cd build/obj-primehub_$appname
```
以下により，
```bash
(cd ../obj-primehub_kernel && && make libkernel.a) && rm -rf asp asp.bin && make
```
再ビルド・再リンクする場合もこのコマンドで良い．

### 例
モータのサンプル・アプリケーション`sample/motor`のビルド例．

```bash
mkdir -p build/obj-primehub_kernel
cd build/obj-primehub_kernel
../../asp3/configure.rb -T primehub_gcc -f -m ../../common/kernel.mk
make libkernel.a
cd -

mkdir -p build/obj-primehub_motor
cd build/obj-primehub_motor
../../asp3/configure.rb -T primehub_gcc -L ../obj-primehub_kernel -a ../../sample/motor/ -A app -m ../../common/app.mk

make
cd -

exit
```

## 書き込み
ホスト側で以下を実行し，カレントディレクトリをアプリのビルドディレクトに移動する．（既に移動している場合は不要）
```bash
cd build/obj-primehub_$appname
```

### HubのDFUモードへの遷移
- USBケーブルを抜き，Hubの電源を切る．
- Bluetoothボタンを押しながらUSBを接続する．
- ボタンのLEDが点滅するまで，Bluetoothボタンを押し続ける．

### 書き込みの実行
SPIKE Prime HubをDFUモードにしてPCに接続し、Windows上のpythonをWSL2から起動してasp.binをHubに書き込む
```bash
 ../../scripts/deploy-dfu.sh asp.bin     
```

## USBシリアルの接続
WindowsのTeratermなどでUSBシリアルに接続し，ログ出力を確認することができる．
一旦，Hub の電源をONにしてからでないと，接続できないことに注意する．
