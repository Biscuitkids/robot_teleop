#include "ros/ros.h"
#include "std_msgs/String.h"
#include "serial_union_header.h"

//test send value  发送测试数据
float testSend1=5555.0;
float testSend2=2222.0;
unsigned char testSend3=0x07;

//test receive value  接收测试数据
float testRece1=0.0;
float testRece2=0.0;
float testRece3=0.0;
unsigned char testRece4=0x00;

int main(int agrc,char **argv)
{
    ros::init(agrc,argv,"public_node");
    ros::NodeHandle nh;

    ros::Rate loop_rate(10);
    
    //串口初始化
    serialInit();

    while(ros::ok())
    {
        ros::spinOnce();
        //向STM32端发送数据，前两个为double类型，最后一个为unsigned char类型
	   // writeSpeed(testSend3);
	    write(testSend1,testSend2,testSend3);
        //从STM32接收数据
	    read(testRece1,testRece2,testRece3,testRece4);
        //打印数据
	    ROS_INFO("re:%f,%f,%f,%c\n",testRece1,testRece2,testRece3,testRece4);

        loop_rate.sleep();
    }
    return 0;
}
