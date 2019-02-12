#ifndef OBSTACLE_FOLLOWER_HH
#define OBSTACLE_FOLLOWER_HH

#include "CamLidarHandler.hh"

class ObstacleFollower : public CamLidarHandler {
public:
  ObstacleFollower();
  ~ObstacleFollower();

  void MainLoop();
protected:
  void OnScan(ConstLaserScanStampedPtr &msg);
  void OnImage(ConstImageStampedPtr &msg);
private:
  cv::Mat _imageWithProjections;
};

#endif
