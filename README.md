# Wireless-ollection-Of-CO
# 基于STC单片机的一氧化碳无线采集系统

---
## 使用部件：
+ ### 主控：    stc12系列单片机
+ ### 无线模块：NRF24L01
+ ### 一氧化碳传感器：MQ-7
+ ### 显示模块：OLED12864
+ ### 时钟模块：DS1302
+ ### 外部存储：24C02
+ ### 其他部件：蜂鸣器，LED
---
## 编译环境：win10 Keil4  
---

## 文件使用：
>### main.c是运行文件。
>### TX1,TX2是两个从机main文件。
>### RX是主机main文件。
>### 使用方法：为不同芯片下载程序的时候，只需要把把对应程序替换到主main文件就可以。如果需要多从机，只需要把对应从机main文件多复制几份然后修改里面的发送标志就可以了。
---
### 引脚设置在对应的头文件里面。
---

![image1](https://github.com/omega-Lee/Wireless-collection-of-CO/blob/master/image/image1.jpg "image1")  
![image2](https://github.com/omega-Lee/Wireless-collection-of-CO/blob/master/image/image2.jpg "image2")

### 欢迎给小星星哦:kissing_heart::kissing_heart::kissing_heart:
