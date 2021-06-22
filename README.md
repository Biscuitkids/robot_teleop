# robot_teleop
ROS机器人系统与STM32开发板通讯

  //**********************************************
  
  实现步骤一：向STM32传输字符，点亮小灯。
  
  ************************************************//
  
  首先你需要安装串口通讯的功能包，以能够引用串口通讯库。
  
  ` sudo apt-getinstall ros-melodic-serial`
  
  安装好后我们就可以开始我们的配置了。
  
  首先我们可以先建立一个自定义的头文件，以是我们的主程序程序变得整洁。暂且先命名为：serial_header.cpp 和 serial_header.h。
  ##serial_header.cpp
  
  第一步：引用头文件
  
  ` #include "slam_car_control_with_imu_header.h" `
  
  ` using namespace std; `
  
  ` using namespace boost::asio; `
