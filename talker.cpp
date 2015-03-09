#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
     geometry_msgs::Twist msg;
  
       
       msg.angular.z = 0.5; //set the linear and angular velocites
       msg.linear.x = 0.5;   
       
       ROS_INFO("%s", msg.angular.z.c_str());
       ROS_INFO("%s", msg.linear.x.c_str());
       chatter_pub.publish(msg);
  
      ros::spinOnce(); 
      loop_rate.sleep();
     
  }
  
  
    return 0;
  }
