#include <Engine/Rendering/Device/VulkanDevice.h>
#include <vector>
#include <set>
#include <Engine/Debug/DebugLog.h>
#include <Engine/Rendering/Memory/VulkanMemoryAllocator.h>
#include <Engine/Rendering/ValidationLayers/VulkanValidationLayers.h>

//Init static instance reference

ScrapEngine::Render::VulkanDevice* ScrapEngine::Render::VulkanDevice::instance_ = nullptr;

void ScrapEngine::Render::VulkanDevice::init(vk::SurfaceKHR* vulkan_surface_input_ref)
{
	vulkan_surface_ref_ = vulkan_surface_input_ref;

	choose_physical_device();
	create_logical_device();
	init_vulkan_allocator();
}

ScrapEngine::Render::VulkanDevice::~VulkanDevice()
{
	delete VulkanMemoryAllocator::get_instance();
	device_.destroy();
}

ScrapEngine::Render::VulkanDevice* ScrapEngine::Render::VulkanDevice::get_instance()
{
	if (instance_ == nullptr)
	{
		instance_ = new VulkanDevice();
	}
	return instance_;
}

void ScrapEngine::Render::VulkanDevice::choose_physical_device()
{
	std::vector<vk::PhysicalDevice> devices =
		VukanInstance::get_instance()->get_vulkan_instance()->enumeratePhysicalDevices();

	if (devices.empty())
	{
		Debug::DebugLog::fatal_error(vk::Result(-13), "VulkanDevice: Failed to find GPUs with Vulkan support!");
	}

	for (auto& entry_device : devices)
	{
		if (is_device_suitable(&entry_device, vulkan_surface_ref_))
		{
			physical_device_ = entry_device;
			msaa_samples_ = get_max_usable_sample_count();
			break;
		}
	}

	if (!physical_device_)
	{
		Debug::DebugLog::fatal_error(vk::Result(-13), "VulkanDevice: Failed to find a suitable GPU!");
	}
}

void ScrapEngine::Render::VulkanDevice::create_logical_device()
{
	cached_indices_ = find_queue_families(&physical_device_, vulkan_surface_ref_);

	std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
	const std::set<int> unique_queue_families = {cached_indices_.graphics_family, cached_indices_.present_family};

	float queue_priority = 1.0f;
	for (int queue_family : unique_queue_families)
	{
		vk::DeviceQueueCreateInfo queue_create_info(
			vk::DeviceQueueCreateFlags(),
			queue_family,
			1,
			&queue_priority
		);
		queue_create_infos.push_back(queue_create_info);
	}

	vk::PhysicalDeviceFeatures device_features;
	device_features.setSamplerAnisotropy(true);
	device_features.setSampleRateShading(true);

	vk::DeviceCreateInfo create_info(
		vk::DeviceCreateFlags(),
		static_cast<uint32_t>(queue_create_infos.size()),
		queue_create_infos.data(),
		0,
		nullptr,
		static_cast<uint32_t>(device_extensions_.size()),
		device_extensions_.data(),
		&device_features
	);

	VulkanValidationLayers* validation_layers = VukanInstance::get_instance()->get_validation_layers_manager();
	//Fill the vector only if the layers are enabled
	if (validation_layers && validation_layers->are_validation_layers_enabled())
	{
		const std::vector<const char*> validation_layers_list = validation_layers->get_validation_layers();

		create_info.setEnabledLayerCount(static_cast<uint32_t>(validation_layers_list.size()));
		create_info.setPpEnabledLayerNames(validation_layers_list.data());

		//Use the validation layer dynamic dispatcher
		const vk::Result result = physical_device_.createDevice(&create_info, nullptr, &device_, *validation_layers->get_dynamic_dispatcher());
		if (result != vk::Result::eSuccess)
		{
			Debug::DebugLog::fatal_error(result, "VulkanDevice: Failed to create logical device!");
		}

		//Init the dispatcher with the device
		validation_layers->get_dynamic_dispatcher()->init(device_);
	}
	else
	{
		//No dispatcher
		const vk::Result result = physical_device_.createDevice(&create_info, nullptr, &device_);
		if (result != vk::Result::eSuccess)
		{
			Debug::DebugLog::fatal_error(result, "VulkanDevice: Failed to create logical device!");
		}
	}
}

vk::PhysicalDevice* ScrapEngine::Render::VulkanDevice::get_physical_device()
{
	return &physical_device_;
}

vk::Device* ScrapEngine::Render::VulkanDevice::get_logical_device()
{
	return &device_;
}

ScrapEngine::Render::VulkanDevice::operator VkDevice_T*()
{
	return *(reinterpret_cast<VkDevice*>(&device_));
}

ScrapEngine::Render::VulkanDevice::operator vk::Device() const
{
	return device_;
}

