#ifndef EXTRINSIC_CALIBRATION_PLUGIN_HH
#define EXTRINSIC_CALIBRATION_PLUGIN_HH

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo {
  class ExtrinsicCalibrationPlugin : public ModelPlugin {
    public: ExtrinsicCalibrationPlugin();
    public: ~ExtrinsicCalibrationPlugin();

    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
  };
}
#endif

