/*
 * @Author: your name
 * @Date: 2021-09-06 09:18:43
 * @LastEditTime: 2021-09-24 17:40:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_management\vulkan_management.cpp
 */

#include <string>
#include <vector>
#include <iostream>

#include <vulkan/vulkan.h>
#include <vulkan_management.hpp>

namespace harpocrates {

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT type,
		uint64_t object,
		size_t location,
		int32_t message_code,
		const char* layer_prefix,
		const char* message,
		void* user_data) {
		if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
			//LOGE("Validation Layer: Error: {}: {}", layer_prefix, message);
			printf("Validation Layer: Error: {%s}: {%s}\n", layer_prefix, message);
		}
		else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
			//LOGE("Validation Layer: Warning: {}: {}", layer_prefix, message);
			printf("Validation Layer: Warning: {%s}: {%s}\n", layer_prefix, message);
		}
		else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
			//LOGI("Validation Layer: Performance warning: {}: {}", layer_prefix, message);
			printf("Validation Layer: Performance warning: {%s}: {%s}\n", layer_prefix, message);
		}
		else {
			//LOGI("Validation Layer: Information: {}: {}", layer_prefix, message);
			printf("Validation Layer: Information: {%s}: {%s}\n", layer_prefix, message);
		}
		return VK_FALSE;
	}

	vulkan::vulkan() : __vk_res{ VK_SUCCESS }, __vk_instance{ nullptr }, __device_index{ 0 },
		__logic_device{ nullptr }, __graphics_queue{ nullptr }, __descriptor_pool{ nullptr },
		__command_pool{ nullptr } {
		auto res = 0;
		do {
			res = __validate_layer();
			if (res != 0) break;
			res = __validate_extension();
			if (res != 0) break;
			res = __create_instance("arcsoft_algorithm", "vulkan_engine");
			if (res != 0) break;
			res = __select_device();
			if (res != 0) break;
			res = __create_device();
			if (res != 0) break;
			//res = __create_swapchain();
			//if (res != 0) break;
		} while (false);
	}
	vulkan::~vulkan() {
		__release_operation();
	}

	int vulkan::set_algorithm(std::string name) {
		do {
			__algorithm_name = name;
		} while (false);
		return 0;
	}

	int vulkan::create_descriptorpool() {
		auto res = 0;
		do {
			res = __create_descriptorpool();
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::create_commandpool() {
		auto res = 0;
		do {
			res = __create_commandpool();
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::create_descriptorsetlayout(int count) {
		auto res = 0;
		do {
			// TODO: add uniform memory interface VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
			res = __create_descriptorsetlayout(count);
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::create_descriptorset() {
		auto res = 0;
		do {
			res = __create_descriptorset();
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::create_pipeline(std::string shader) {
		auto res = 0;
		do {
			// TODO: add push constant range operation
			res = __create_pipeline(shader);
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::allocate_gpubuffer(std::string name, VkDeviceSize size) {
		auto res = 0;
		do {
			res = __allocate_gpubuffer(name, size);
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::allocate_gpumemory(std::string name, VkDeviceSize size) {
		auto res = 0;
		do {
			res = __allocate_gpumemory(name, size);
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::bind_buffermemory(std::string buffer_name, std::string memory_name, VkDeviceSize offset) {
		do {
			__vk_res = vkBindBufferMemory(__logic_device, __device_buffer[__algorithm_name + "::" + buffer_name], __device_memory[__algorithm_name + "::" + memory_name], offset);
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::allcate_buffermemory(std::string name, VkDeviceSize size) {
		auto res = 0;
		do {
			res = __allocate_gpubuffer(name, size);
			if (res != 0) break;
			res = __allocate_gpumemory(name, size);
			if (res != 0) break;
			__vk_res = vkBindBufferMemory(__logic_device, __device_buffer[name], __device_memory[name], 0);
			if (__vk_res != VK_SUCCESS) { res = static_cast<int>(VK_ERROR_OUT_OF_POOL_MEMORY); break; }
		} while (false);
		return res;
	}

	int vulkan::update_descriptorset(std::string buffer_name, std::string memory_name, VkDeviceSize size, uint32_t binding_index) {
		auto res = 0;
		do {
			VkDescriptorBufferInfo descriptor_buffer_info{
				__device_buffer[__algorithm_name + "::" + buffer_name],
				0,
				size
			};
			VkWriteDescriptorSet write_info{
				VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				nullptr,
				__descriptor_set[__algorithm_name],
				binding_index,
				0,
				1,
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				nullptr,
				& descriptor_buffer_info,
				nullptr
			};
			vkUpdateDescriptorSets(__logic_device, 1, &write_info, 0, nullptr);
		} while (false);
		return res;
	}

	int vulkan::read_cacheddata(std::string name, VkDeviceSize size, VkDeviceSize offset, void* host_data) {
		void* device_data{ nullptr };
		auto data_name = __algorithm_name + "::" + name;
		do {
			if (__device_memory.count(data_name) != 1) { return VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS; };
			VkMappedMemoryRange read_range{
				 VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
				 nullptr,
				 __device_memory[data_name],
				 offset,
				 size
			};
			__vk_res = vkMapMemory(__logic_device, __device_memory[data_name], offset, size, 0, &device_data);
			if (__vk_res != VK_SUCCESS) break;
			__vk_res = vkInvalidateMappedMemoryRanges(__logic_device, 1, &read_range);
			if (__vk_res != VK_SUCCESS) break;
			std::memcpy(host_data, device_data, size);
			vkUnmapMemory(__logic_device, __device_memory[data_name]);
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::write_cacheddata(std::string name, VkDeviceSize size, VkDeviceSize offset, void* host_data) {
		void* device_data{ nullptr };
		auto data_name = __algorithm_name + "::" + name;
		do {
			if (__device_memory.count(data_name) != 1) { return VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS; };
			VkMappedMemoryRange write_range{
				 VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
				 nullptr,
				 __device_memory[data_name],
				 offset,
				 size
			};
			__vk_res = vkMapMemory(__logic_device, __device_memory[name], offset, size, 0, &device_data);
			if (__vk_res != VK_SUCCESS) break;
			std::memcpy(device_data, host_data, size);
			__vk_res = vkFlushMappedMemoryRanges(__logic_device, 1, &write_range);
			if (__vk_res != VK_SUCCESS) break;
			vkUnmapMemory(__logic_device, __device_memory[name]);
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::transfer_normaldata(std::string name, VkDeviceSize size, VkDeviceSize offset, void* host_data, copy_oriention oriention) {
		void* device_data{ nullptr };
		auto data_name = __algorithm_name + "::" + name;
		do {
			if (__device_memory.count(data_name) != 1) { return VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS; };
			__vk_res = vkMapMemory(__logic_device, __device_memory[data_name], offset, size, 0, &device_data);
			if (__vk_res != VK_SUCCESS) break;
			oriention == copy_oriention::host_to_device ? std::memcpy(device_data, host_data, size) : std::memcpy(host_data, device_data, size);
			vkUnmapMemory(__logic_device, __device_memory[data_name]);
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::wait_queue(int x, int y, int z) {
		auto res = 0;
		do {
			res = __allocate_commandbuffer();
			if (res != 0) break;
			res = __command_operation(x, y, z);
			if (res != 0) break;
			res = __fence_operation();
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::__release_operation() {
		auto res = 0;
		do {
			res = __destroy_buffer();
			if (res != 0) break;
			res = __free_memory();
			if (res != 0) break;
			res = __destroy_shadermodule();
			if (res != 0) break;
			res = __destroy_descriptorpool();
			if (res != 0) break;
			res = __destroy_descriptorsetlayout();
			if (res != 0) break;
			res = __destroy_pipelinelayout();
			if (res != 0) break;
			res = __destroy_pipeline();
			if (res != 0) break;
			res = __destroy_commandpool();
			if (res != 0) break;
			res = __destroy_device();
			if (res != 0) break;
			res = __destroy_instance();
			if (res != 0) break;
		} while (false);
		return res;
	}

	int vulkan::__validate_layer() {
		do {
#ifdef _debug_report_
			__request_layers.push_back("VK_LAYER_KHRONOS_validation");
#endif // _debug_report_
			uint32_t layer_count;
			__vk_res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
			if (__vk_res != VK_SUCCESS) break;
			std::vector<VkLayerProperties> valid_layers(layer_count);
			__vk_res = vkEnumerateInstanceLayerProperties(&layer_count, valid_layers.data());
			if (__vk_res != VK_SUCCESS) break;
			for (const auto& r : __request_layers) {
				auto checked = false;
				for (const auto& v : valid_layers) {
					if (strcmp(r, v.layerName) == 0) checked = true;
				}
				if (checked == false) {
					__vk_res = VK_ERROR_LAYER_NOT_PRESENT; break;
				}
			}
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}
	int vulkan::__validate_extension() {
		do {
			__request_extensions.push_back(_platform_extension_);
			__request_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef _debug_report_
			__request_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif // _debug_report_
			uint32_t extension_count;
			__vk_res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
			if (__vk_res != VK_SUCCESS) break;
			std::vector<VkExtensionProperties> valid_extensions(extension_count);
			__vk_res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, valid_extensions.data());
			if (__vk_res != VK_SUCCESS) break;
			for (const auto& r : __request_extensions) {
				auto checked = false;
				for (const auto& v : valid_extensions) {
					if (strcmp(r, v.extensionName) == 0) checked = true;
				}
				if (checked == false) {
					__vk_res = VK_ERROR_EXTENSION_NOT_PRESENT; break;
				}
			}
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_instance(std::string application_name, std::string engine_name) {
		do {
			VkApplicationInfo app{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
			app.pEngineName = engine_name.c_str();
			app.apiVersion = VK_MAKE_VERSION(1, 0, 0);
			app.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			app.pApplicationName = application_name.c_str();
			app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			VkInstanceCreateInfo instance_create_info{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
			instance_create_info.pApplicationInfo = &app;
			instance_create_info.enabledLayerCount = static_cast<uint32_t>(__request_layers.size());
			instance_create_info.ppEnabledLayerNames = __request_layers.data();
			instance_create_info.enabledExtensionCount = static_cast<uint32_t>(__request_extensions.size());
			instance_create_info.ppEnabledExtensionNames = __request_extensions.data();
#ifdef _debug_report_
			VkDebugReportCallbackCreateInfoEXT debug_create_info = { VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT };
			debug_create_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			debug_create_info.pfnCallback = debug_callback;
			instance_create_info.pNext = &debug_create_info;
#endif // _debug_report_
			__vk_res = vkCreateInstance(&instance_create_info, nullptr, &__vk_instance);
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__select_device() {
		// physical device
		do {
			uint32_t device_count = 0;
			__vk_res = vkEnumeratePhysicalDevices(__vk_instance, &device_count, nullptr);
			if (__vk_res != VK_SUCCESS) break;
			if (device_count == 0) { __vk_res = VK_ERROR_DEVICE_LOST; break; }
			std::vector<VkPhysicalDevice> valid_devices(device_count);
			__vk_res = vkEnumeratePhysicalDevices(__vk_instance, &device_count, valid_devices.data());
			if (__vk_res != VK_SUCCESS) break;
			// select vilid device
			for (const auto& v : valid_devices) {
				uint32_t queue_family_count = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(v, &queue_family_count, nullptr);
				std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
				vkGetPhysicalDeviceQueueFamilyProperties(v, &queue_family_count, queue_families.data());
				for (const auto& q : queue_families) {
					if (q.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						__physical_device = v; break;
					}
					__device_index += 1;
				}
				if (__physical_device != nullptr) break;
			}
			if (__physical_device == nullptr) { __vk_res = VK_ERROR_DEVICE_LOST; break; }
			// detect the gpu device is integrated or discrete
			// vkGetPhysicalDeviceProperties
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_device() {
		// logic device
		do {
			float queue_priority = 1.0f;
			VkDeviceQueueCreateInfo queue_create_info{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				__device_index,
				1,
				&queue_priority
			};

			VkPhysicalDeviceFeatures device_features{};

			VkDeviceCreateInfo device_create_info{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
			device_create_info.pQueueCreateInfos = &queue_create_info;
			device_create_info.queueCreateInfoCount = 1;
			device_create_info.pEnabledFeatures = &device_features;
			device_create_info.ppEnabledLayerNames = __request_layers.data();
			device_create_info.enabledLayerCount = static_cast<uint32_t>(__request_layers.size());
			device_create_info.ppEnabledExtensionNames = nullptr;
			device_create_info.enabledExtensionCount = 0;
			// vkEnumerateDeviceLayerProperties
			// EnumerateDeviceExtensionProperties
			__vk_res = vkCreateDevice(__physical_device, &device_create_info, nullptr, &__logic_device);
			if (__vk_res != VK_SUCCESS) break;
			vkGetDeviceQueue(__logic_device, __device_index, 0, &__graphics_queue);
			if (__graphics_queue == nullptr) { __vk_res = VK_ERROR_DEVICE_LOST; break; }
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_swapchain() {
		do {
			__vk_res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(__physical_device, __vk_surface, &__capable_surface);
			if (__vk_res != VK_SUCCESS) break;
			uint32_t format_count;
			__vk_res = vkGetPhysicalDeviceSurfaceFormatsKHR(__physical_device, __vk_surface, &format_count, nullptr);
			if (__vk_res != VK_SUCCESS) break;

			__surface_format.resize(format_count);
			__vk_res = vkGetPhysicalDeviceSurfaceFormatsKHR(__physical_device, __vk_surface, &format_count, __surface_format.data());
			if (__vk_res != VK_SUCCESS) break;
			// check image format and color space

			uint32_t present_mode_count;
			__vk_res = vkGetPhysicalDeviceSurfacePresentModesKHR(__physical_device, __vk_surface, &present_mode_count, nullptr);
			if (__vk_res != VK_SUCCESS) break;

			__present_mode.resize(present_mode_count);
			__vk_res = vkGetPhysicalDeviceSurfacePresentModesKHR(__physical_device, __vk_surface, &present_mode_count, __present_mode.data());
			if (__vk_res != VK_SUCCESS) break;
			// check image mode


			VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
			createInfo.surface = __vk_surface;

			createInfo.minImageCount = 3;
			createInfo.imageFormat = VK_FORMAT_B8G8R8A8_UINT;
			createInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			createInfo.imageExtent = VkExtent2D{ 1080,720 };
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			//QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
			//uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

			//if (indices.graphicsFamily != indices.presentFamily) {
			//    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			//    createInfo.queueFamilyIndexCount = 2;
			//    createInfo.pQueueFamilyIndices = queueFamilyIndices;
			//} else {
			//    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			//}

			//createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			//createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			//createInfo.presentMode = presentMode;
			//createInfo.clipped = VK_TRUE;

			//createInfo.oldSwapchain = VK_NULL_HANDLE;

			//if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			//    throw std::runtime_error("failed to create swap chain!");
			//}

			//vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
			//swapChainImages.resize(imageCount);
			//vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

			//swapChainImageFormat = surfaceFormat.format;
			//swapChainExtent = extent;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_descriptorpool() {
		do {
			VkDescriptorPoolSize pool_size{
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				2
			};
			VkDescriptorPoolCreateInfo descriptorpool_create_info{
				VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				nullptr,
				0,
				2,
				1,
				&pool_size
			};
			__vk_res = vkCreateDescriptorPool(__logic_device, &descriptorpool_create_info, nullptr, &__descriptor_pool);
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_commandpool() {
		do {
			VkCommandPoolCreateInfo commandpool_create_info{
				VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				nullptr,
				0,
				__device_index
			};
			__vk_res = vkCreateCommandPool(__logic_device, &commandpool_create_info, nullptr, &__command_pool);
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_descriptorsetlayout(int count) {
		VkDescriptorSetLayout descriptorset_layout;
		auto bindings = new VkDescriptorSetLayoutBinding[count]{ 0 };
		do {
			if (__descriptorset_layout.count(__algorithm_name) == 1) break;
			for (auto i = 0; i < count; ++i) {
				bindings[i].binding = i;
				bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				bindings[i].descriptorCount = 1;
				bindings[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				bindings[i].pImmutableSamplers = nullptr;
			}
			VkDescriptorSetLayoutCreateInfo descriptorsetlayout_create_info{
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				nullptr,
				0,
				count,
				bindings
			};
			__vk_res = vkCreateDescriptorSetLayout(__logic_device, &descriptorsetlayout_create_info, nullptr, &descriptorset_layout);
			if (__vk_res != VK_SUCCESS) break;
			__descriptorset_layout[__algorithm_name] = descriptorset_layout;
		} while (false);
		delete[] bindings;
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_descriptorset() {
		VkDescriptorSet descriptor_set;
		do {
			if (__descriptor_set.count(__algorithm_name) == 1) break;
			VkDescriptorSetAllocateInfo descriptorset_allocate_info{
				VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				nullptr,
				__descriptor_pool,
				1,
				&__descriptorset_layout[__algorithm_name]
			};
			__vk_res = vkAllocateDescriptorSets(__logic_device, &descriptorset_allocate_info, &descriptor_set);
			if (__vk_res != VK_SUCCESS) break;
			__descriptor_set[__algorithm_name] = descriptor_set;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__create_pipeline(std::string shader) {
		VkPipeline computer_pipeline;
		VkShaderModule shader_module;
		VkPipelineLayout pipeline_layout;
		do {
			if (__shader_module.count(__algorithm_name) == 1) break;
			VkShaderModuleCreateInfo shadermodule_create_info{
				VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				nullptr,
				0,
				shader.size(),
				reinterpret_cast<const uint32_t*>(shader.c_str())
			};
			__vk_res = vkCreateShaderModule(__logic_device, &shadermodule_create_info, nullptr, &shader_module);
			if (__vk_res != VK_SUCCESS) break;
			__shader_module[__algorithm_name] = shader_module;
			VkPipelineLayoutCreateInfo pipelinelayout_create_info{
				VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				nullptr,
				0,
				1,
				&__descriptorset_layout[__algorithm_name],
				0,
				nullptr
			};
			__vk_res = vkCreatePipelineLayout(__logic_device, &pipelinelayout_create_info, nullptr, &pipeline_layout);
			if (__vk_res != VK_SUCCESS) break;
			__pipeline_layout[__algorithm_name] = pipeline_layout;
			VkPipelineShaderStageCreateInfo shaderstage_create_info{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				0,
				VK_SHADER_STAGE_COMPUTE_BIT,
				shader_module,
				_shader_entry_point_,
				nullptr
			};
			VkComputePipelineCreateInfo pipeline_create_info{
				VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
				nullptr,
				0,
				shaderstage_create_info,
				pipeline_layout,
				nullptr,
				0
			};
			__vk_res = vkCreateComputePipelines(__logic_device, nullptr, 1, &pipeline_create_info, nullptr, &computer_pipeline);
			if (__vk_res != VK_SUCCESS) break;
			__computer_pipeline[__algorithm_name] = computer_pipeline;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__allocate_gpubuffer(std::string name, VkDeviceSize size) {
		VkBuffer vk_buffer;
		auto buffer_name = __algorithm_name + "::" + name;
		do {
			if (__device_buffer.count(buffer_name) == 1) break;
			VkBufferCreateInfo buffer_create_info{
				VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				nullptr,
				0,
				size,
				VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				0,
				nullptr
			};
			__vk_res = vkCreateBuffer(__logic_device, &buffer_create_info, nullptr, &vk_buffer);
			if (__vk_res != VK_SUCCESS) break;
			__device_buffer[buffer_name] = vk_buffer;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__allocate_gpumemory(std::string name, VkDeviceSize size) {
		VkDeviceMemory device_memory;
		uint32_t memory_index = UINT32_MAX;
		VkPhysicalDeviceMemoryProperties properties;
		auto memory_name = __algorithm_name + "::" + name;
		do {
			if (__device_memory.count(memory_name) == 1) break;
			vkGetPhysicalDeviceMemoryProperties(__physical_device, &properties);
			for (auto i = 0; i < properties.memoryTypeCount; ++i) {
				auto type = properties.memoryTypes[i];
#ifdef _debug_report_
				std::cout << "memory [" << i << "] : " << std::endl;
				if (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT & type.propertyFlags) {
					std::cout << "\tVK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT" << std::endl;
				}
				if (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT & type.propertyFlags) {
					std::cout << "\tVK_MEMORY_PROPERTY_HOST_VISIBLE_BIT" << std::endl;
				}
				if (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT & type.propertyFlags) {
					std::cout << "\tVK_MEMORY_PROPERTY_HOST_COHERENT_BIT" << std::endl;
				}
				if (VK_MEMORY_PROPERTY_HOST_CACHED_BIT & type.propertyFlags) {
					std::cout << "\tVK_MEMORY_PROPERTY_HOST_CACHED_BIT" << std::endl;
				}
				if (VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT & type.propertyFlags) {
					std::cout << "\tVK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT" << std::endl;
				}
				if (VK_MEMORY_PROPERTY_PROTECTED_BIT & type.propertyFlags) {
					std::cout << "\tVK_MEMORY_PROPERTY_PROTECTED_BIT" << std::endl;
				}
#endif
				if (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
					//if (0 == (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
					//	auto memory_is_cached = 1;
					//}
					memory_index = i;
					break;
				}
			}
			if (memory_index == UINT32_MAX) {
				__vk_res = VK_ERROR_OUT_OF_POOL_MEMORY;
				break;
			}
			VkMemoryAllocateInfo memory_allocate_info{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				size,
				memory_index
			};
			__vk_res = vkAllocateMemory(__logic_device, &memory_allocate_info, nullptr, &device_memory);
			if (__vk_res != VK_SUCCESS) break;
			__device_memory[memory_name] = device_memory;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__allocate_commandbuffer() {
		VkCommandBuffer command_buffer;
		do {
			if (__command_buffer.count(__algorithm_name) == 1) break;
			VkCommandBufferAllocateInfo commandbuffer_allocate_info{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				nullptr,
				__command_pool,
				VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				1
			};
			__vk_res = vkAllocateCommandBuffers(__logic_device, &commandbuffer_allocate_info, &command_buffer);
			if (__vk_res != VK_SUCCESS) break;
			__command_buffer[__algorithm_name] = command_buffer;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__command_operation(int x, int y, int z) {
		do {
			VkCommandBufferBeginInfo commandbufffer_begin_info{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				nullptr,
				VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				nullptr
			};
			__vk_res = vkBeginCommandBuffer(__command_buffer[__algorithm_name], &commandbufffer_begin_info);
			if (__vk_res != VK_SUCCESS) break;
			vkCmdBindPipeline(__command_buffer[__algorithm_name], VK_PIPELINE_BIND_POINT_COMPUTE, __computer_pipeline[__algorithm_name]);
			vkCmdBindDescriptorSets(
				__command_buffer[__algorithm_name],
				VK_PIPELINE_BIND_POINT_COMPUTE,
				__pipeline_layout[__algorithm_name],
				0,
				1,
				&__descriptor_set[__algorithm_name],
				0,
				nullptr
			);
			vkCmdDispatch(__command_buffer[__algorithm_name], x, y, z);
			__vk_res = vkEndCommandBuffer(__command_buffer[__algorithm_name]);
			if (__vk_res != VK_SUCCESS) break;
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__fence_operation() {
		const auto _max_time_ = 60 * 1e9;
		// TODO: can not protect fence
		do {
		VkFence fence;
			VkFenceCreateInfo fence_create_info{
				VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				nullptr,
				0
			};
			__vk_res = vkCreateFence(__logic_device, &fence_create_info, nullptr, &fence);
			if (__vk_res != VK_SUCCESS) break;
			VkSubmitInfo submit_info{
				VK_STRUCTURE_TYPE_SUBMIT_INFO,
				nullptr,
				0,
				nullptr,
				nullptr,
				1,
				&__command_buffer[__algorithm_name],
				0,
				nullptr
			};
			__vk_res = vkQueueSubmit(__graphics_queue, 1, &submit_info, fence);
			if (__vk_res != VK_SUCCESS) break;
			__vk_res = vkWaitForFences(__logic_device, 1, &fence, VK_TRUE, _max_time_);
			if (__vk_res != VK_SUCCESS) break;
			vkDestroyFence(__logic_device, fence, nullptr);
		} while (false);
		return static_cast<int>(__vk_res);
	}

	int vulkan::__destroy_buffer() {
		do {
			for (auto [_, buffer] : __device_buffer) {
				vkDestroyBuffer(__logic_device, buffer, nullptr);
			}
		} while (false);
		return 0;
	}

	int vulkan::__free_memory() {
		do {
			for (auto [_, memory] : __device_memory) {
				vkFreeMemory(__logic_device, memory, nullptr);
			}
		} while (false);
		return 0;
	}

	int vulkan::__destroy_shadermodule() {
		do {
			for (auto [_, shader] : __shader_module) {
				vkDestroyShaderModule(__logic_device, shader, nullptr);
			}
		} while (false);
		return 0;
	}

	int vulkan::__destroy_descriptorpool() {
		do {
			vkDestroyDescriptorPool(__logic_device, __descriptor_pool, nullptr);
		} while (false);
		return 0;
	}

	int vulkan::__destroy_descriptorsetlayout() {
		do {
			for (auto [_, descriptorset_layout] : __descriptorset_layout) {
				vkDestroyDescriptorSetLayout(__logic_device, descriptorset_layout, nullptr);
			}
		} while (false);
		return 0;
	}

	int vulkan::__destroy_pipelinelayout() {
		do {
			for (auto [_, pipeline_layout] : __pipeline_layout) {
				vkDestroyPipelineLayout(__logic_device, pipeline_layout, nullptr);
			}
		} while (false);
		return 0;
	}

	int vulkan::__destroy_pipeline() {
		do {
			for (auto [_, pipeline] : __computer_pipeline) {
				vkDestroyPipeline(__logic_device, pipeline, nullptr);
			}
		} while (false);
		return 0;
	}

	int vulkan::__destroy_commandpool() {
		do {
			vkDestroyCommandPool(__logic_device, __command_pool, nullptr);
		} while (false);
		return 0;
	}

	int vulkan::__destroy_device() {
		do {
			vkDestroyDevice(__logic_device, nullptr);
		} while (false);
		return 0;
	}

	int vulkan::__destroy_instance() {
		do {
			vkDestroyInstance(__vk_instance, nullptr);
		} while (false);
		return 0;
	}

}
