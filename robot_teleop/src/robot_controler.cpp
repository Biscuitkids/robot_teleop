#include<ros/ros.h>
#include<std_msgs/String.h>
#include "robot_controler_header.h"

const char* control;
char out;



// receive value  接收数据
short testRece1;
short testRece2;
short testRece3;
unsigned char testRece4=0x00;



void personInfoCallback(const std_msgs::String::ConstPtr& msg)
{
	//设置延时时间
	ros::Rate loop_rate(1);

	control = msg->data.data();

	out = control[0];

	write(out);

	//从STM32接收数据
	read(testRece1,testRece2,testRece3,testRece4);

	ROS_INFO("out:%c",out);

	ROS_INFO("Speed \r linear:%d  angular:%d    mileage:%d,%c\n",testRece1,testRece2,testRece3,testRece4);

	//按照循环频率延时
	loop_rate.sleep();
}

int main(int argc, char **argv)
{
	//初始化ROS节点
	ros::init(argc, argv, "teleop_subscriber");

	//创建句柄节点
	ros::NodeHandle n;

	//串口初始化，引用自头文件learn_linux_serial.h，即获取同名cpp文件中的serialInit()函数
	serialInit();

	//创建一个Subscriber，订阅名为/person_info的topic,注册回调函数personInfoCallback
	ros::Subscriber person_info_sub = n.subscribe("/robot_teleop_key",10,personInfoCallback);

	//循环等待函数
	ros::spin();

	return 0;
}
