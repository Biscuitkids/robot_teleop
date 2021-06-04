#ifndef MBOT_LINUX_SERIAL_H
#define MBOT_LINUX_SERIAL_H

#include <ros/ros.h>
#include <ros/time.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <boost/asio.hpp>
#include <geometry_msgs/Twist.h>

extern void serialInit();
extern void write(float x, float y,unsigned char z);
extern void writeSpeed(unsigned char ctrlFlag);
extern bool read(float &a,float &b,float &c,unsigned char &d);
unsigned char getCrc8(unsigned char *ptr, unsigned short len);

#endif
