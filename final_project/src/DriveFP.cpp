#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/TwistStamped.h>
#include <math.h>
#include <dynamic_reconfigure/server.h>
#include <final_project/FinalProjectConfig.h>

final_project::FinalProjectConfig  config; 
// defining node variables
ros::Publisher CmdVelpub;
ros::Publisher Steeringpub;
double dis_meters;
double vela1;
double anga1;
// for DRC SRV
double followDist = 10; // changeable( 10 min) ( want to make a dynamic reconfigure server for this)
double orginalcommand = 23;// changeable ( need it to pull from the Dynamic Reconfigure Server)
// for PID controler (do not touch)
//double kp  = .5;
//double ki = 0.1;
//double kd = 0;
int writehz = 10 ; 
//double error_dis_prior = 0;
geometry_msgs::Twist CmdVel;

void printCallback(const ros::TimerEvent & event ){
 ROS_INFO("velA1 %f", vela1); 
 ROS_INFO("anga1 %f", anga1); 
 ROS_INFO("dist %f",dis_meters);
 ROS_INFO("errordist %f",dis_meters-followDist);
 //ROS_INFO("orgcom %f",orginalcommand);
}
// get distance data
void distCallback(const std_msgs::Float64ConstPtr& msg)
{dis_meters = msg -> data;

}


void A1Callback(const geometry_msgs::TwistStampedConstPtr& msg)
{

  // msg variables
   vela1 = msg -> twist.linear.x;
   anga1 = msg -> twist.angular.z;


  
   // if angle is large, slow down car
// if dis_meters is writing
  if (dis_meters > 0){
    double error_dis = (dis_meters-followDist) ; // 5 meters = the car in front
  
// control via distancewith pid
    //vela1 = (vela1 + 1*(dis_meters-followDist) - 10*abs(anga1));
    // fastest change allowed is 10m/s^2 ( 10 hz adding 1 m/s)

    // this if statement was to make sure the car did not drive off the road
    if (error_dis > 10) { //  start speeding up the vehicle
   vela1 =  vela1+1;
    }else if (error_dis > 0) {
   vela1 =  vela1 - 0.025*(10-error_dis); // start slowing down vehicle ( 0.5 m/s^2)
    }else if (error_dis < 0 ) {
    vela1 = vela1 + 0.25*error_dis; // slowdown faster (2.5 m/s^2)
    }else {
    vela1 = vela1;// if error = 0
    }
// if vela is too large or small
    if (vela1 > orginalcommand) {
      vela1 = orginalcommand;
      }    
      else if (vela1 < 0){
      vela1 = 0;
      }
  
    if (dis_meters>200){ // if too far stop car
    vela1 = 0;
    ROS_INFO("Follow Vehicle too far"); 
    }
  }
    // publish
  CmdVel.angular.z = anga1;
  CmdVel.linear.x = vela1;
  CmdVelpub.publish(CmdVel);


}

void reconfig(final_project::FinalProjectConfig& cfg, uint32_t level){
  ROS_INFO("Current Distance:"); 
  config = cfg; 
  //followDist = config.distance;
}




int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "DriveFP");
  ros::NodeHandle node;
   

  
  CmdVelpub = node.advertise<geometry_msgs::Twist>("/a1/cmd_vel",1);

  ros::Subscriber sub1 = node.subscribe("/a1/twist", writehz, A1Callback);
  ros::Subscriber sub3 = node.subscribe("/dist", writehz, distCallback);

  
  ros::Timer writetimer = node.createTimer(ros::Duration(.1), printCallback);// debug messages

  // dynamic reconfigure 
  dynamic_reconfigure::Server<final_project::FinalProjectConfig> srv(node);
  srv.setCallback(boost::bind(reconfig,_1,_2));

 ros::spin();
  return 0;
} 
