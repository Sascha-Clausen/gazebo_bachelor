#ifndef PROJECTION_MATRIX_PLUGIN_HH
#define PROJECTION_MATRIX_PLUGIN_HH

#include <gazebo/plugins/CameraPlugin.hh>

namespace gazebo {
  class ProjectionMatrixPlugin : public CameraPlugin {
    public: ProjectionMatrixPlugin();
    public: ~ProjectionMatrixPlugin();

    public: void Load(sensors::SensorPtr parent, sdf::ElementPtr sdf);
  };
}
#endif

