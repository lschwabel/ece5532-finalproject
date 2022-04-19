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

void printCallback(const ros::TimerEvent & event ){
 ROS_INFO("disavg %f", disavg); 
 //ROS_INFO("orgcom %f",orginalcommand);
}

void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& scan){
//max_ang = scan -> angle_max;
//min_ang = scan -> angle_min;
//inc_ang = scan -> angle_increment;
//scantime = scan -> scan_time;
//scaninc = scan -> time_increment;
//dis_max = scan -> range_max;
//dis_min = scan ->range_min;

int i;

for( i = 0; i < 10; i++){
dis_msg.data[i] = scan -> ranges[i];
  if(dis_msg.data[i] > 1) {
     if(dis_msg.data[i] < 30){
      disavg =  (dis_msg.data[i] + disavg )/2;
      if (angA != 0){
        angA = i;
      }  else {
        angB = i;
        }
    }
  }

}


}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "LidarCodeFP");
  ros::NodeHandle node;
  
 // subscribers
  ros::Subscriber A1_Lidar= node.subscribe<sensor_msgs::LaserScan>("a1/laser_front/scan",10,& processLaserScan); 
  ros::Timer writetimer = node.createTimer(ros::Duration(.1), printCallback);// debug messages
  
//  pub_dist = node.advertise<std_msgs::Float64>("/dist", 1);
 // pub_laser = node.advertise<std_msgs::Float64>("a1/laser_front/scan", 1);

  
  ros::spin();
}