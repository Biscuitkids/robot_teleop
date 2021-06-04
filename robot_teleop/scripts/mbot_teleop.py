#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from geometry_msgs.msg import Twist
import sys, select, termios, tty

#操作教程
msg = """
Control mbot!
---------------------------
Moving around:
   u    i    o
   j    k    l
   m    ,    .

q/z : increase/decrease max speeds by 10%
w/x : increase/decrease only linear speed by 10%
e/c : increase/decrease only angular speed by 10%
space key, k : force stop
anything else : stop smoothly

CTRL-C to quit
"""

#用于改变机器人运动方向的字典
moveBindings = {
        'i':(1,0),
        'o':(1,-1),
        'j':(0,1),
        'l':(0,-1),
        'u':(1,1),
        ',':(-1,0),
        '.':(-1,1),
        'm':(-1,-1),
           }

#用于改变机器人运送速度的字典
speedBindings={
        'q':(1.1,1.1),
        'z':(.9,.9),
        'w':(1.1,1),
        'x':(.9,1),
        'e':(1,1.1),
        'c':(1,.9),
          }

def getKey():
    tty.setraw(sys.stdin.fileno())
    #tty 模块定义了将 tty 放入 cbreak 和 raw 模式的函数。因为他需要termios模块，所以只能在UNIX上运行
    #tty 模块定义了以下函数
    #tty.setraw(fd, when=termios.TCSAFLUSH)
    #将文件描述符 fd 的模式更改为 raw 。如果 when 被省略，则默认为 termios.TCSAFLUSH ，并传递给 termios.tcsetattr() 。
    #tty.setcbreak(fd, when=termios.TCSAFLUSH)
    #将文件描述符 fd 的模式更改为 cbreak 。如果 when 被省略，则默认为 termios.TCSAFLUSH ，并传递给 termios.tcsetattr() 。
    rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
    #rlist则是键盘输入的信息列表
    #select()方法接收并监控3个通信列表
    #第1个是所有的输入的data,就是指外部发过来的数据
    #第2个是监控和接收所有要发出去的data(outgoing data)
    #第3个监控错误信息


    #判断是否接受到数据
    if rlist:
        key = sys.stdin.read(1)#读取终端上的交互输入
    else:
        key = ''

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    #termios.tcsetattr(fd, when, attributes)
    #fd获取终端上交互输入的返回列表
    #when  tcsanow立即更改，tcsadrain在传输所有排队的输出后更改，或tcsaflush在传输所有排队的输出并丢弃所有排队的输入后更改。

    return key

speed = .2    #设置初始直行速度
turn = 1      #设置初始角速度

def vels(speed,turn):
    return "currently:\tspeed %s\tturn %s " % (speed,turn)

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)
    
    rospy.init_node('mbot_teleop')
    

    x = 0
    th = 0
    status = 0
    count = 0
    acc = 0.1
    target_speed = 0
    target_turn = 0
    control_speed = 0
    control_turn = 0
    try:
        print msg
        print vels(speed,turn)
        while(1):
            key = getKey()
            # 运动控制方向键（1：正方向，-1负方向）
            if key in moveBindings.keys():
                x = moveBindings[key][0]
                th = moveBindings[key][1]
                count = 0
            # 速度修改键
            elif key in speedBindings.keys():
                speed = speed * speedBindings[key][0]  # 线速度增加0.1倍
                turn = turn * speedBindings[key][1]    # 角速度增加0.1倍
                count = 0

                print vels(speed,turn)
                if (status == 14):
                    print msg
                status = (status + 1) % 15
            # 停止键
            elif key == ' ' or key == 'k' :
                x = 0
                th = 0
                control_speed = 0
                control_turn = 0
            else:
                count = count + 1
                if count > 4:
                    x = 0
                    th = 0
                if (key == '\x03'):
                    break

            # 目标速度=速度值*方向值
            target_speed = speed * x
            target_turn = turn * th

            # 速度限位，防止速度增减过快
            if target_speed > control_speed:
                control_speed = min( target_speed, control_speed + 0.02 )
            elif target_speed < control_speed:
                control_speed = max( target_speed, control_speed - 0.02 )
            else:
                control_speed = target_speed

            if target_turn > control_turn:
                control_turn = min( target_turn, control_turn + 0.1 )
            elif target_turn < control_turn:
                control_turn = max( target_turn, control_turn - 0.1 )
            else:
                control_turn = target_turn

            # 创建并发布twist消息
            twist = Twist()
            twist.linear.x = control_speed; 
            twist.linear.y = 0; 
            twist.linear.z = 0
            twist.angular.x = 0; 
            twist.angular.y = 0; 
            twist.angular.z = control_turn
            pub.publish(twist)

    except:
        print e

    finally:
        twist = Twist()
        twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
        pub.publish(twist)

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
