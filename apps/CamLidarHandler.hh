#ifndef CAM_LIDAR_HANDLER_HH
#define CAM_LIDAR_HANDLER_HH

#include <gazebo/transport/transport.hh>
#include <opencv2/core/core.hpp>
#include <mutex>

class CamLidarHandler {
public:
  CamLidarHandler();
  ~CamLidarHandler();

  // the connections are made in a seperate function to ensure the object is fully constructed,
  // when the first callback is called
  void ConnectToGazebo();
  // MainLoop is not synchronized with mutexes. Better avoid changing data in MainLoop()
  virtual void MainLoop() = 0;

  bool IsRunning();
protected:
  cv::Mat _image;
  std::vector<cv::Point3f> _scanPoints;
  bool _running;
  cv::Mat _K, _R, _t;

  void SendVelocityMessage(double longitudinalVelocity, double angularVelocity);
  virtual void OnScan(ConstLaserScanStampedPtr &msg) = 0;
  virtual void OnImage(ConstImageStampedPtr &msg) = 0;

private:
  gazebo::transport::PublisherPtr _velCmdPublisher;
  gazebo::transport::SubscriberPtr _scanSub, _imageSub;

  std::mutex _dataLock;

  unsigned char *_imageData;
  unsigned int _imageWidth, _imageHeight;

  void OnScanInternal(ConstLaserScanStampedPtr &msg);
  void OnImageInternal(ConstImageStampedPtr &msg);

  static cv::Point3f CoordinatesFromAngleAndDistance(double angle, double distance);
};

#endif
