#include <gazebo/gazebo_client.hh>

#include "ObstacleFollower.hh"

int main(int argc, char **argv)
{
  // Load gazebo as a client
  gazebo::client::setup(argc, argv);

  ObstacleFollower obstacleFollower;
  obstacleFollower.ConnectToGazebo();
  obstacleFollower.MainLoop();

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}
