## 一、安装过程 
### 1.添加source.list
* 配置电脑使其能够安装来自 packages.ros.org的软件包
`sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'`
### 2.添加keys
* 指令如下
`sudo apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-key 0xB01FA116`
### 3.环境配置及安装
* 首先，确保虚拟机Debian软件包索引是最新的
`sudo apt-get update`
* 桌面完整版安装： 包含ROS、rqt、rviz、通用机器人函数库、2D/3D仿真器、导航以及2D/3D感知功能
`sudo apt-get install ros-jade-desktop-full`
* 初始化rosdep,rosdep可以方便在需要编译某些源码的时候为其安装一些系统依赖，同时也是某些ROS核心功能组件所必需用到的工具
`sudo rosdep init`
 `rosdep update`
* 环境配置：如果每次打开一个新的终端时ROS环境变量都能够自动配置好（即添加到bash会话中），那将会方便很多
`echo "source /opt/ros/jade/setup.bash" >> ~/.bashrc`
 `source ~/.bashrc`
* 如果安装有多个ROS版本, ~/.bashrc 必须只能 source 你当前使用版本所对应的 setup.bash。

* 如果只想改变当前终端下的环境变量，可以执行以下命令
 `source /opt/ros/jade/setup.bash`
* 安装rosinstall
`sudo apt-get install python-rosinstall`

## 二、测试ROS
### 1.运行Beginner Tutorials的简单的示例程序
* 打开第一个终端输入命令`roscore`
* 打开第二个终端输入命令`rosrun turtlesim turtlesim_node`开启小乌龟界面，界面显示如下
![enter description here][1]
* 打开第三个终端输入命令`rosrun turtlesim turtle_teleop_key`接收键盘输入，选中这个终端，键盘操控上下左右键可控制小乌龟的移动，结果如下图：
![enter description here][2]
* 打开第四个终端，输入指令`rosrun rqt_graph rqt_graph`，可以看到ROS node和Topics等图形展示，出现界面如下
![enter description here][3]

## 三、实验感想
这次的实验是进行ROS的安装和配置，ROS全称是Robot operation system，底层提供硬件驱动，软件层面支持通用的文件格式，主要用于仿真功能。安装过程跟着教程走没什么问题，测试过程也挺顺利的。


  [1]: ./images/xiaowugui.PNG "xiaowugui.PNG"
  [2]: ./images/yidong.PNG "yidong.PNG"
  [3]: ./images/tubiao.PNG "tubiao.PNG"