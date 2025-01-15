<div align="center">

<img src="https://github.com/rubikpi-ai/documentation/blob/1ba7e3b6a60915213496a6cc06ea8ad3ce2c0d8c/media/RUBIK-Pi-3-Logo.png?raw=true" width="50%">

<h1>Demo</h1>
------------------------
<br>
 [简体中文] | [<a href="https://github.com/rubikpi-ai/demo/blob/main/README.md">English</a>]
<br><br>
</div>

此仓库保存在 RUBIK Pi 上运行的一些小型Demo。

## QIM

### 1. 什么是 QIM

高通®智能多媒体软件开发工具包（QIM SDK）是一套统一的 SDK，适用于物联网（IoT）各个领域，助力实现多媒体和人工智能/机器学习（AI/ML）应用无缝部署。该 SDK 使用开源多媒体框架 GStreamer，并提供应用于多媒体和机器学习（ML）领域的简便 API 和插件。

有了这些插件，应用开发者能够在智能/连接/IP/运动/网络摄像头、机器人/无人机、AI 或 ML 设备等多个领域中开发各种多媒体和 AI/ML 应用程序。应用可以是单流/多流多媒体应用、ML应用或两者的结合。

该 SDK 提供一套具备硬件加速功能的高通插件，实现用例/管道的灵活构建。这些插件经过优化，可在高通硬件/IP 上工作，从而确保最终应用程序以最有效的方式运行。插件涵盖视频编码/解码、摄像头 ISP、GPU、显示、音频 DSP（aDSP）、计算 DSP（cDSP）以及 AI/ML 加速器。

这种插件式框架保证开发者“即插即用”各种插件，构建特定的多媒体或 AI/ML 应用，既简便又灵活。开发者无需理解各芯片组不相同的底层平台库和硬件细节。该 SDK 通过插件架构屏蔽了硬件的复杂性，并为应用提供了简便的API，使得基于该 SDK 构建的应用能够在不同版本或不同层级的芯片组上顺畅运行。

### 2. QIM 中 Sample applications 的使用方法

#### 2.1 Sample applications 使用前准备：

1.连接相机和 HDMI OUT 线。

2.每次烧写后执行如下命令：

```cmake
adb root
adb shell mount -o rw,remount /
adb shell "echo multiCameraLogicalXMLFile=kodiak_dc.xml >> /var/cache/camera/camxoverridesettings.txt"
adb shell "echo enableNCSService=FALSE >> /var/cache/camera/camxoverridesettings.txt"
```

3.请将QIM目录下面的RubikPi_models_for_sample_applications压缩包内的模型文件推送到板子的/opt 目录下面

```cmake
adb push ./* /opt
```

#### 2.2 可执行的 Sample applications 和使用方法

##### 2.2.1 gst-ai-classification

gst-ai-classification 能够识别图像中的主体。用例可以选择 Qualcomm Neural Processing SDK 运行时或 TensorFlow Lite (TFLite) 运行时。

###### 2.2.1.1 用例

运行如下命令，针对不同用例运行应用：

- - 使用 Qualcomm Neural Processing SDK 运行时:

```cmake
gst-ai-classification --ml-framework=1 --model=/opt/inceptionv3.dlc --labels=/opt/classification.labels
```

- 使用 TFLite 运行时:

```javascript
gst-ai-classification  -t 2 --ml-framework=2 --model=/opt/inception_v3_quantized.tflite  --labels=/opt/classification.labels
```

##### 2.2.2 gst-ai-object-detection

