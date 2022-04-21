#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/LaserScan.h>

ros::Publisher pub_dist;
ros::Publisher pub_laser;
sensor_msgs::LaserScan cmdlaser;
std_msgs::Float64MultiArray dis_msg;
double disavg;
double max_ang ;
double min_ang ;
int inc_ang ;
double scantime;
double scaninc;
double dis_max;
double dis_min;
double angA; 
double angB;
double range0;
double range90;
double rangeloop;
int i = 0;
int a;
double avg_range;

void printCallback(const ros::TimerEvent & event ){
 //ROS_INFO("disavg %f", disavg);
//ROS_INFO("range0 %f", range0);
//ROS_INFO("range90 %f", range90);

}

void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& scan){
max_ang = scan -> angle_max;
min_ang = scan -> angle_min;
inc_ang = scan -> angle_increment;
scantime = scan -> scan_time;
scaninc = scan -> time_increment;
dis_max = scan -> range_max;
dis_min = scan ->range_min;
range0 = scan -> ranges[0];
range90 = scan -> ranges[90];

i = 450 ;
while (i < 560){
  rangeloop = scan -> ranges[i];
  if ((rangeloop > 5) && (rangeloop < 30)) {
    //ROS_INFO("rangeloop %f", rangeloop);
     avg_range =  (avg_range*.75 + rangeloop*.25);// weighted average
    //ROS_INFO("rangeavg %f", avg_range);
    //ROS_INFO("rangeloopi %u", i);
  }
  i=i+5;
}
std_msgs::Float64 distmsg;
distmsg.data = avg_range;
pub_dist.publish(distmsg);
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "LidarCodeFP");
  ros::NodeHandle node;
  
 // subscribers
  ros::Subscriber A1_Lidar= node.subscribe<sensor_msgs::LaserScan>("a1/laser_front/scan",1,& processLaserScan); 
  ros::Timer writetimer = node.createTimer(ros::Duration(.1), printCallback);// debug messages
  
  pub_dist = node.advertise<std_msgs::Float64>("/dist", 1);
 // pub_laser = node.advertise<std_msgs::Float64>("a1/laser_front/scan", 1);


  
  ros::spin();
}