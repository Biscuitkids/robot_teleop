#include "learn_linux_serial.h"

using namespace std;
using namespace boost::asio;
//串口相关对象
boost::asio::io_service iosev;
boost::asio::serial_port sp(iosev, "/dev/car");
boost::system::error_code err;
/********************************************************
            串口发送接收相关常量、变量、共用体对象
********************************************************/
const unsigned char header[2] = {0x55, 0xaa};  //数据头
const unsigned char ender[2] = {0x0d,0x0a};  //数据尾

//发送数据共用体
union sendData
{
	short d;
	unsigned char data[2];
}a,b;

//接收数据共用体（-32767 - +32768）
union receiveData
{
	short d;
	unsigned char data[2];
}rx,ry,rz;

/********************************************************
函数功能：串口参数初始化
入口参数：无
出口参数：
********************************************************/
void serialInit()
{
    sp.set_option(serial_port::baud_rate(9600));
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    sp.set_option(serial_port::character_size(8));
    ROS_INFO("Init success!");
}

/********************************************************
函数功能：将数据打包发送给下位机
入口参数：数据
出口参数：
********************************************************/
void write(char x)
{

    unsigned char buf[1];//数据发送缓存数组

    buf[0] = x;

    // 通过串口下发数据s
    boost::asio::write(sp, boost::asio::buffer(buf));
}

bool read(char &y)
{
    unsigned char buf[1];//数据接受缓存数组
    char read;
    try
    {
        boost::asio::streambuf response;
        boost::asio::read_until(sp, response, "",err);   
        copy(istream_iterator<unsigned char>(istream(&response)>>noskipws),
        istream_iterator<unsigned char>(),buf); 
    }  
    catch(boost::system::system_error &err)
    {
        ROS_INFO("read_until error");
    }

    y = buf[0];

    //ROS_INFO("read_until error");

    return true;
}
