#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "speed_control_header.h"

float x,y,z,th_x,th_y,th_z;
float re_x,re_y,re_z,re_th_x,re_th_y,re_th_z;

//接受到订阅的消息后，会进入消息回调函数
void teleopCallback(const geometry_msgs::Twist& msg)
{
	
	x = msg.linear.x;
	y = msg.linear.y;
        th_z = msg.angular.z;

	//ROS_INFO("\nliner:\nx:%.3f\ny:%.3f\nz:%.3f\nangler:\nx:%.3f\ny:%.3f\nz:%.3f\n",x,y,z,th_x,th_y,th_z);

	write(x,y,th_z);

	read(re_x,re_y,re_th_z);

	ROS_INFO("receive:\nliner:\nx:%.3f\ny:%.3f\nz:%.3f\nangler:\nx:%.3f\ny:%.3f\nz:%.3f\n",re_x,re_y,re_z,re_th_x,re_th_y,re_th_z);
}

int main(int argc, char **argv)
{
	//初始化ROS节点
	ros::init(argc, argv, "speed_control");

	//创建句柄节点
	ros::NodeHandle n;

	//设置延时时间
	ros::Rate loop_rate(1);

	//串口初始化，引用自头文件learn_linux_serial.h，即获取同名cpp文件中的serialInit()函数
	serialInit();

	//创建一个Subscriber，订阅名为/person_info的topic,注册回调函数personInfoCallback
	ros::Subscriber person_info_sub = n.subscribe("/cmd_vel",10,teleopCallback);

	//循环等待函数
	ros::spin();

	return 0;
}
