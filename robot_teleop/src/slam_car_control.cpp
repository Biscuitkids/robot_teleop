#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "slam_car_control_header.h"

float x,y,z,th_x,th_y,th_z;
float re_x,re_y,re_z,re_th_x,re_th_y,re_th_z;
bool recall_vel;

geometry_msgs::Twist twist;
geometry_msgs::Vector3 linear;
geometry_msgs::Vector3 angular;

ros::Publisher serial_vel;

//接受到订阅的消息后，会进入消息回调函数
void teleopCallback(const geometry_msgs::Twist& msg)
{

	x = msg.linear.x;
	y = msg.linear.y;
        th_z = msg.angular.z;

	//测试速度信息的接收情况
	//ROS_INFO("\nliner:\nx:%.3f\ny:%.3f\nz:%.3f\nangler:\nx:%.3f\ny:%.3f\nz:%.3f\n",x,y,z,th_x,th_y,th_z);

	write(x,y,th_z);

	recall_vel = read(re_x,re_y,re_th_z);

	//显示串口接收到的速度信息
	ROS_INFO("receive:\nliner:\nx:%.3f\ny:%.3f\nz:%.3f\nangler:\nx:%.3f\ny:%.3f\nz:%.3f\n",re_x,re_y,re_z,re_th_x,re_th_y,re_th_z);

	linear.x=re_x;
	linear.y=re_y;
	angular.z=re_th_z;
	twist.linear=linear;
	twist.angular=angular;
	serial_vel.publish(twist);
}

int main(int argc, char **argv)
{
	//初始化ROS节点
	ros::init(argc, argv, "speed_control");

	//创建句柄节点
	ros::NodeHandle n;

	//设置延时时间
	ros::Rate loop_rate(1);

        ros::Rate r(10.0);

	//串口初始化，引用自头文件learn_linux_serial.h，即获取同名cpp文件中的serialInit()函数
	serialInit();

	//创建一个Subscriber，订阅名为/cmd_vel的topic,注册回调函数teleopCallback
	ros::Subscriber person_info_sub = n.subscribe("/cmd_vel",10,teleopCallback);
	
	r.sleep();

	ros::Publisher vel_data_pub = n.advertise<geometry_msgs::Twist>("/vel_data", 10);

	serial_vel = vel_data_pub;

	ros::spin();

	return 0;
}
