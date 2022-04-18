#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <sensor_msgs/LaserScan.h>
sensor_msgs::LaserScan 


void recvlaser(const sensor_msgs::LaserScan& msg){

dis_msg = msg -> ranges;


}

void distCalc(const ros::TimerEvent & event ){
// i want to take the avearge of the visable item.
// take the scan and take and average. 
for (int i = 0; i < 5; i++) {
  cout << i << "\n";
}


_Float64 distance = 
std_msgs::Float64 distmsg;
distmsg.data = distance ;
pub_dist.publish(distmsg);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "LidarCodeFP");
  ros::NodeHandle node;
  
 // gps subscribers
  ros::Subscriber A1_Lidar= node.subscribe("/a1/laser_front/scan",1,recvlaser);
  ros::Timer Disttimer = node.createTimer(ros::Duration(0.001), distCalc);
  pub_dist = node.advertise<std_msgs::Float64>("/dist", 1);
  

  
  ros::spin();
}