gst-ai-object-detection 可检测图像和视频中的物体。用例展示了如何使用 Qualcomm Neural Processing SDK 运行时执行 [YOLO-NAS](https://github.com/Deci-AI/super-gradients/blob/master/YOLONAS.md) 。

###### 2.2.2.1 用例

- 使用 Qualcomm Neural Processing SDK 运行时和摄像头输入执行 YOLO-NAS。 

```cmake
gst-ai-object-detection --ml-framework=1 --model=/opt/yolonas.dlc --labels=/opt/yolonas.labels
```

##### 2.2.3 gst-ai-pose-detection

gst-ai-pose-detection 能够检测图像或视频中主体的身体姿势。用例使用摄像头的输入流，并将结果显示在屏幕上。

###### 2.2.3.1 用例

- 使用该用例，您可通过 TFLite 外部委托代理 - Hexagon 张量处理器运行推理。

```cmake
gst-ai-pose-detection --model=/opt/posenet_mobilenet_v1.tflite --labels=/opt/posenet_mobilenet_v1.labels
```

##### 2.2.4 gst-ai-segmentation

通过 gst-ai-segmentation， 您可以将图像划分为多个有意义的部分或区域，并根据属性的相似度为每个同质区域分配标签。该应用程序使用了 Qualcomm Neural Processing SDK 运行时。

###### 2.2.4.1 用例

- 使用 Qualcomm Neural Processing SDK 运行时，从主摄像头运行 deeplabv3_resnet50 模型：

```cmake
gst-ai-segmentation --ml-framework=1 --model=/opt/deeplabv3_resnet50.dlc --labels=/opt/deeplabv3_resnet50.labels
```

##### 2.2.5 gst-ai-parallel-inference

gst-ai-parallel-inference 可针对不同来源（如摄像头）的输入流执行物体检测、物体分类、姿势检测和图像分割。用例中，物体检测和图像分割使用 Qualcomm Neural Processing SDK 运行时，分类和姿势检测则使用 TFLite 运行时。

###### 2.2.5.1 用例

- 该示例应用演示了实时流的分类、分割、目标检测、姿态检测，并输出4个并行流。

```cmake
gst-ai-parallel-inference
```

##### 2.2.6 gst-ai-multi-input-output-object-detection

gst-ai-multi-input-output-object-detection 针对不同来源的多个视频流（如摄像头或文件）执行目标检测。

###### 2.2.6.1 Use cases

- 在来自两台摄像头的流上运行 AI 物体检测：

```cmake
gst-ai-multi-input-output-object-detection --num-camera=2 --display --model=/opt/yolov5.tflite --labels=/opt/yolov5.labels
```

- 在来自文件源的流上运行 AI 物体检测：

```cmake
gst-ai-multi-input-output-object-detection --num-file=2 --display --model=/opt/yolov5.tflite --labels=/opt/yolov5.labels
```

视频文件的默认源路径是 /opt/video1.mp4 and /opt/video2.mp4

##### 2.2.7 gst-ai-daisychain-detection-classification

使用 gst-ai-daisychain-detection-classification， 您可以通过摄像头或文件源执行级联物体检测和分类。使用场景包括检测物体并对检测到的物体进行分类。

###### 2.2.7.1 用例

- 通过摄像头流运行应用程序，在 Wayland 显示器上查看输出：

```cmake
gst-ai-daisychain-detection-classification --camera
```

- 通过文件流运行应用程序，在 Wayland 显示器上查看输出：

```cmake
gst-ai-daisychain-detection-classification --file=/opt/video.mp4
```

##### 2.2.8 gst-ai-monodepth

gst-ai-monodepth 使您从实时摄像头流中推断源馈送的深度。

###### 2.2.8.1 用例

- 该示例应用演示了实时流中的 Monodepth。

```cmake
gst-ai-monodepth --ml-framework=1 --model=/opt/midasv2.dlc --labels=/opt/monodepth.labels
```

### 3. QIM 中 AI 相关的 GStreamer 插件的使用方法

#### 3.1 插件 qtimlvsegmentation

qtimlvsegmentation 插件将来自 ML 推理插件（例如 qtimlTFLite、qtimlsnpe 和 qtimlaic）的图像分割/深度估计模型的输出张量处理为预测结果。

处理后的输出是图像掩码（GstCaps：video/x-raw），协商的 GstCaps 决定了尺寸和格式。可以使用 qtivcomposer 将其应用于原始图像。

对于此掩码，该组件使用基于 CPU 的 Cairo 2D 图形库，在 ION/DMA 缓冲区中绘制预测结果。GstImageBufferPool 自定义缓冲池类通过 IOCTL 命令将 ION/DMA 缓冲区分配给内核。

##### 3.1.1 使用 snpe 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1920,height=1080,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::dimensions="<1920,1080>" sink_1::alpha=0.5 ! queue ! waylandsink sync=false fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimlsnpe delegate=dsp model=/opt/deeplabv3_resnet50.dlc ! queue ! qtimlvsegmentation module=deeplab-argmax labels=/opt/deeplabv3_resnet50.labels ! video/x-raw,width=256,height=144 ! queue ! mixer.
```

##### 3.1.2 使用 tflite 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::alpha=0.5 ! queue ! waylandsink fullscreen=true sync=false split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=gpu model=/opt/deeplabv3_resnet50.tflite ! queue ! qtimlvsegmentation module=deeplab-argmax labels=/opt/deeplabv3_resnet50.labels ! video/x-raw,width=256,height=144 ! queue ! mixer.
```

#### 3.2 插件 qtimlvclassification

qtimlvclassification 插件将来自 ML 推理插件（如 qtimlTFLite、qtimlsnpe 和 qtimlaic）的图像分类模型的输出张量处理为预测结果。 

协商的 GstCaps 决定了处理后的输出。它可以是以下任一项：

- 图像掩码（GstCaps：video/x-raw），可以使用 qtivcomposer 将其应用于原始图像。

- 包含预测结果的 GStreamer 格式文本（GstCaps：text/x-raw）。

qtimlvclassification 插件使用基于 CPU 的 Cairo 2D 图形库作为图像叠加掩码。图像叠加掩码使其能够在 ION/DMA 缓冲区中绘制预测结果。GstImageBufferPool 自定义缓冲池类通过 IOCTL 命令将 ION/DMA 缓冲区分配给内核。

在通用文本格式中，预测结果被解析为使用常规系统内存分配的缓冲区内的 GStreamer 格式的字符串。


##### 3.2.1 使用 snpe 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1,compression=ubwc ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::position="<30, 30>" sink_1::dimensions="<320, 180>" ! queue ! waylandsink fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimlsnpe delegate=dsp model=/opt/inceptionv3.dlc ! queue ! qtimlvclassification threshold=40.0 results=2 module=mobilenet labels=/opt/classification.labels ! video/x-raw,format=BGRA,width=640,height=360 ! queue ! mixer.
```

##### 3.2.2 使用 tflite 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::position="<30, 30>" sink_1::dimensions="<320, 180>" ! queue ! waylandsink fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=external external-delegate-path=libQnnTFLiteDelegate.so \
external-delegate-options="QNNExternalDelegate,backend_type=htp;" model=/opt/inceptionv3.tflite ! queue ! qtimlvclassification threshold=40.0 results=2 module=mobilenet labels=/opt/classification.labels ! video/x-raw,format=BGRA,width=640,height=360 ! queue ! mixer.
```

#### 3.3 插件 qtimlvdetection

qtimlvdetection 插件将来自 ML 推理插件（例如 qtimlTFLite、qtimlsnpe 和 qtimlaic）的目标检测模型的输出张量处理为预测结果。

协商的 GstCaps 决定了处理后的输出。输出可以是以下任一值：

- 图像叠加掩码（GstCaps：video/x-raw），可以使用 qtivcomposer 将其应用于原始图像。

- 包含预测结果的 GStreamer 格式文本（GstCaps：text/x-raw）。

qtimlvclassification 插件使用基于 CPU 的 Cairo 2D 图形库作为图像叠加掩码。图像叠加掩码使其能够在 ION/DMA 缓冲区中绘制预测结果。GstImageBufferPool 自定义缓冲池类通过 IOCTL 命令将 ION/DMA 缓冲区分配给内核。

在通用文本格式中，预测结果被解析为使用常规系统内存分配的缓冲区内的 GStreamer 格式的字符串。

##### 3.3.1 使用 snpe 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1,compression=ubwc ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::dimensions="<1920,1080>" ! queue ! waylandsink fullscreen=true sync=true split. ! queue ! qtimlvconverter ! queue ! qtimlsnpe delegate=dsp model=/opt/yolonas.dlc layers="</heads/Mul, /heads/Sigmoid>" ! queue ! qtimlvdetection threshold=51.0 results=10 module=yolo-nas labels=/opt/yolonas.labels ! video/x-raw,width=640,height=360 ! queue ! mixer.
```

##### 3.3.2 使用 tflite 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=2 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1920,height=1080,framerate=30/1 ! queue ! tee name=split ! queue ! qtimetamux name=metamux ! queue ! qtioverlay ! queue ! waylandsink sync=false fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=external external-delegate-path=libQnnTFLiteDelegate.so \
external-delegate-options="QNNExternalDelegate,backend_type=htp;" model=/opt/yolov5m-320x320-int8.tflite ! queue ! qtimlvdetection threshold=75.0 results=10 module=yolov5 labels=/opt/yolov5m.labels constants="YoloV5,q-offsets=<3.0>,q-scales=<0.005047998391091824>;" ! text/x-raw ! queue ! metamux.
```

#### 3.4 qtimlvpose

qtimlvpose 插件将来自 ML 推理插件（如 qtimlTFLite、qtimlsnpe 和 qtimlaic）的姿势估计模型的输出张量处理为预测结果。

插件输出上协商的 GstCaps 决定了处理后的输出，可以是以下任一：

- 可以使用 qtivcomposer 在原始图像上应用的图像掩码（GstCaps：video/x-raw）。

  - 该组件使用基于 CPU 的 Cairo 2D 图形库在 ION/DMA 缓冲区中绘制预测结果。

  - GstImageBufferPool 自定义缓冲池类通过 IOCTL 命令将 ION/DMA 缓冲区分配给内核。

- 包含预测结果的 GStreamer 格式文本（GstCaps：text/x-raw）。

预测结果被解析为使用常规系统内存分配的缓冲区内的 GStreamer 格式的字符串。

##### 3.4.1 使用 tflite 的用例

GStreamer 命令：

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtimetamux name=metamux ! queue ! qtioverlay ! queue ! waylandsink fullscreen=true sync=false split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=external external-delegate-path=libQnnTFLiteDelegate.so \
external-delegate-options="QNNExternalDelegate,backend_type=htp;" model=/opt/posenet_mobilenet_v1.tflite ! queue ! qtimlvpose threshold=51.0 results=2 module=posenet labels=/opt/posenet_mobilenet_v1.labels constants="Posenet,q-offsets=<128.0,128.0,117.0>,q-scales=<0.0784313753247261,0.0784313753247261,1.3875764608383179>;" ! text/x-raw ! queue ! metamux.
```

## Sense HAT

### 1. 简介

此指南针 Demo 基于 Raspberry Pi 的 Sense HAT。

### 2. 功能

此 Demo 使用 Sense HAT 的磁力计、8*8 RGB LED 矩阵和操控杆，模拟具备校准功能的简化指南针。

### 3. 开始操作

源文件 compass.c 已经存在于 RUBIK Pi 3 中，可以使用以下方法在 RUBIK Pi 3 上编译并验证：

> **注意：** 在运行 demo 前，需将 Sense HAT 连接到 RUBIK Pi 3 的 40-pin 接口。

```cmake
cd /usr/src/rubikpi-sense-hat-demo/compass/
gcc compass.c -lm -o compass
./compass # Run demo
```

校准方法：

a.按下 Sense HAT 上的操控杆；

b.匀速将 RUBIK Pi 水平旋转 360 度；

c.再次按下操控杆以完成校准。

---

## 🚀About RUBIK Pi 3

**🔖Shortcuts**

[官网](https://www.rubikpi.ai/) | [购买](https://www.thundercomm.com/product/rubik-pi/) | [文档及资源](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#%E6%96%87%E6%A1%A3%E5%8F%8A%E8%B5%84%E6%BA%90) | [产品优势](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#%E9%AD%94%E6%96%B9%E6%B4%BE3%E7%9A%84%E4%BC%98%E5%8A%BF) | [产品对比](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#%E4%BA%A7%E5%93%81%E5%AF%B9%E6%AF%94) | [产品规格书](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#%E9%AD%94%E6%96%B9%E6%B4%BE3%E4%BA%A7%E5%93%81%E8%A7%84%E6%A0%BC%E4%B9%A6) | [Demo](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#demo) | [FAQ](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#%EF%B8%8Ffaq) | [联系我们](https://github.com/rubikpi-ai/documentation/blob/main/README-CN.md#%E8%81%94%E7%B3%BB%E6%88%91%E4%BB%AC)

<div align="center">
<img src="https://github.com/rubikpi-ai/documentation/blob/main/media/rubik-pi-3.gif?raw=true" width="100%">
<br>
</div>

[魔方派3 （RUBIK Pi 3）](https://www.thundercomm.com/cn/product/rubik-pi/)，是采用[Qualcomm® QCS6490](https://www.qualcomm.com/products/internet-of-things/industrial/building-enterprise/qcs6490) 平台的轻量型开发板， 是首款基于高通 AI 平台打造的、支持开源 Qualcomm Linux 等多操作系统的面向开发者的“派”产品。

RUBIK Pi 3 可支持 Android / Linux / LU 多操作系统， Thundercomm 提供的多个 SDK 为开发者带来更完善的体验。 高通 QCS6490 具备 12 TOPS 的卓越 AI 性能， 配合[Qualcomm AI Hub](https://aihub.qualcomm.com/), 让开发者可亲自感受 AI 进化带来的冲击。

RUBIK Pi 3 具有丰富的接口和功能设计， 支持 USB Type-A (1x 2.0, 2x 3.0) 、USB 3.1 Gen 1 Type-C with DP (4K@60) 、Camera、HDMI OUT (4K@30) 、1000M 网口、 40pin 排针连接器、 3.5mm 耳机孔、Wi-Fi 5、BT 5.2、M.2 M-key 连接器、Micro USB 和 RTC 等，满足多样化的开发需求；同时接口还兼容树莓派（Raspberry Pi）的外设配件， 降低开发者使用难度和使用成本。

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-1.jpg" alt=""></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-2.jpg" alt=""></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-3.jpg" alt=""></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-4.jpg" alt=""></td>
    </tr>
</table>
</div>

---

### 😍魔方派3的优势

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td width="15%"><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/qualcomm-2.png" alt=""></td>
        <td width="35%"><strong>Qualcomm® Linux®</strong><br><p>基于高通 QCS6490 平台的首款面向开发者的开源硬件</p></td>
        <td width="15%"><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Compatibility-2.png" alt=""></td>
        <td width="35%"><strong>兼容性</strong><br><p>接口兼容树莓派5官方配件</p></td>
    </tr>
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Compact-2.png" alt=""></td>
        <td><strong>设计紧凑</strong><br><p>100mm x 75mm 的轻便设计，易于原型机开发和产品量产</p></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/ai-2.png" alt=""></td>
        <td><strong>AI 性能</strong><br><p>12 TOPS 端侧 AI 性能</p></td>
    </tr>
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Expand-2.png" alt=""></td>
        <td><strong>易于扩展</strong><br><p>• C5430P/C6490P/C8550 SOM 引脚兼容 | • 便于扩展不同性能的 EVK</p></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Community-1.png" alt=""></td>
        <td><strong>开源社区</strong><br><p>开发者进行交流、协作、互动的平台</p></td>
    </tr>
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Multiple-OS-support-1.png" alt=""></td>
        <td><strong>多操作系统支持</strong><br><p>Qualcomm open-source Linux, Android, Ubuntu on Qualcomm IoT Platform</p></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/folder-1.png" alt=""></td>
        <td><strong>完善的文档系统</strong><br><p>包括文档，使用指导，教程，数据手册等；便于开发者快速使用</p></td>
    </tr>
</table>
</div>

---

### 🏅产品对比

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
<tbody>
<tr class="firstRow" style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">Item</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">RUBIK Pi 3</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">***</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">***</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">***</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">SOC</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">Qualcomm - QCS 6490</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">BCM2712+Hailo 8L</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">BCM2712</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">RK 3588</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">Process</td>
<td class="c1" style="height: 23px; text-align: center; vertical-align: middle;"><span style="color: #0071e3;"><strong>6nm</strong></span></td>
<td style="height: 23px; text-align: center; vertical-align: middle;">16nm</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">16nm</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">8nm</td>
</tr>
<tr style="height: 95px;">
<td style="height: 95px; text-align: center; vertical-align: middle;">CPU</td>
<td class="c1" style="height: 95px; text-align: center; vertical-align: middle;"><strong><span style="color: #0071e3;">CPU670 built on Arm V8</span></strong><br>
<strong><span style="color: #0071e3;">1 x GoldP@2.7GHz</span></strong><br>
<strong><span style="color: #0071e3;">3 x Gold@2.4GHz</span></strong><br>
<strong><span style="color: #0071e3;">4 x Silver@1.9GHz</span></strong></td>
<td style="height: 95px; text-align: center; vertical-align: middle;">4 x Arm A76@2.4GHz</td>
<td style="height: 95px; text-align: center; vertical-align: middle;">4 x Arm A76@2.4GHz</td>
<td style="height: 95px; text-align: center; vertical-align: middle;">4 x Arm A76@2.4GHz<br>
4 x Arm A55@1.8GHz</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">GPU</td>
<td class="c1" style="height: 23px; text-align: center; vertical-align: middle;"><span style="color: #0071e3;"><strong>Adreno 643</strong></span></td>
<td style="height: 23px; text-align: center; vertical-align: middle;">VideoCore VII</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">VideoCore VII</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">Mali G610MP4</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">AI</td>
<td class="c1" style="height: 23px; text-align: center; vertical-align: middle;"><span style="color: #0071e3;"><strong>12 Tops</strong></span></td>
<td style="height: 23px; text-align: center; vertical-align: middle;">13 Tops</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">Null</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">6 Tops</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">RAM</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">8GB LPDDR4X</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">8GB LPDDR4X</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">8GB LPDDR4X</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">8GB LPDDR4/4X</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">ROM</td>
<td class="c1" style="height: 23px; text-align: center; vertical-align: middle;"><strong><span style="color: #0071e3;">128GB UFS2.x</span></strong></td>
<td style="height: 23px; text-align: center; vertical-align: middle;" colspan="3">NA (Need to purchase external SD card separately)</td>
</tr>
<tr style="height: 23px;">
<td style="height: 23px; text-align: center; vertical-align: middle;">Size</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">75mm x 100mm</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">85mm x 56mm</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">85mm x 56mm</td>
<td style="height: 23px; text-align: center; vertical-align: middle;">75mm x 100mm</td>
</tr>
</tbody>
</table>
</div>

> 注册商标 Linux® 的使用会依据 Linux 基金会授予的再许可，Linux 基金会是该商标在全球范围内的所有者 Linus Torvalds 的独家受许可人。

---

### 📒魔方派3产品规格书

<div align="center">

<table  style="border-collapse: collapse; border: none;width:100%;">
<tbody>
<tr class="firstRow">
<td valign="top" width="30%">Category</td>
<td valign="top" width="70%">RUBIK Pi 3 Feature　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　</td>
</tr>
<tr>
<td valign="top">Platform</td>
<td valign="top">Qualcomm<sup>®</sup> QCS6490</td>
</tr>
<tr>
<td colspan="1" rowspan="1" valign="top">Memory</td>
<td colspan="1" rowspan="1" valign="top">RAM 8 GB LPDDR4x<br>
ROM 128 GB UFS 2.2</td>
</tr>
<tr>
<td valign="top">Video</td>
<td valign="top">1 x HDMI 1.4 output (up to 4K 30 Hz)<br>
1 x DP over USB Type-C (up to 4K 60 Hz)<br>
2 x camera connector (4-lane MIPI CSI D-PHY)</td>
</tr>
<tr>
<td valign="top">Audio</td>
<td valign="top">1 x 3.5mm headphone jack</td>
</tr>
<tr>
<td valign="top">Connectivity</td>
<td valign="top">1 x USB Type-C (USB 3.1 Gen 1)<br>
2 x USB Type-A (USB 3.0)<br>
1 x USB Type-A (USB 2.0)<br>
1 x 1000M Ethernet (RJ45)<br>
1 x UART for debug (over Micro USB)<br>
1 x M.2 Key M connector (PCIe 3.0 2-lane)<br>
<b>40-pin LS connector supporting various interface options:<br>
</b>- Up to 28 x GPIO<br>
- Up to 2 x I2C<br>
- Up to 3 x UART<br>
- Up to 3 x SPI<br>
- 1 x I2S (PCM)<br>
- 1 x PWM channel</td>
</tr>
<tr>
<td valign="top">Others</td>
<td valign="top">1 x Power on button<br>
1 x EDL button<br>
1 x RGB LED<br>
2-pin RTC battery connector<br>
4-pin PWM fan connector</td>
</tr>
<tr>
<td valign="top">Wireless connection</td>
<td valign="top">Wi-Fi: IEEE 802.11 a/b/g/n/ac<br>
Bluetooth: BT 5.2<br>
On-board PCB antenna</td>
</tr>
<tr>
<td valign="top">Power supply</td>
<td>Power Delivery over Type-C, 12V 3A</td>
</tr>
<tr>
<td valign="top">Operating environment</td>
<td>Operating temperature: 0 ~ 70°C</td>
</tr>
<tr>
<td valign="top">Dimensions</td>
<td valign="top">100mm x 75mm x 25mm</td>
</tr>
<tr>
<td valign="top">OS support</td>
<td valign="top">Android 13<br>
Qualcomm® Linux®<br>
*Canonical Ubuntu for Qualcomm platforms</td>
</tr>
</tbody>
</table>

</div>

*Planning

---

### 📚文档及资源

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td style="text-align:center;" width="33%" align="center">　<a href="https://www.thundercomm.com/rubik-pi-3/cn/docs/rubik-pi-3-user-manual" target="_blank" ><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-user-manual-icon-1.png" alt="" width="50%"></a>　</td>
        <td style="text-align:center;" width="33%" align="center">　<a href="https://www.thundercomm.com/rubik-pi-3/cn/docs/hardware-resources" target="_blank" ><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-datasheet-icon-1.png" alt="" width="50%"></a>　</td>
        <td style="text-align:center;" width="33%" align="center">　<a href="https://www.thundercomm.com/rubik-pi-3/cn/docs/image" target="_blank" ><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-image-iconpng-1.png" alt="" width="50%"></a>　</td>
    </tr>
    <tr>
        <td style="text-align:center;"  align="center"><a href="https://www.thundercomm.com/rubik-pi-3/cn/docs/rubik-pi-3-user-manual" target="_blank">用户手册</a></td>
        <td style="text-align:center;"  align="center"><a href="https://www.thundercomm.com/rubik-pi-3/cn/docs/hardware-resources" target="_blank">硬件资料</a></td>
        <td style="text-align:center;"  align="center"><a href="https://www.thundercomm.com/rubik-pi-3/cn/docs/image" target="_blank">系统镜像</a></td>
    </tr>
</table>
</div>

---

### 🤖Demo

1.uHand Demo👋

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td  style="vertical-align: top;">uHand Demo 利用 RUBIK Pi 3 卓越的设备端 AI 性能和灵活的集成能力，重点展示了多种先进的设备端 CV/ML 算法：</td>
        <td width="25%"><img src="https://github.com/rubikpi-ai/documentation/blob/main/media/uhand.png?raw=true" width="100%"></td>
    </tr>
    <tr >
        <td colspan="2"><img src="https://github.com/rubikpi-ai/documentation/blob/main/media/uhand-2.png?raw=true" width="100%"></td>
    </tr>
</table>
</div>

2.AI Camera📷

<div align="center">

<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td  style="vertical-align: top;">这款基于RUBIK Pi 3的宝丽来相机提供了便捷、智能的成像体验。这款相机在教育应用方面表现优异，尤其在语言学习场景中，让学生能够拍摄日常物品和场景，快速识别图像并呈现分析结果。
</td>
        <td width="25%"><img src="https://github.com/rubikpi-ai/documentation/blob/main/media/ai-camera.png?raw=true" width="100%"></td>
    </tr>   
</table>

</div>

---

### 🙋‍♂️FAQ

> **1.魔方派 3（RUBIK Pi 3）体验版和正式版有什么区别？**
>
> -- 体验版软硬件可能存在不稳定性，部分功能可能尚不完善，可能会出现Bug。正式版目前开放购买，预计发货时间为2025年3月。

---

### 📫联系我们

获取支持：[support@rubikpi.ai](mailto:support@rubikpi.ai)

Bug反馈： [https://github.com/rubikpi-ai/documentation/issues/new](https://github.com/rubikpi-ai/documentation/issues/new)
