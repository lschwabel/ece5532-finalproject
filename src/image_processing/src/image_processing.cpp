#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>

using namespace std;

static const uint32_t MY_ROS_QUEUE_SIZE = 1000;

void callback(const sensor_msgs::ImageConstPtr& msg)
{

   try {
        cv_bridge::CvImageConstPtr cv_ptr;
        cv_ptr = cv_bridge::toCvShare(msg);

        //get image dimension once
        static bool runOnce = true;
        if (runOnce){
            cout << "Image dimension (Row,Col): " << cv_ptr->image.rows << " x " << cv_ptr->image.cols << endl;
            runOnce = false;
        }

        //get global max depth value
        double max = 0.0;
        cv::minMaxLoc(cv_ptr->image, 0, &max, 0, 0);
        std::cout << "Max value: " << max << endl;

        //get global min depth value
        double min = 0.0;
        cv::minMaxLoc(cv_ptr->image, &min, &max, 0, 0);
        std::cout << "Min value: " << min << endl;

        //get depth value at a point
        float distanceVal = cv_ptr->image.at<float>(100, 100);
        std::cout << "Distance value: " << distanceVal << "m" << endl;

    } catch (const cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "foo");

    std::cout << "Getting Image depth value!" << std::endl;

    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/a1/front_camera/image_raw", MY_ROS_QUEUE_SIZE, callback);

    std:cout << "raw camera message" << sub << endl;

    ros::spin();

    std::cout << "Done" << std::endl;

    return 0;
}