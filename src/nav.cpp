#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <kobuki_msgs/AutoDockingAction.h>
#include <geometry_msgs/Twist.h>
#include <actionlib/client/simple_client_goal_state.h>

bool moveToGoal(double xGoal, double yGoal); //funktionsdefinationer til senere i koden
void learnpos();

//punkter paa ruten udtryk ved en vektor, så der er mulighed for at skrive koordinater på (x, y) form. gemt i en double som indeholder 8 bytes.
double p1[2] = {0, 0};
double p2[2] = {0, 0};
double p3[2] = {0, 0};
double p4[2] = {0, 0};

int main(int argc, char** argv){                                                    //mainfunktion
	ros::init(argc, argv, "navigation");                                            //stárt noden
	ros::NodeHandle n;                                                              //nodehandle til at kommunikere med masteren

	learnpos();                                                                     //starter learnpos funktionen som er beskrevet længere nede
    
	ros::spinOnce();                                                                //ros funktion som er vital for loop funktioner i ros loops
	
	char choice = 'y';                                                              //vi laver en char(1 byte) med startværdien y
	while (choice == 'y')                                                           //loop som kører, hvis choice er y
	{
		
		if (choice == 'y'){                                                         //et if statement som også kun kører hvis choice er y.
        
moveToGoal(p1[0], p1[1]);                                                           //funktion som sender vektorkoordinaterne i 1. og 2. indgang til movetogoal

moveToGoal(p2[0], p2[1]);	
			
moveToGoal(p3[0], p3[1]);	
			
moveToGoal(p4[0], p4[1]);		
				
}                                                                                   // afslutning af if statement
		std::cout << "repeat route? y/n\n";                                         // et cout, cin spørgsmål om ruten skal køre, ændres choice til alt andet end y, vil while loopet afslutte
		std::cin >> choice;
	}
                                                                      // kalder dockingfunktionen som ikke virker
	return 0;                                                                       // afslutter programmet.
}

bool moveToGoal(double xGoal, double yGoal){                                        // funktionsnavn og defination af de to værdier den skal have med fra main funktionen

	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true); // starter en actionclient som skal flytte robotten
	while(!ac.waitForServer(ros::Duration(5.0))){                                         // standardbesked hvis ikke noden kan få kontakt til actionserveren, den får 5 sekunder 
		ROS_INFO("Waiting for the move_base action server to come up");
	}

	move_base_msgs::MoveBaseGoal goal;                                                   // actionbeskeden defineres som typen Goal, fra move_base_msgs som er et bibleotek af beskeder, der hører med til move base funktionen

	goal.target_pose.header.frame_id = "/map";                                           //frame id, er rammen for den ønskede bevægelse
	goal.target_pose.header.stamp = ros::Time::now();                                    //ros tid, bruges til tidssync for funktionerne

	goal.target_pose.pose.position.x =  xGoal;                                           // her indsættes de to koordinater fra main funktionen som x og y koordinat. z er altid 0, da vi arbejder i 2d
	goal.target_pose.pose.position.y =  yGoal;
	goal.target_pose.pose.position.z =  0.0;
	goal.target_pose.pose.orientation.x = 0.0;                                           //her sættes målets orientering, og der sendes ingen x,y,z dimensioner, da dette er fuldstændig ligegyldigt for vores mål.
	goal.target_pose.pose.orientation.y = 0.0;                                           // til gengæld sættes w til 1.0 for at definere at vores mål eksiterer, en orientering på 0.0.0.0 ville gøre at det var et 0-dimensionelt mål hvilket er svært at køre efter
	goal.target_pose.pose.orientation.z = 0.0;
	goal.target_pose.pose.orientation.w = 1.0;

	ROS_INFO("Sending goal location ...");                                               //her sendes en besked når der er sendt en target pose
	ac.sendGoal(goal);                                                                   //her sendes goal beskeden med alle informationerne i til actionserveren. noter at sendgoal() er en funktion

	ac.waitForResult();                                                                  //ac skal vente på at serveren løser opgaven også en funktion

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){                    //et if statement der printer en besked hvis funktionen getstate() sender værdien succeded retur
		ROS_INFO("You have reached a turning point");
		return true;                                                                     //returnerer true fra funktionen.
	}
	else{
		ROS_INFO("The robot failed to reach the destination");                           //returnerer false hvis ikke robotten kan gennemføre
		return false;
	}

}

void learnpos(){                                                                  //funktionsnavn. void betyder at den ikke returnerer noget.

    ros::NodeHandle r;                                                            //nodehandle til funktionen, er nok egentlig ikke nødvendigt, men hey.. det virker
    ros::Publisher cmd_vel_pub = r.advertise<geometry_msgs::Twist>("cmd_vel_mux/input/teleop",1); //opretter en publisher som skriver beskeder på teleop topic'et

    geometry_msgs::Twist cmd_vel_message;                                         //vi bruger class'en cmd_vel_messege i beskeden
    cmd_vel_message.angular.z = 0.5;                                              //vi giver den ene værdi i beskeden som 0.5 m/s rotation om z aksen
    ros::Duration(10).sleep();                                                    // en ros duration vi skal prøve at køre uden..
    for(int i = 0; i < 35; i++)                                                   // et for-loop der kører 35 omgange, det er beregnet ved "baglæns korrektion"
    {
        cmd_vel_pub.publish(cmd_vel_message);                                     //publish beskeden om den angulære accelleration.
        ros::Duration(0.5).sleep();                                               // en besked om at køre beskeden i kun et halvt sekund. Dette gør vi fordi cmd_vel_messege kører i et helt sekund, og for at undgå hak i runden, skal loopet starte igen, så der er en ny besked til robotten.  
    }
}