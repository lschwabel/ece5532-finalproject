#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <sensor_msgs/NavSatFix.h>
#include <nav_msgs/Path.h>
#include <ugv_course_libs/gps_conv.h>


UTMCoords ref_coords;
tf::Vector3 relative_A1postion;
tf::Vector3 relative_A2postion;
ros::Publisher pub_dist;
double dist;
double convangleA1;
double relative_A1position;
double convangleA2;
double relative_A2position;
UTMCoords a1_utm;
UTMCoords a2_utm;

void recvA1Fix(const sensor_msgs::NavSatFixConstPtr& msg){
  UTMCoords current_coords(*msg);
  LatLon current_coords_latlon(*msg);
  double CM_ref = ref_coords.getCentralMeridian();
  double vlat = current_coords_latlon.getLat();
  double vlon = current_coords_latlon.getLon();
  double a = (vlon - CM_ref);
  double b = tan(a);
  double c = sin(vlat);
  convangleA1 = atan(b*c);
  relative_A1postion = current_coords - ref_coords;
}
void recvA2Fix(const sensor_msgs::NavSatFixConstPtr& msg){
  UTMCoords current_coords(*msg);
  LatLon current_coords_latlon(*msg);
  double CM_ref = ref_coords.getCentralMeridian();
  double vlat = current_coords_latlon.getLat();
  double vlon = current_coords_latlon.getLon();
  double a = (vlon - CM_ref);
  double b = tan(a);
  double c = sin(vlat);
  convangleA2 = atan(b*c);
  relative_A2postion = current_coords - ref_coords;
}

void distCalc(const ros::TimerEvent & event ){

_Float64 distance = sqrt((relative_A1postion.x()-relative_A2postion.x())*(relative_A1postion.x()-relative_A2postion.x())+(relative_A1postion.y()-relative_A2postion.y())*(relative_A1postion.y()-relative_A2postion.y()));
ROS_INFO("dist %f", distance);
std_msgs::Float64 distmsg;
distmsg.data = distance - 5  ;// to account for the car in front bumper
pub_dist.publish(distmsg);
}




int main(int argc, char **argv)
{
  ros::init(argc, argv, "GpsCodeFP");
  ros::NodeHandle node;
  
 // gps subscribers
  ros::Subscriber A1gps_fix = node.subscribe("/a1/gps/fix",10,recvA1Fix);
  ros::Subscriber A2gps_fix = node.subscribe("/a2/gps/fix",10,recvA2Fix);
  ros::Timer Disttimer = node.createTimer(ros::Duration(0.001), distCalc);
  pub_dist = node.advertise<std_msgs::Float64>("/dist", 10);
  
  double ref_lat;
  double ref_lon;

  node.getParam("/a1/gps/ref_lat", ref_lat);
  node.getParam("/a1/gps/ref_lon", ref_lon);
  

  LatLon ref_coords_lat_lon(ref_lat, ref_lon, 0);
  ref_coords = UTMCoords(ref_coords_lat_lon);
  
  ros::spin();
}