#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <kobuki_msgs/AutoDockingAction.h>
#include <geometry_msgs/Twist.h>
#include <actionlib/client/simple_client_goal_state.h>

bool moveToGoal(double xGoal, double yGoal);
void docking();
void learnpos();

//punkter paa ruten
double x1 = -6.35;
double py1 = -13.43;
double x2 = 0.0;
double y2 = 0.0;
double x3 = 0.0;
double y3 = 0.0;
double x4 = 0.0;
double y4 = 0.0;

bool goalReached = false;

int main(int argc, char** argv){
	ros::init(argc, argv, "navigation");
	ros::NodeHandle n;

	learnpos();
    
	ros::spinOnce();
	

	char choice = 'y';
	while (choice == 'y')
	{
		
		if (choice == 'y'){
        
			 goalReached = moveToGoal(x1, py1);
        
	
				goalReached = moveToGoal(x2, y2);
	
			

				goalReached = moveToGoal(x3, y3);	
	
			
				goalReached = moveToGoal(x4, y4);		
		
			
				
			
				
		
		if (goalReached == true){
				ROS_INFO("Route completed\n");
				ros::spinOnce();
				ros::spinOnce();

			}else{
				ROS_INFO("unable to comply! please shut down and reset\n");
			}
		}
		std::cout << "repeat route? y/n\n";
		std::cin >> choice;
	}
	docking();
	return 0;
}

bool moveToGoal(double xGoal, double yGoal){

	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
	while(!ac.waitForServer(ros::Duration(5.0))){
		ROS_INFO("Waiting for the move_base action server to come up");
	}

	move_base_msgs::MoveBaseGoal goal;

	goal.target_pose.header.frame_id = "/map";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x =  xGoal;
	goal.target_pose.pose.position.y =  yGoal;
	goal.target_pose.pose.position.z =  0.0;
	goal.target_pose.pose.orientation.x = 0.0;
	goal.target_pose.pose.orientation.y = 0.0;
	goal.target_pose.pose.orientation.z = 0.0;
	goal.target_pose.pose.orientation.w = 1.0;

	ROS_INFO("Sending goal location ...");
	ac.sendGoal(goal);

	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
		ROS_INFO("You have reached a turning point");
		return true;
	}
	else{
		ROS_INFO("The robot failed to reach the destination");
		return false;
	}

}

void docking(){
	 // Create the client
  actionlib::SimpleActionClient<kobuki_msgs::AutoDockingAction> docking_ac("dock_drive_action", true);
  docking_ac.waitForServer();

  // Create goal object
  kobuki_msgs::AutoDockingGoal goal;

  // Assign initial state
  actionlib::SimpleClientGoalState dock_state = actionlib::SimpleClientGoalState::LOST;

  // Send the goal
  docking_ac.sendGoal(goal);

  ros::Time time = ros::Time::now();

  // Monitor progress
  while (!docking_ac.waitForResult(ros::Duration(3))) {

    dock_state = docking_ac.getState();
    ROS_INFO("Docking status: %s",dock_state.toString().c_str());

    if (ros::Time::now() > (time+ros::Duration(20000))) {
      ROS_INFO("Docking took more than 20 seconds, canceling.");
      docking_ac.cancelGoal();
      break;
    }// end if
  }// end while
}

void learnpos(){

    ros::NodeHandle r;
    ros::Publisher cmd_vel_pub = r.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop",1);

    geometry_msgs::Twist cmd_vel_message;
    cmd_vel_message.angular.z = 0.0;
    cmd_vel_message.linear.x = 0.5;
    ros::Rate loop_rate(10);
    for(int i = 0; i < 1; i++)
    {
        cmd_vel_pub.publish(cmd_vel_message);
        ros::Duration(2).sleep();
        
    }

    cmd_vel_message.angular.z = 0.5;
    cmd_vel_message.linear.x = 0.0;
    ros::Duration(10).sleep();
    for(int i = 0; i < 35; i++)
    {
        cmd_vel_pub.publish(cmd_vel_message);
        ros::Duration(0.5).sleep();
        
    }
}