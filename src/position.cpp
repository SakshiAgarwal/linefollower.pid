#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "quad/perpendicular.h"
#include <sstream>
#include <math.h>

quad::perpendicular msg_in;

void read_position(const quad::perpendicular::ConstPtr& msg)
{
  msg_in.theta = msg->theta;
  msg_in.imageCenter_x = msg->imageCenter_x;
  //ROS_INFO("%f",msg_in.theta);
  msg_in.imageCenter_y = msg->imageCenter_y;
  msg_in.projection_x = msg->projection_x;
  msg_in.projection_y = msg->projection_y;
  msg_in.perpendicular_distance = msg->perpendicular_distance;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;

  std_msgs::Empty my_msg;
  geometry_msgs::Twist m;
  ros::Publisher takeoff = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 10,true);
  takeoff.publish(my_msg);

  ros::Publisher twist;
  twist = n.advertise<geometry_msgs::Twist>("/quad/cmd_vel", 1);

  ros::Subscriber read = n.subscribe("/perpendicular_distance_center", 1000, read_position);

  int c;

      for(c=0;c<5;c++)
      c++;

       ros::Rate loop_rate(50);

       m.linear.z = 0;
       m.linear.y = 0.1;
       m.angular.x = 0;
       m.angular.y = 0;
       m.angular.z = 0;
       m.linear.x = 0;
       twist.publish(m);
       float errorp,pre_error,derivative,sum_integral=0;
       float p_error = 0;
       //pre_error = 0;
       float x2 = msg_in.projection_x;
       float x1 = msg_in.imageCenter_x;
       float theta = msg_in.theta;
       float kp,ki,kd;
       kp = 0.25;
       ki = 0.001;
       kd = 0.0;
       double e = 0.1;
       //double integral[10];
       double integral=0;
       int i;
       c=0;
       double error = 5;
       while(ros::ok())
       {

       //while(error > e || error < -e)
       //{

      /*   error = theta;
         //integral[(c++)%10] 0  = error;

         //for(i=0;i<10;i++)
         integral = integral + error;

         derivative = error-pre_error;
         m.angular.z = -(kp*error + ki*integral + kd*derivative);
         ROS_INFO("Angular error : %f", error);
         twist.publish(m);
         pre_error = error;

         theta = msg_in.theta;

         ros::spinOnce();
       */
       //}

      // loop_rate.sleep();
     //}

     errorp = (x2 - x1)/70;
     //integral[(c++)%10] = error;
     //for(i=0;i<10;i++)
     sum_integral = sum_integral + errorp;

     derivative = errorp - p_error;
     m.linear.x = -(kp*error + ki*sum_integral + kd*derivative);
     ROS_INFO("Error :  %f", errorp);

     twist.publish(m);

     p_error = errorp;
     x2 = msg_in.projection_x;
     x1 = msg_in.imageCenter_x;
     ros::spinOnce();
     loop_rate.sleep();

       //for(i=0;i<10;i++)
       //integral[i]=0;
       //c=0;


       loop_rate.sleep();

     }


    return 0;
  }
