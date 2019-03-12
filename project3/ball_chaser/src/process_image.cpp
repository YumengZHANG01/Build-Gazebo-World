#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

#define WHITE_PIXEL	(255)

#define CAR_SPEED_X	(1)
#define CAR_ANGULAR_Z	(4)
typedef enum{
	CAR_NO_COMMAND,
	CAR_TURN_LEFT,
	CAR_TURN_RIGHT,
	CAR_STOP,
	CAR_MOVE_FORWARD
}DRIVE_COMMAND;


// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget srv;

    //assign the x value
    srv.request.linear_x = lin_x;

    //assign the z angular value
    srv.request.angular_z = ang_z;
    
    //send the request
    if(!client.call(srv)){
	ROS_ERROR("Failed to call service command_robot.");
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image &img)
{

    ROS_INFO("process_image module: Image data received.");
    
    DRIVE_COMMAND carCommand = CAR_STOP;

    //loop the data
    for(int i= 0; i<img.height*img.step; ++i){

	if(WHITE_PIXEL == img.data[i])
	{
		if(i%img.step < img.step/3){

		   carCommand = CAR_TURN_LEFT;
		}else if(i%img.step > img.step/3*2){

		   carCommand = CAR_TURN_RIGHT;
		}else{

		   carCommand = CAR_MOVE_FORWARD;
		}
		break;
	}	   
    }

    //control the car
    switch(carCommand){
    case CAR_TURN_LEFT:

	drive_robot(CAR_SPEED_X,CAR_ANGULAR_Z);
	ROS_INFO("Drive left");
	break;
    case CAR_TURN_RIGHT:

	drive_robot(CAR_SPEED_X,-CAR_ANGULAR_Z);
	ROS_INFO("Drive right");
	break;
    case CAR_MOVE_FORWARD:

	drive_robot(CAR_SPEED_X,0);
	ROS_INFO("Drive forward");
	break;
    default:

	drive_robot(0,0);
	ROS_INFO("Stop");
	break;
    }

    return;    
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
