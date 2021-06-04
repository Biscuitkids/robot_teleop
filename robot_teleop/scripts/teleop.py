#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import sys, select, termios, tty
from std_msgs.msg import String


#操作教程
msg = """
Control mbot!
---------------------------
Moving around:
        1    
   3    0    4
        2     

5/6 : clockwise/counterclockwise rotation
7/8 : increase/decrease max speeds by 10%
0 : force stopmsgs
anything else : stop smoothly

CTRL-C to quit
"""

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
        key = '0'

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    #termios.tcsetattr(fd, when, attributes)
    #fd获取终端上交互输入的返回列表
    #when  tcsanow立即更改，tcsadrain在传输所有排队的输出后更改，或tcsaflush在传输所有排队的输出并丢弃所有排队的输入后更改。

    return key

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)

    rospy.init_node('mbot_teleop')
    pub = rospy.Publisher('/robot_teleop_key', String, queue_size = 10)

    try:
        print msg
        while(1):
            key = getKey()
            #print key
            pub.publish(key)
            if (key == '\x03'):
                    break

    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
