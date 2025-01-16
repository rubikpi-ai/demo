<div align="center">

<img src="https://github.com/rubikpi-ai/documentation/blob/1ba7e3b6a60915213496a6cc06ea8ad3ce2c0d8c/media/RUBIK-Pi-3-Logo.png?raw=true" width="50%">

<h1>Demo</h1>
------------------------
<br>
  [English] | [<a href="https://github.com/rubikpi-ai/demo/blob/main/README-CN.md">简体中文</a>] 
<br><br>
</div>

This repository contains some small demos running on RUBIK Pi.

## QIM

### 1. What is QIM

The Qualcomm® intelligent multimedia software development kit (QIM SDK) is a unified SDK across Internet of Things (IoT) segments enabling seamless multimedia and AI/ML application deployment. This SDK uses GStreamer, an open-source multimedia framework, and exposes easy APIs and plug-ins in both multimedia and machine learning (ML) domains.

These plug-ins enable application developers to develop various multimedia and AI/ML applications across various segments such as smart/connected/IP/sports/web cameras, robotics/drones, AI, or ML boxes.

The application can be single/multi-stream multimedia, ML, or a combination of both. This SDK enables flexible construction of use cases/pipelines by providing a catalog of Qualcomm plug-ins with hardware acceleration. These plug-ins are optimized to run on Qualcomm hardware/IPs and enable the end application to run in the most efficient way. The plug-ins constitute video encode/decode, camera ISP, GPU, display, audio DSP (aDSP), compute DSP (cDSP), and AI/ML accelerators.

This plug-in style framework allows the developer to plug and play various types of plug-ins to build specific multimedia or AI/ML applications with ease of use and flexibility. Developers do not need to understand low-level platform libraries and hardware details, which can vary across chipsets. This SDK hides the complexity of hardware within the plug-in architecture and provides easy APIs to applications so that applications built on top of this SDK can run seamlessly in different versions or different tiers of the chipset.

### 2. Use sample applications in QIM

#### 2.1 Before using sample applications:

1.Connect the camera and HDMI OUT cable to RUBIK Pi 3.

2.Run the following commands after flashing the image each time:

```cmake
adb root
adb shell mount -o rw,remount /
adb shell "echo multiCameraLogicalXMLFile=kodiak_dc.xml >> /var/cache/camera/camxoverridesettings.txt"
adb shell "echo enableNCSService=FALSE >> /var/cache/camera/camxoverridesettings.txt"
```

3.Push the model files in the RubikPi_models_for_sample_applications zip folder under the QIM directory to the /opt directory of RUBIK Pi 3.

```cmake
adb push ./* /opt
```

#### 2.2 Use executable sample applications

##### 2.2.1 gst-ai-classification

The gst-ai-classification application enables you to identify the subject in an image. The use cases use either the Qualcomm Neural Processing SDK runtime or TensorFlow Lite (TFLite) runtime.

###### 2.2.1.1 Use cases

The following list provides the commands to run the application for various use cases:

- Run using Qualcomm Neural Processing SDK runtime:

```cmake
gst-ai-classification --ml-framework=1 --model=/opt/inceptionv3.dlc --labels=/opt/classification.labels
```

- Run using TFLite runtime:

```javascript
gst-ai-classification  -t 2 --ml-framework=2 --model=/opt/inception_v3_quantized.tflite  --labels=/opt/classification.labels
```

##### 2.2.2 gst-ai-object-detection

