#include <gazebo/gazebo.hh>
#include <iostream>

namespace gazebo
{
	class WelcomePlugin: public WorldPlugin
	{
	 public: WelcomePlugin(): WorldPlugin()
		{
			std::cout<<"Hello World"<<std::endl;
		}
	 public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
		{
		}
		
	};

	GZ_REGISTER_WORLD_PLUGIN(WelcomePlugin)
}
