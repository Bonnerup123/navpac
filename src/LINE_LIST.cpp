
 #include <ros/ros.h>
#include <visualization_msgs/Marker.h>
   

   int main( int argc, char** argv )
   {
     ros::init(argc, argv, "points_and_lines");
     ros::NodeHandle n;
     ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
   
     ros::Rate r(30);
   
     float f = 0.0;
     while (ros::ok())
     {
   
       visualization_msgs::Marker points, line_strip;
       points.header.frame_id = line_strip.header.frame_id = "/map";
       points.header.stamp = line_strip.header.stamp = ros::Time::now();
       points.ns = line_strip.ns = "points_and_lines";
       points.action = line_strip.action = visualization_msgs::Marker::ADD;
       points.pose.orientation.w = line_strip.pose.orientation.w = 1.0;
   
   
   
       points.id = 0;
       line_strip.id = 1;
       
   
       points.type = visualization_msgs::Marker::POINTS;
       line_strip.type = visualization_msgs::Marker::LINE_STRIP;
   
       points.scale.x = 0.2;
       points.scale.y = 0.2;
  
       line_strip.scale.x = 0.1;
   
   
       points.color.g = 1.0f;
       points.color.a = 1.0;
   
       line_strip.color.b = 1.0;
       line_strip.color.a = 1.0;
  
   for (uint32_t i = 0; i < 1; ++i)
       {
        geometry_msgs::Point p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
  
        points.points.push_back(p);
        line_strip.points.push_back(p);
   
       }
   for (uint32_t i = 0; i < 1; ++i)
       {
        geometry_msgs::Point p;
        p.x = -6.35;
        p.y = 13.43;
        p.z = 0;
  
        points.points.push_back(p);
        line_strip.points.push_back(p);
   
       }
   for (uint32_t i = 0; i < 1; ++i)
       {
        geometry_msgs::Point p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
  
        points.points.push_back(p);
        line_strip.points.push_back(p);
   
       }
   for (uint32_t i = 0; i < 1; ++i)
       {
        geometry_msgs::Point p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
  
        points.points.push_back(p);
        line_strip.points.push_back(p);
   
       }
   for (uint32_t i = 0; i < 1; ++i)
       {
        geometry_msgs::Point p;
        p.x = 0;
        p.y = 0;
        p.z = 0;
  
        points.points.push_back(p);
        line_strip.points.push_back(p);
   
       }


      marker_pub.publish(points);
      marker_pub.publish(line_strip);
  
      r.sleep();
  
      f += 0.04;
    }
  }