#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "quad/perpendicular.h"
#include <sstream>
#include <math.h>
#include "geometry_msgs/Point.h"


quad::perpendicular msg_in;
geometry_msgs::Point node_point;

void read_position(const quad::perpendicular::ConstPtr& msg)
{
  msg_in.theta = msg->theta;
  msg_in.imageCenter_x = msg->imageCenter_x;
  msg_in.imageCenter_y = msg->imageCenter_y;
  msg_in.projection_x = msg->projection_x;
  msg_in.projection_y = msg->projection_y;
  msg_in.perpendicular_distance = msg->perpendicular_distance;

}

void read_node(const geometry_msgs::Point::ConstPtr& node_p)
{
  node_point.x = node_p->x;
  node_point.y = node_p->y;
  node_point.z = node_p->z;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "p1");
  ros::NodeHandle n;

  std_msgs::Empty my_msg;
  geometry_msgs::Twist m;
  ros::Publisher takeoff = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 10,true);
  takeoff.publish(my_msg);

  ros::Publisher twist;
  twist = n.advertise<geometry_msgs::Twist>("/quad/cmd_vel", 1);

  ros::Subscriber read = n.subscribe("/perpendicular_distance_center", 1000, read_position);
  //ros::Subscriber sub = n.subscribe("node_point", 1000, read_node);
  int c;

      for(c=0;c<5;c++)
      c++;

       ros::Rate loop_rate(100); // this was 50

       m.linear.z = 0;
       m.linear.y = -0.05;
       m.angular.x = 0;
       m.angular.y = 0;
       m.angular.z = 0;
       m.linear.x = 0.05;

       twist.publish(m);

       float error1,error2,error_p,p_error,derivative,integral,sum_integral=0, derivative_p;
       //float p_error = 0;
       //pre_error = 0;
       float x2 = msg_in.projection_x;
       float y2 = msg_in.projection_y;
       float x1 = msg_in.imageCenter_x;
       float y1 = msg_in.imageCenter_y;
      // float theta = msg_in.theta;
       float kp,ki,kd;
       kp = 0.25;
       ki = 0.0012;
       kd = 0.0;

       float x_final = 340, y_final = 60;
       while(ros::ok())
       {

     while(error1!=0)
     {
     error1 = (x2 - x1)/100; // /70 here
     sum_integral = sum_integral + error1;
     derivative_p = error1 - p_error;

     m.linear.y = (kp*error1 + ki*sum_integral + kd*derivative_p);
     ROS_INFO("Error x:  %f", error1);

     twist.publish(m);
     p_error = error1;
     x2 = msg_in.projection_x;
     x1 = msg_in.imageCenter_x;
     
     }
      error2 = (y2 - y1)/100;
      derivative = error2 - error_p;
      integral = integral + error2;

     m.linear.x = (kp*error2 + ki*integral + kd*derivative);

     ROS_INFO("Error y:  %f", error2);

     twist.publish(m);

     p_error = error2;
     y2 = msg_in.projection_y;
     y1 = msg_in.imageCenter_y;

     ros::spinOnce();
     loop_rate.sleep();

     }
     return 0;
  }
