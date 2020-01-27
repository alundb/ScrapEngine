#pragma once

#include <Engine/Rendering/Manager/RenderManager.h>
#include <Engine/Rendering/Model/SkyboxInstance/VulkanSkyboxInstance.h>

namespace ScrapEngine
{
	namespace Core
	{
		class SceneManager
		{
		private:
			Render::RenderManager* render_manager_ref_ = nullptr;
			Render::VulkanSkyboxInstance* current_skybox_ = nullptr;

			Render::StandardShadowmapping* shadowmapping_manager_ = nullptr;
		public:
			explicit SceneManager(Render::RenderManager* input_render_manager_ref);
			~SceneManager() = default;

			void set_skybox(const std::array<std::string, 6>& files_path);
			void set_skybox_size(unsigned int new_size) const;

			//By default the light always point to (0, 0, 0)
			void set_light_pos(const SVector3& pos) const;
			SVector3 get_light_pos() const;
			void set_light_lookat(const SVector3& lookat) const;
			SVector3 get_light_lookat() const;
			float get_light_fov() const;
			void set_light_fov(float fov) const;
		};
	}
}
