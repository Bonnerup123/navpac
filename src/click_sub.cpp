#include <ros/ros.h>
#include "geometry_msgs/PointStamped.h"
#include <sstream>
#include "std_msgs/String.h"


void msgCallback(const geometry_msgs::PointStamped::ConstPtr& msg){
    
    ROS_INFO("The coordinantes you have clicked on is: %f, %f, %f", msg->point.x, msg->point.y, msg->point.z);
}



int main(int argc, char **argv){

    std::cout<<"how to use. \n1:Publish a point. \n2: See the published point on the terminal. \n\n\n3: Profit????? \n\n\n\n\n-----------------"<<std::endl;

  
    ros::init(argc, argv, "click_sub");

    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("clicked_point", 1000, msgCallback);    

    ros::spin();

    

    return 0;

}
