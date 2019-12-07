#include <ros/ros.h>
#include "geometry_msgs/PointStamped.h"
#include <sstream>
#include "std_msgs/String.h"

int x=1;

void msgCallback(const geometry_msgs::PointStamped::ConstPtr& msg){
    std::cout<<"This was point "<<x<<".\n";
    ROS_INFO("Clicked point is: \x1B[92m%f\033[0m, \x1B[94m%f\033[0m\n\n" , msg->point.x, msg->point.y);
    x++; 
 

}



int main(int argc, char **argv){

    std::cout<<"\e[1mCOORDINATE FINDER\e[0m \nHow to use: \n1:Publish a point. \n2:See the published point on the terminal. \n\n-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴-̴"<<std::endl;

  
    ros::init(argc, argv, "click_sub");

    ros::NodeHandle n;
       
    ros::Subscriber sub = n.subscribe("clicked_point", 1000, msgCallback);

    ros::spin();

    

    return 0;

}
