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
extern void write(char x);
extern bool read(short &a,short &b,short &c,unsigned char &d);
unsigned char getCrc8(unsigned char *ptr, unsigned short len);

#endif