ScrapEngine::Render::BaseQueue::QueueFamilyIndices ScrapEngine::Render::VulkanDevice::
get_cached_queue_family_indices() const
{
	return cached_indices_;
}

bool ScrapEngine::Render::VulkanDevice::is_device_suitable(vk::PhysicalDevice* physical_device_input,
                                                           vk::SurfaceKHR* surface)
{
	const vk::PhysicalDeviceProperties device_properties = physical_device_input->getProperties();
	const vk::PhysicalDeviceFeatures device_features = physical_device_input->getFeatures();

	// std::string gpu_name(device_properties.deviceName);
        std::string gpu_name = device_properties.deviceName;
	Debug::DebugLog::print_to_console_log("GPU Selected:" + gpu_name);
	Debug::DebugLog::print_to_console_log("GPU API v" + std::to_string(device_properties.apiVersion));
	Debug::DebugLog::print_to_console_log("GPU Driver v" + std::to_string(device_properties.driverVersion));

	BaseQueue::QueueFamilyIndices cached_indices = find_queue_families(physical_device_input, surface);

	const vk::PhysicalDeviceFeatures supported_features = physical_device_input->getFeatures();

	bool extensions_supported = check_device_extension_support(physical_device_input);

	bool swap_chain_adequate = false;
	if (extensions_supported)
	{
		VulkanSwapChain::SwapChainSupportDetails swap_chain_support = query_swap_chain_support(
			physical_device_input);
		swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
	}

	return device_properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu && device_features.geometryShader
		&& cached_indices.is_complete() && extensions_supported && swap_chain_adequate && supported_features.
		samplerAnisotropy;
}

bool ScrapEngine::Render::VulkanDevice::check_device_extension_support(vk::PhysicalDevice* device) const
{
	std::vector<vk::ExtensionProperties> available_extensions = device->enumerateDeviceExtensionProperties();

	std::set<std::string> required_extensions(device_extensions_.begin(), device_extensions_.end());

	for (const auto& extension : available_extensions)
	{
		required_extensions.erase(extension.extensionName);
	}

	return required_extensions.empty();
}

void ScrapEngine::Render::VulkanDevice::init_vulkan_allocator() const
{
	VulkanMemoryAllocator* allocator = VulkanMemoryAllocator::get_instance();
	allocator->init(physical_device_, device_);
	Debug::DebugLog::print_to_console_log("VulkanMemoryAllocator loaded!");
}

ScrapEngine::Render::VulkanSwapChain::SwapChainSupportDetails ScrapEngine::Render::VulkanDevice::
query_swap_chain_support(vk::PhysicalDevice* physical_device_input) const
{
	VulkanSwapChain::SwapChainSupportDetails details;

	physical_device_input->getSurfaceCapabilitiesKHR(*vulkan_surface_ref_, &details.capabilities);

	details.formats = physical_device_input->getSurfaceFormatsKHR(*vulkan_surface_ref_);

	details.present_modes = physical_device_input->getSurfacePresentModesKHR(*vulkan_surface_ref_);

	return details;
}

vk::SampleCountFlagBits ScrapEngine::Render::VulkanDevice::get_max_usable_sample_count() const
{
	const vk::PhysicalDeviceProperties physical_device_properties = physical_device_.getProperties();

	vk::SampleCountFlags counts;
	if (
		static_cast<uint32_t>(physical_device_properties.limits.framebufferColorSampleCounts) <
		static_cast<uint32_t>(physical_device_properties.limits.framebufferDepthSampleCounts)
	)
	{
		counts = physical_device_properties.limits.framebufferColorSampleCounts;
	}
	else
	{
		counts = physical_device_properties.limits.framebufferDepthSampleCounts;
	}

	if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
	if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
	if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
	if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
	if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
	if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

	return vk::SampleCountFlagBits::e1;
}

vk::SampleCountFlagBits ScrapEngine::Render::VulkanDevice::get_msaa_samples() const
{
	return msaa_samples_;
}

ScrapEngine::Render::BaseQueue::QueueFamilyIndices ScrapEngine::Render::VulkanDevice::find_queue_families(
	vk::PhysicalDevice* physical_device_input, vk::SurfaceKHR* surface)
{
	BaseQueue::QueueFamilyIndices indices;

	std::vector<vk::QueueFamilyProperties> queue_families = physical_device_input->getQueueFamilyProperties();

	int i = 0;
	for (const auto& queue_family : queue_families)
	{
		if (queue_family.queueCount > 0 && queue_family.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			indices.graphics_family = i;
		}

		VkBool32 present_support = false;
		physical_device_input->getSurfaceSupportKHR(i, *surface, &present_support);

		if (queue_family.queueCount > 0 && present_support)
		{
			indices.present_family = i;
		}

		if (indices.is_complete())
		{
			break;
		}

		i++;
	}

	return indices;
}
