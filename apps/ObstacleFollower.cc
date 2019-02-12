#include "ObstacleFollower.hh"

#include <string>

#include <gazebo/sensors/Sensor.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/sensors/SensorTypes.hh>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2//highgui.hpp>

// Constructor
ObstacleFollower::ObstacleFollower(){

}

// Destructor
ObstacleFollower::~ObstacleFollower(){

}

void ObstacleFollower::MainLoop() {
  while(_running){
    if(!_imageWithProjections.empty()) {
      cv::imshow("camera", _imageWithProjections);
      int key = cv::waitKey(10);
      if(key == 'q') {
        _running = false;
      }
    }
  }
}

void ObstacleFollower::OnScan(ConstLaserScanStampedPtr &msg) {
  ConstLaserScanStampedPtr laserScanMsg = static_cast<ConstLaserScanStampedPtr>(msg);
  double currentAngle = laserScanMsg->scan().angle_min();
  // initialize the smallest distance to infinity
  double minDist = std::numeric_limits<double>::infinity();
  double angleMinDist = 0;
  // iterate over all rays
  for(int i = 0; i < laserScanMsg->scan().ranges_size(); i++) {
    // if the current range is not infinity, it hits an object
    if(laserScanMsg->scan().ranges(i) != std::numeric_limits<double>::infinity()) {
      // if the distance is smaller than the prior minimal distance, save the direction and the distance
      if(laserScanMsg->scan().ranges(i) < minDist){
        angleMinDist = currentAngle;
        minDist = laserScanMsg->scan().ranges(i);
      }
    }
    // msg->scan().angle_step() is the angle difference between two rays
    currentAngle += laserScanMsg->scan().angle_step();
  }


  if(minDist != std::numeric_limits<double>::infinity()) {
    // send the resulting velocity command to gazebo
    // scale the velocity with the distance, so the robot slows down if it gets nearer to the obstacle
    SendVelocityMessage(minDist / 10, -angleMinDist);
    //std::cout << "Minimal distance " << minDist << " at angle " << angleMinDist << std::endl;
  }
}

void ObstacleFollower::OnImage(ConstImageStampedPtr &msg) {
  _imageWithProjections = _image.clone();
  if(!_scanPoints.empty()) {
    //cv::Mat R = cv::Mat::eye(3, 3, CV_64FC1);
    cv::Mat rvec;
    cv::Rodrigues(_R, rvec);

    //cv::Mat tvec = (cv::Mat_<double>(3,1) <<  0, 0, 0);

    std::vector<cv::Point2f> imagePoints;
    cv::projectPoints(_scanPoints, rvec, _t, _K, cv::noArray(), imagePoints);

    for(cv::Point2f imagePoint : imagePoints) {
      cv::circle(_imageWithProjections, imagePoint, 3, cv::Scalar(255, 0, 0), CV_FILLED);
    }
  }

}