The gst-ai-object-detection application enables you to detect objects within images and videos. The use case shows the execution of [YOLO-NAS](https://github.com/Deci-AI/super-gradients/blob/master/YOLONAS.md) using the Qualcomm Neural Processing SDK runtime.

###### 2.2.2.1 Use case

- YOLO-NAS using Qualcomm Neural Processing SDK runtime with input from a camera. 

```cmake
gst-ai-object-detection --ml-framework=1 --model=/opt/yolonas.dlc --labels=/opt/yolonas.labels
```

##### 2.2.3 gst-ai-pose-detection

The gst-ai-pose-detection application enables you to detect the body pose of the subject in an image or video. The use case uses an input stream from a camera and displays the results on the screen.

###### 2.2.3.1 Use case

- The use case enables you to run the inference using TFLite external delegate - Hexagon Tensor Processor.

```cmake
gst-ai-pose-detection --model=/opt/posenet_mobilenet_v1.tflite --labels=/opt/posenet_mobilenet_v1.labels
```

##### 2.2.4 gst-ai-segmentation

The gst-ai-segmentation application enables you to divide an image into different and meaningful parts or segments and assign a label to each homogenous segment based on the similarity of the attributes. The application shows how to use Qualcomm Neural Processing SDK runtime.

###### 2.2.4.1 Use case

- Run the deeplabv3_resnet50 model using Qualcomm Neural Processing SDK runtime from the primary camera:

```cmake
gst-ai-segmentation --ml-framework=1 --model=/opt/deeplabv3_resnet50.dlc --labels=/opt/deeplabv3_resnet50.labels
```

##### 2.2.5 gst-ai-parallel-inference

The gst-ai-parallel-inference application enables you to perform object detection, object classification, pose detection, and image segmentation on an input stream from different sources such as a camera. The use case uses the Qualcomm Neural Processing SDK runtime for object detection and image segmentation, and TFLite runtime for classification and pose detection.

###### 2.2.5.1 Use case

- This sample application demonstrates Classification, Segmentation, Object Detection, Pose Detection On Live Stream and outputs 4 parallel streams.

```cmake
gst-ai-parallel-inference
```

##### 2.2.6 gst-ai-multi-input-output-object-detection

The gst-ai-multi-input-output-object-detection application enables you to perform object detection on multiple video streams from different sources, such as a camera or a file.

###### 2.2.6.1 Use cases

- Run the AI object detection on streams from two cameras:

```cmake
gst-ai-multi-input-output-object-detection --num-camera=2 --display --model=/opt/yolov5.tflite --labels=/opt/yolov5.labels
```

- Run AI object detection on streams from a file source:

```cmake
gst-ai-multi-input-output-object-detection --num-file=2 --display --model=/opt/yolov5.tflite --labels=/opt/yolov5.labels
```

The default source path for the video files is /opt/video1.mp4 and /opt/video2.mp4

##### 2.2.7 gst-ai-daisychain-detection-classification 

The gst-ai-daisychain-detection-classification application enables you to perform cascaded object detection and classification with a camera or file source. The use case involves detecting objects and classifying the detected objects.

###### 2.2.7.1 Use case

- The use cases enable you to run the application using a camera stream. The output can be viewed on the Wayland display:

```cmake
gst-ai-daisychain-detection-classification --camera
```

- The use cases enable you to run the application using a file stream. The output can be viewed on the Wayland display:

```cmake
gst-ai-daisychain-detection-classification --file=/opt/video.mp4
```

##### 2.2.8 gst-ai-monodepth

The gst-ai-monodepth application enables you to infer the depth of a source feed from a live camera stream.

###### 2.2.8.1 Use case

- This sample application demonstrates Monodepth on Live Stream.

```cmake
gst-ai-monodepth --ml-framework=1 --model=/opt/midasv2.dlc --labels=/opt/monodepth.labels
```

### 3. Use AI-related GStreamer plug-ins in QIM

#### 3.1 qtimlvsegmentation

The qtimlvsegmentation element processes output tensors of an image segmentation/depth estimation model from ML inference plug-in (such as qtimlTFLite, qtimlsnpe and qtimlaic) into result of predictions. 

The processed output is always an image mask (GstCaps: video/x-raw) with dimensions and format determined by the negotiated plug-in output GstCaps which can be applied over the original image using qtivcomposer. 

For this mask, the element will leverage the CPU-based Cairo 2D graphics library to draw the prediction results in ION/DMA buffers allocated by the custom buffer pool class GstImageBufferPool through IOCTL commands to the kernel.

##### 3.1.1 Use case of snpe

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1920,height=1080,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::dimensions="<1920,1080>" sink_1::alpha=0.5 ! queue ! waylandsink sync=false fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimlsnpe delegate=dsp model=/opt/deeplabv3_resnet50.dlc ! queue ! qtimlvsegmentation module=deeplab-argmax labels=/opt/deeplabv3_resnet50.labels ! video/x-raw,width=256,height=144 ! queue ! mixer.
```

##### 3.1.2 Use case of tflite

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::alpha=0.5 ! queue ! waylandsink fullscreen=true sync=false split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=gpu model=/opt/deeplabv3_resnet50.tflite ! queue ! qtimlvsegmentation module=deeplab-argmax labels=/opt/deeplabv3_resnet50.labels ! video/x-raw,width=256,height=144 ! queue ! mixer.
```

#### 3.2 qtimlvclassification

The qtimlvclassification element processes output tensors of an Image Classification model from ML inference plug-in (such as qtimlTFLite, qtimlsnpe and qtimlaic) into result of predictions. 

The processed output is determined by the negotiated GstCaps on the plug-in output. It can be either an image mask (GstCaps: video/x-raw) which for example can be applied over the original image using qtivcomposer or GStreamer formatted text (GstCaps: text/x-raw) containing the prediction results. 

For the image overlay mask, the element will leverage the CPU-based Cairo 2D graphics library to draw the prediction results in ION/DMA buffers allocated by the custom buffer pool class GstImageBufferPool through IOCTL commands to the kernel. While in the versatile text format, the prediction results will be parsed into GStreamer formatted string inside buffers allocated using regular system memory.

##### 3.2.1 Use case of snpe

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1,compression=ubwc ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::position="<30, 30>" sink_1::dimensions="<320, 180>" ! queue ! waylandsink fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimlsnpe delegate=dsp model=/opt/inceptionv3.dlc ! queue ! qtimlvclassification threshold=40.0 results=2 module=mobilenet labels=/opt/classification.labels ! video/x-raw,format=BGRA,width=640,height=360 ! queue ! mixer.
```

##### 3.2.2 Use case of tflite

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::position="<30, 30>" sink_1::dimensions="<320, 180>" ! queue ! waylandsink fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=external external-delegate-path=libQnnTFLiteDelegate.so \
external-delegate-options="QNNExternalDelegate,backend_type=htp;" model=/opt/inceptionv3.tflite ! queue ! qtimlvclassification threshold=40.0 results=2 module=mobilenet labels=/opt/classification.labels ! video/x-raw,format=BGRA,width=640,height=360 ! queue ! mixer.
```

#### 3.3 qtimlvdetection

The qtimlvdetection element processes output tensors of an Object Detection model from ML inference plug-in (such as qtimlTFLite, qtimlsnpe and qtimlaic) into result of predictions. 

The processed output is determined by the negotiated GstCaps on the plug-in output. It can be either an image mask (GstCaps: video/x-raw) which for example can be applied over the original image using qtivcomposer or GStreamer formatted text (GstCaps: text/x-raw) containing the prediction results. 

For the image overlay mask, the element will leverage the CPU-based Cairo 2D graphics library to draw the prediction results in ION/DMA buffers allocated by the custom buffer pool class GstImageBufferPool through IOCTL commands to the kernel. While in the versatile text format, the prediction results will be parsed into GStreamer formatted string inside buffers allocated using regular system memory.

##### 3.3.1 Use case of snpe

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1,compression=ubwc ! queue ! tee name=split split. ! queue ! qtivcomposer name=mixer sink_1::dimensions="<1920,1080>" ! queue ! waylandsink fullscreen=true sync=true split. ! queue ! qtimlvconverter ! queue ! qtimlsnpe delegate=dsp model=/opt/yolonas.dlc layers="</heads/Mul, /heads/Sigmoid>" ! queue ! qtimlvdetection threshold=51.0 results=10 module=yolo-nas labels=/opt/yolonas.labels ! video/x-raw,width=640,height=360 ! queue ! mixer.
```

##### 3.3.2 Use case of tflite

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=2 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1920,height=1080,framerate=30/1 ! queue ! tee name=split ! queue ! qtimetamux name=metamux ! queue ! qtioverlay ! queue ! waylandsink sync=false fullscreen=true split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=external external-delegate-path=libQnnTFLiteDelegate.so \
external-delegate-options="QNNExternalDelegate,backend_type=htp;" model=/opt/yolov5m-320x320-int8.tflite ! queue ! qtimlvdetection threshold=75.0 results=10 module=yolov5 labels=/opt/yolov5m.labels constants="YoloV5,q-offsets=<3.0>,q-scales=<0.005047998391091824>;" ! text/x-raw ! queue ! metamux.
```

#### 3.4 qtimlvpose

The qtimlvpose element processes output tensors of a Pose Estimation model from ML inference plug-in (such as qtimlTFLite, qtimlsnpe and qtimlaic) into result of predictions. 

The processed output is determined by the negotiated GstCaps on the plug-in output. It can be either an image mask (GstCaps: video/x-raw) which for example can be applied over the original image using qtivcomposer or GStreamer formatted text (GstCaps: text/x-raw) containing the prediction results. 

For the image overlay mask, the element will leverage the CPU-based Cairo 2Dgraphics library to draw the prediction results in ION/DMA buffers allocated by the custom buffer pool class GstImageBufferPool through IOCTL commands to the kernel. While in the versatile text format, the prediction results will be parsed into GStreamer formatted string inside buffers allocated using regular system memory.

##### 3.4.1 Use case of tflite

GStreamer command:

```cmake
export XDG_RUNTIME_DIR=/dev/socket/weston
export WAYLAND_DISPLAY=wayland-1
setprop persist.overlay.use_c2d_blit 2
gst-launch-1.0 -e --gst-debug=1 qtiqmmfsrc name=camsrc camera=0 ! video/x-raw\(memory:GBM\),format=NV12,width=1280,height=720,framerate=30/1 ! queue ! tee name=split split. ! queue ! qtimetamux name=metamux ! queue ! qtioverlay ! queue ! waylandsink fullscreen=true sync=false split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=external external-delegate-path=libQnnTFLiteDelegate.so \
external-delegate-options="QNNExternalDelegate,backend_type=htp;" model=/opt/posenet_mobilenet_v1.tflite ! queue ! qtimlvpose threshold=51.0 results=2 module=posenet labels=/opt/posenet_mobilenet_v1.labels constants="Posenet,q-offsets=<128.0,128.0,117.0>,q-scales=<0.0784313753247261,0.0784313753247261,1.3875764608383179>;" ! text/x-raw ! queue ! metamux.
```

## Sense HAT

### 1. Introduction

This is a compass demo based on the Sense HAT of Raspberry Pi.

### 2. Functionality

This demo utilizes the magnetometer, 8*8 RGB LED matrix, and joystick on the Sense HAT to form a simplified compass with calibration functionality.

### 3. Get Started

The source file compass.c already exists in RUBIK Pi 3 and can be compiled and verified in RUBIK Pi 3 using the following method:

> **Note:** Before running the demo, the Sense HAT needs to be connected to the 40-pin connector of RUBIK Pi 3.

```cmake
cd /usr/src/rubikpi-sense-hat-demo/compass/
gcc compass.c -lm -o compass
./compass # Run demo
```

Calibration method: 

a.Press the joystick on the Sense HAT.

b.Rotate RUBIK Pi 360 degrees horizontally at a constant speed.

c.Press the joystick again to complete the calibration.


---

## 🚀About RUBIK Pi 3

**🔖Shortcuts**

[Website](https://www.rubikpi.ai/) | [Purchase](https://www.thundercomm.com/product/rubik-pi/) | [Documents and Resources](https://github.com/rubikpi-ai/documentation?tab=readme-ov-file#documents-and-resources)  | [Advantage](https://github.com/rubikpi-ai/documentation/tree/main#rubik-pi-3-advantage) | [Product Comparison](https://github.com/rubikpi-ai/documentation/tree/main#product-comparison) | [Product Specifications](https://github.com/rubikpi-ai/documentation/tree/main#rubik-pi-3-specifications) | [Demo](https://github.com/rubikpi-ai/documentation/tree/main#demo) | [FAQ](https://github.com/rubikpi-ai/documentation/tree/main#faq) | [Contact Us](https://github.com/rubikpi-ai/documentation/tree/main#contact-us)

<div align="center">
<img src="https://github.com/rubikpi-ai/documentation/blob/main/media/rubik-pi-3.gif?raw=true" width="100%">
<br>
</div>

[RUBIK Pi 3](https://www.thundercomm.com/product/rubik-pi/), a lightweight development board based on [Qualcomm® QCS6490](https://www.qualcomm.com/products/internet-of-things/industrial/building-enterprise/qcs6490) platform, is the first Pi built on Qualcomm AI platforms for developers.

RUBIK Pi 3 supports multiple operating systems such as Android, Linux, and LU, with SDKs from Thundercomm to provide developers with a better experience. With excellent AI performance at 12 TOPS, Qualcomm® QCS6490 can be paired with [Qualcomm AI Hub](https://aihub.qualcomm.com/) to allow developers to personally experience the impact of AI evolution.

RUBIK Pi 3 boasts a rich array of interfaces and functions, including USB Type-A (1x 2.0, 2x 3.0), USB 3.1 Gen 1 Type-C with DP (4K@60), camera, HDMI OUT (4K@30), 1000M Ethernet port, 40-pin header connector, 3.5mm headphone jack, Wi-Fi 5, BT 5.2, M.2 M-key connector, Micro USB, and RTC, to meet diverse development needs. Its interfaces are compatible with Raspberry Pi peripheral accessories, which helps reduce implementation difficulty and cost for developers.

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

### 😍RUBIK Pi 3 Advantage

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td width="15%"><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/qualcomm-2.png" alt=""></td>
        <td width="35%"><strong>Qualcomm® Linux®</strong><br><p>First open-source Linux based on Qualcomm® QCS 6490 platform for developers</p></td>
        <td width="15%"><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Compatibility-2.png" alt=""></td>
        <td width="35%"><strong>Compatibility</strong><br><p>Compatible with Raspberry Pi 5 official accessories</p></td>
    </tr>
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Compact-2.png" alt=""></td>
        <td><strong>Compact Design</strong><br><p>With a portable size of 100mm x 75mm, enabling easy direct application in products</p></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/ai-2.png" alt=""></td>
        <td><strong>AI Capability</strong><br><p>12 TOPS On-device AI capability</p></td>
    </tr>
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Expand-2.png" alt=""></td>
        <td><strong>Easy to Expand</strong><br><p>• C5430P/6490P/C8550 SOM are P2P compatible | • Easy to build customized EVK</p></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Community-1.png" alt=""></td>
        <td><strong>Open-source Community</strong><br><p>A vibrant and collaborative group of enthusiasts, developers, and educators</p></td>
    </tr>
    <tr>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/Multiple-OS-support-1.png" alt=""></td>
        <td><strong>Multiple OS support</strong><br><p>Qualcomm open-source Linux, Android, Ubuntu on Qualcomm IoT Platform</p></td>
        <td><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/folder-1.png" alt=""></td>
        <td><strong>Comprehensive Documentation</strong><br><p>Accessible documentations, including official guides, tutorials, datasheets, and FAQs</p></td>
    </tr>
</table>
</div>

---

### 🏅Product Comparison

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

> The registered trademark Linux® is used pursuant to a sublicense from the Linux Foundation, the exclusive licensee of Linus Torvalds, owner of the mark on a worldwide basis.

---

### 📒RUBIK Pi 3 Specifications

<div align="center">

<table  style="border-collapse: collapse; border: none;width:100%;">
<tbody>
<tr class="firstRow">
<td valign="top" width="30%">Category</td>
<td valign="top" width="70%">RUBIK Pi 3 Feature　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　</td>
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

### 📚Documents and Resources

<div align="center">
<table  style="border-collapse: collapse; border: 0;width:100%;">
    <tr>
        <td style="text-align:center;" width="33%" align="center" border='0'>　<a href="https://www.thundercomm.com/rubik-pi-3/en/docs/rubik-pi-3-user-manual" target="_blank" ><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-user-manual-icon-1.png" alt="" width="50%"></a>　</td>
        <td style="text-align:center;" width="33%" align="center">　<a href="https://www.thundercomm.com/rubik-pi-3/en/docs/hardware-resources" target="_blank" ><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-datasheet-icon-1.png" alt="" width="50%"></a>　</td>
        <td style="text-align:center;" width="33%" align="center">　<a href="https://www.thundercomm.com/rubik-pi-3/en/docs/image" target="_blank" ><img src="https://www.thundercomm.com/wp-content/uploads/2024/09/rubik-pi-3-image-iconpng-1.png" alt="" width="50%"></a>　</td>
    </tr>
    <tr>
        <td style="text-align:center;"  align="center"><a href="https://www.thundercomm.com/rubik-pi-3/en/docs/rubik-pi-3-user-manual" target="_blank">User Manual</a></td>
        <td style="text-align:center;"  align="center"><a href="https://www.thundercomm.com/rubik-pi-3/en/docs/hardware-resources" target="_blank">Hardware Resources</a></td>
        <td style="text-align:center;"  align="center"><a href="https://www.thundercomm.com/rubik-pi-3/en/docs/image" target="_blank">System Image</a></td>
    </tr>
</table>
</div>

---

### 🤖Demo

1.uHand Demo👋

<div align="center">
<table  style="border-collapse: collapse; border: none;width:100%;">
    <tr>
        <td  style="vertical-align: top;">The uHand Demo leverages the excellent on-device AI performance and flexible integration capabilities of RUBIK Pi 3 to showcase various advanced on-device CV/ML algorithms.</td>
        <td width="25%"><img src="https://github.com/rubikpi-ai/documentation/blob/main/media/uhand.png?raw=true" width="100%"></td>
    </tr>
    <tr >
        <td colspan="2"><img src="https://github.com/rubikpi-ai/documentation/blob/main/media/uhand-2.png?raw=true" width="100%"></td>
    </tr>
</table>
</div>

---

### 🙋‍♂️FAQ

> **1.What is the difference between the RUBIK Pi 3 Trial Edition and the Official Edition?**
>
> -- The Trial Edition may have software and hardware instability, with some features potentially being incomplete and bugs possibly occurring. The Official Edition is now available for purchase, with an estimated shipping time in March 2025.

---

### 📫Contact Us

Support: [support@rubikpi.ai](mailto:support@rubikpi.ai)

Report Bugs: [https://github.com/rubikpi-ai/documentation/issues/new](https://github.com/rubikpi-ai/documentation/issues/new)
