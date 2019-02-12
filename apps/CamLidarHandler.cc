#include "CamLidarHandler.hh"

#include <string>

#include <gazebo/sensors/Sensor.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/sensors/SensorTypes.hh>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2//highgui.hpp>

// Constructor
CamLidarHandler::CamLidarHandler(){
  _imageData = NULL;
  _imageHeight = 0;
  _imageWidth = 0;
  _image = cv::Mat();

  _running = true;

  // load the camera matrix
  cv::FileStorage fs_K("/home/sascha/gazebo_bachelor_wip/plugins/K.xml", cv::FileStorage::READ);
  fs_K["K"] >> _K;
  fs_K.release();
  // load the extrinsics
  cv::FileStorage fs_extrinsics("/home/sascha/gazebo_bachelor_wip/plugins/extrinsics.xml", cv::FileStorage::READ);
  fs_extrinsics["R"] >> _R;
  fs_extrinsics["t"] >> _t;
  fs_extrinsics.release();
}

// Destructor
CamLidarHandler::~CamLidarHandler(){
  if(_imageData != NULL) {
    delete _imageData;
  }
}

void CamLidarHandler::ConnectToGazebo() {
  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();
  _velCmdPublisher = node->Advertise<gazebo::msgs::Pose>("~/pioneer2dx/vel_cmd");
  _scanSub = node->Subscribe("~/pioneer2dx/lidar/link/laser/scan", &CamLidarHandler::OnScanInternal, this);
  _imageSub = node->Subscribe("~/pioneer2dx/custom_camera/link/camera/image", &CamLidarHandler::OnImageInternal, this);

  // Wait for a subscriber to connect to this publisher
  _velCmdPublisher->WaitForConnection();
}

bool CamLidarHandler::IsRunning() {
  return _running;
}

void CamLidarHandler::SendVelocityMessage(double longitudinalVelocity, double angularVelocity) {
  //velocity command is expressed in gazebo::math::Pose form.
  ignition::math::Pose3<double> pose(longitudinalVelocity, 0.0, 0.0, 0.0, 0.0, angularVelocity);
  gazebo::msgs::Pose poseMsg;
  gazebo::msgs::Set(&poseMsg, pose);//generate message

  _velCmdPublisher->Publish(poseMsg); //publish message
}

cv::Point3f CamLidarHandler::CoordinatesFromAngleAndDistance(double angle, double distance) {
  // calculate the x and z coordinate of the given point from angle and distance
  // by definition, the point lies in the plane with y=0
  double x = -sin(angle) * distance;
  double y = 0; // the plane of the lidar is defined as y=0
  double z = cos(angle) * distance;

  return cv::Point3f(x, y, z);
}

void CamLidarHandler::OnScanInternal(ConstLaserScanStampedPtr &msg) {
  std::cout << "Received lidar scan at time " << gazebo::msgs::Convert(msg->time()).Double() << std::endl;
  // the access to this function is mutually exclusive
  std::lock_guard<std::mutex> guard(_dataLock);
  _scanPoints.clear();

  double currentAngle = msg->scan().angle_min();
  // initialize the smallest distance to infinity
  double minDist = std::numeric_limits<double>::infinity();
  double angleMinDist = 0;
  // iterate over all rays
  for(int i = 0; i < msg->scan().ranges_size(); i++) {
    // if the current range is not infinity, it hits an object,
    if(msg->scan().ranges(i) != std::numeric_limits<double>::infinity()) {
      //std::cout << "Range at " << currentAngle << ": " << msg->scan().ranges(i) << std::endl;
      _scanPoints.push_back(CamLidarHandler::CoordinatesFromAngleAndDistance(currentAngle, msg->scan().ranges(i)));
      //std::cout << "Corresponds to 3D point" << _scanPoints.back() << std::endl;
      // if the distance is smaller than the prior minimal distance, save the direction and the distance
      if(msg->scan().ranges(i) < minDist){
        angleMinDist = currentAngle;
        minDist = msg->scan().ranges(i);
      }
    }
    // msg->scan().angle_step() is the angle difference between two rays
    currentAngle += msg->scan().angle_step();
  }

  OnScan(msg);
}

void CamLidarHandler::OnImageInternal(ConstImageStampedPtr &msg) {
  std::cout << "Received image at time " << gazebo::msgs::Convert(msg->time()).Double() << std::endl;
  // the access to this function is mutually exclusive
  std::lock_guard<std::mutex> guard(_dataLock);

  if((msg->image().width() != _imageWidth) ||
     (msg->image().height() != _imageHeight)){
    if(_imageData != NULL) {
      delete _imageData;
    }

    _imageData = new unsigned char[msg->image().data().size()];
    _imageHeight = msg->image().height();
    _imageWidth = msg->image().width();
  }
  memcpy(_imageData, msg->image().data().c_str(), msg->image().data().length());
  // create a new image from the raw RGB data
  _image = cv::Mat(_imageHeight, _imageWidth, CV_8UC3, _imageData);
  // OpenCV uses BGR images, so change the order of the image channels
  cv::cvtColor(_image, _image, CV_RGB2BGR);

  OnImage(msg);
}

