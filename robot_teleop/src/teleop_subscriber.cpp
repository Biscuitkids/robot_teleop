#include<ros/ros.h>
#include<std_msgs/String.h>
#include"learn_linux_serial.h"

const char* control;
char out;
char input;

//接受到订阅的消息后，会进入消息回调函数
void personInfoCallback(const std_msgs::String::ConstPtr& msg)
{
	
	control = msg->data.data();

	out = control[0];

	write(out);

	//read(input);

	ROS_INFO("out:%c",out);

	//ROS_INFO("re:%c",input);
}

int main(int argc, char **argv)
{
	//初始化ROS节点
	ros::init(argc, argv, "teleop_subscriber");

	//创建句柄节点
	ros::NodeHandle n;

	//设置延时时间
	ros::Rate loop_rate(1);

	//串口初始化，引用自头文件learn_linux_serial.h，即获取同名cpp文件中的serialInit()函数
	serialInit();

	//创建一个Subscriber，订阅名为/person_info的topic,注册回调函数personInfoCallback
	ros::Subscriber person_info_sub = n.subscribe("/robot_teleop_key",10,personInfoCallback);

	//read(input);

	//循环等待函数
	ros::spin();

	return 0;
}
