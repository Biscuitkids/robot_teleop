#include "slam_car_control_header.h"

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

//用于检验发送数据
double cx,cy,cth_z;

/*******************************************************
	注意：共用体的字节长度由字节长度最长的变量决定。
	故使用共同变量时，此处字节长度应为4字节。
*******************************************************/
//发送数据共用体（-32767 - +32768）
union sendData
{
	float d;
	unsigned char data[4];
}sx,sy,sth_z;

//接收数据共用体（-32767 - +32768）
union receiveData
{
	float d;
	unsigned char data[4];
}rx,ry,rth_z;

/********************************************************
函数功能：串口参数初始化
入口参数：无
出口参数：
********************************************************/
void serialInit()
{
    sp.set_option(serial_port::baud_rate(115200));
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    sp.set_option(serial_port::character_size(8));
    ROS_INFO("Serial init success!");
}

/********************************************************
函数功能：将数据打包发送给下位机
入口参数：数据
出口参数：
********************************************************/
void write(float x,float y,float th_z)
{
    unsigned char buf[18] = {0};//数据发送缓存数组
    int i, length = 0;

    rx.d  = x;  
    ry.d = y;
    rth_z.d = th_z;

    // 设置消息头
    for(i = 0; i < 2; i++)
        buf[i] = header[i];             //buf[0]  buf[1]
    
    // 数据赋值
    length = 12;
    buf[2] = length;                   //buf[2]通知数据长度
    for(i = 0; i < 4; i++)
    {
        buf[i + 3] = rx.data[i];  //buf[3] buf[4] buf[5] buf[6]
        buf[i + 7] = ry.data[i]; //buf[7] buf[8] buf[9] buf[10]
        buf[i + 11] = rth_z.data[i]; //buf[11] buf[12] buf[13] buf[14]
 }

    // 设置校验值、消息尾
    buf[3 + length] = getCrc8(buf, 3 + length);//buf[15]
    buf[3 + length + 1] = ender[0];     //buf[16]
    buf[3 + length + 2] = ender[1];     //buf[17]

    // 通过串口下发数据
    boost::asio::write(sp, boost::asio::buffer(buf));
}


/********************************************************
函数功能：从下位机读取数据
出口参数：bool
********************************************************/
bool read(float &a,float &b,float &c)
{
    char i, length = 0;
    unsigned char checkSum;
    unsigned char buf[150]={0};
    //=========================================================
    //此段代码可以读数据的结尾，进而来进行读取数据的头部
    try
    {
        boost::asio::streambuf response;
        boost::asio::read_until(sp, response, "\r\n",err);   
        copy(istream_iterator<unsigned char>(istream(&response)>>noskipws),
        istream_iterator<unsigned char>(),
        buf); 
    }  
    catch(boost::system::system_error &err)
    {
        ROS_INFO("read_until error");
    } 
    //=========================================================        

    // 检查信息头
    if (buf[0]!= header[0] || buf[1] != header[1])   //buf[0] buf[1]
    {
        ROS_ERROR("Received message header error!");
        return false;
    }
    // 数据长度
    length = buf[2];                                 //buf[2]

    // 检查信息校验值
    checkSum = getCrc8(buf, 3 + length);             //buf[10] 计算得出
    if (checkSum != buf[3 + length])                 //buf[10] 串口接收
    {
        ROS_ERROR("Received data check sum error!");
        return false;
    }    

    // 读取值
    for(i = 0; i < 4; i++)
    {
        rx.data[i] = buf[i + 3]; //buf[3] buf[4] buf[5] buf[6]
        ry.data[i] = buf[i + 7]; //buf[7] buf[8] buf[9] buf[10]
        rth_z.data[i] = buf[i + 11]; //buf[11] buf[12] buf[13] buf[14]
    }

    //d = buf[9];
    
    a =rx.d;
    b=ry.d;
    c =rth_z.d;

    return true;
}
/********************************************************
函数功能：获得8位循环冗余校验值
入口参数：数组地址、长度
出口参数：校验值
********************************************************/
unsigned char getCrc8(unsigned char *ptr, unsigned short len)
{
    unsigned char crc;
    unsigned char i;
    crc = 0;
    while(len--)
    {
        crc ^= *ptr++;
        for(i = 0; i < 8; i++)
        {
            if(crc&0x01)
                crc=(crc>>1)^0x8C;
            else 
                crc >>= 1;
        }
    }
    return crc;
}
