#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"

#include <gazebo/gazebo_client.hh>

gazebo::transport::PublisherPtr pub = NULL;

void SendVelocityMessage(double longitudinal_velocity, double angular_velocity) {
  //velocity command is expressed in gazebo::math::Pose form.
  ignition::math::Pose3<double> pose(longitudinal_velocity, 0.0, 0.0, 0.0, 0.0, angular_velocity);
  gazebo::msgs::Pose pose_msg;
  gazebo::msgs::Set(&pose_msg, pose);//generate message

  pub->Publish(pose_msg); //publish message
}

/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  // Load gazebo as a client
  gazebo::client::setup(argc, argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  pub = node->Advertise<gazebo::msgs::Pose>("~/pioneer2dx/vel_cmd");

  // Wait for a subscriber to connect to this publisher
  pub->WaitForConnection();

  if(argc > 1) {
    double longitudinal_velocity = std::stod(argv[1]);
    double angular_velocity = 0;
    if(argc > 2) {
      angular_velocity = std::stod(argv[2]);
    }
    SendVelocityMessage(longitudinal_velocity, angular_velocity);
  }

  gazebo::client::shutdown();
}

