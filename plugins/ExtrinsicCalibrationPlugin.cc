#include "ExtrinsicCalibrationPlugin.hh"

#include <string>

#include <gazebo/sensors/Sensor.hh>
#include <gazebo/sensors/SensorTypes.hh>
#include <gazebo/common/CommonTypes.hh>

#include <opencv2/core/core.hpp>

namespace gazebo{
  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ExtrinsicCalibrationPlugin)

  ////////////////////////////////////////////////////////////////////////////////
  // Constructor
  ExtrinsicCalibrationPlugin::ExtrinsicCalibrationPlugin(){
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Destructor
  ExtrinsicCalibrationPlugin::~ExtrinsicCalibrationPlugin(){
  }

  void ExtrinsicCalibrationPlugin::Load(physics::ModelPtr parent, sdf::ElementPtr sdf){
    if (sdf->HasElement("filename")) {
      std::string filename = sdf->GetElement("filename")->GetValue()->GetAsString();
      if(filename != ""){

        physics::EntityPtr lidarEntity = boost::dynamic_pointer_cast<physics::Entity>(parent->GetJoint("laser_joint")->GetChild());
        std::cout << "Lidar:" << std::endl;
        std::cout << "Pose: " << lidarEntity->InitialRelativePose() << std::endl;
        std::cout << "Position: " << lidarEntity->InitialRelativePose().Pos() << std::endl;
        std::cout << "Rotation: " << lidarEntity->InitialRelativePose().Rot() << std::endl;
        physics::EntityPtr cameraEntity = boost::dynamic_pointer_cast<physics::Entity>(parent->GetJoint("camera_joint")->GetChild());
        std::cout << "Camera: " << std::endl;
        std::cout << "Pose: " << cameraEntity->InitialRelativePose() << std::endl;
        std::cout << "Position: " << cameraEntity->InitialRelativePose().Pos() << std::endl;
        std::cout << "Rotation: " << cameraEntity->InitialRelativePose().Rot() << std::endl;

        ignition::math::Pose3d difference = cameraEntity->InitialRelativePose() - lidarEntity->InitialRelativePose();
        std::cout << "Pose difference: " << std::endl;
        std::cout << difference << std::endl;

        cv::Mat t = (cv::Mat_<double>(3,1) << 0, 0, 0);
        cv::Mat R = cv::Mat::eye(cv::Size(3, 3), CV_64FC1);

        cv::FileStorage extrinsicsFile(filename, cv::FileStorage::WRITE);
        extrinsicsFile << "R" << R;
        extrinsicsFile << "t" << t;
      }
      else {
        std::cout << "The destination of the extrinsics file has to be set in models/bachelor_robot/model.sdf. Syntax:\n\
<plugin name=\"ExtrinsicCalibrationPlugin\" filename=\"libExtrinsicCalibrationPlugin.so\">\n\
  <filename>/home/sascha/gazebo/plugins/extrinsics.xml</filename>\n\
</plugin>" << std::endl;
      }
    }
    else {
      std::cout << "The destination of the extrinsics file has to be set in models/bachelor_robot/model.sdf. Syntax:\n\
<plugin name=\"ExtrinsicCalibrationPlugin\" filename=\"libExtrinsicCalibrationPlugin.so\">\n\
  <filename>/home/sascha/gazebo/plugins/extrinsics.xml</filename>\n\
</plugin>" << std::endl;
    }
  }
}
