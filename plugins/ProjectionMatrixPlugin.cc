#include "ProjectionMatrixPlugin.hh"

#include <opencv2/core/core.hpp>

namespace gazebo{
  // Register this plugin with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(ProjectionMatrixPlugin)

  ////////////////////////////////////////////////////////////////////////////////
  // Constructor
  ProjectionMatrixPlugin::ProjectionMatrixPlugin(){
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Destructor
  ProjectionMatrixPlugin::~ProjectionMatrixPlugin(){
  }

  void ProjectionMatrixPlugin::Load(sensors::SensorPtr parent, sdf::ElementPtr sdf){
    CameraPlugin::Load(parent, sdf);

    if (sdf->HasElement("filename")) {
      std::string filename = sdf->GetElement("filename")->GetValue()->GetAsString();
      if(filename != ""){
        cv::Mat K = cv::Mat::zeros(cv::Size(3, 3), CV_64FC1);

        double f_x = (this->width / 2.0) / tan(this->camera->HFOV().Radian() / 2.0);
        double f_y = (this->height / 2.0) / tan(this->camera->VFOV().Radian() / 2.0);

        K.at<double>(0, 0) = f_x;
        K.at<double>(0, 2) = this->width / 2;
        K.at<double>(1, 1) = f_y;
        K.at<double>(1, 2) = this->height / 2;
        K.at<double>(2, 2) = 1;
        std::cout << "Camera matrix K:\n" << K << std::endl;

        cv::FileStorage K_file(filename, cv::FileStorage::WRITE);
        K_file << "K" << K;
      }
      else {
        std::cout << "The destination of the camera matrix file has to be set in models/custom_camera/model.sdf. Syntax:\n\
<plugin name=\"ProjectionMatrixPlugin\" filename=\"libProjectionMatrixPlugin.so\">\n\
  <filename>/home/sascha/gazebo/plugins/K.xml</filename>\n\
</plugin>" << std::endl;
      }
    }
    else {
      std::cout << "The destination of the camera matrix file has to be set in models/custom_camera/model.sdf. Syntax:\n\
<plugin name=\"ProjectionMatrixPlugin\" filename=\"libProjectionMatrixPlugin.so\">\n\
  <filename>/home/sascha/gazebo/plugins/K.xml</filename>\n\
</plugin>" << std::endl;
    }
  }
}
