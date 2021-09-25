/*
 * @Author: your name
 * @Date: 2021-09-06 09:18:43
 * @LastEditTime: 2021-09-06 09:21:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_management\vulkan_management.hpp
 */

#include <map>
#include <vector>

#include <vulkan/vulkan.h>

#include <spirv.hpp>

namespace harpocrates {
	enum class copy_oriention {
		host_to_device,
		device_to_host,
	};
	class vulkan {
	public:
		vulkan();
		~vulkan();
	public:
		int set_algorithm(std::string name);
		int create_descriptorpool();
		int create_commandpool();
		int create_descriptorsetlayout(int count);
		int create_descriptorset();
		int create_pipeline(std::string shader);
		// one memory multi buffers with offset
		int allocate_gpubuffer(std::string name, VkDeviceSize size);
		int allocate_gpumemory(std::string name, VkDeviceSize size);
		int bind_buffermemory(std::string buffer_name, std::string memory_name, VkDeviceSize offset);
		// one buffer mapped one memory interface
		int allcate_buffermemory(std::string name, VkDeviceSize size);
		// build connection with logic device
		int update_descriptorset(std::string buffer_name, std::string memory_name, VkDeviceSize size, uint32_t binding_index);
		// can do caculation after mapped operation
		// use template funtion with data operation
		int read_cacheddata(std::string name, VkDeviceSize size, VkDeviceSize offset, void* host_data);
		int write_cacheddata(std::string name, VkDeviceSize size, VkDeviceSize offset, void* host_data);
		int transfer_normaldata(std::string name, VkDeviceSize size, VkDeviceSize offset, void* host_data, copy_oriention oriention);
		// wait queue excution
		int wait_queue(int x, int y, int z);
	private:
		int __release_operation();
		int __validate_layer();
		int __validate_extension();
		int __create_instance(std::string application_name, std::string engine_name);
		int __select_device();
		int __create_device();
		int __create_swapchain();
		int __create_descriptorpool();
		int __create_commandpool();
		int __create_descriptorsetlayout(int count);
		int __create_descriptorset();
		int __create_pipeline(std::string shader);
		// vulkan memory can bind many buffers by offset
		int __allocate_gpubuffer(std::string name, VkDeviceSize size);
		int __allocate_gpumemory(std::string name, VkDeviceSize size);
		int __allocate_commandbuffer();
		int __command_operation(int x, int y, int z = 1);
		int __fence_operation();
		// destroy resource
		int __destroy_buffer();
		int __free_memory();
		int __destroy_shadermodule();
		int __destroy_descriptorpool();
		int __destroy_descriptorsetlayout();
		int __destroy_pipelinelayout();
		int __destroy_pipeline();
		int __destroy_commandpool();
		int __destroy_device();
		int __destroy_instance();
	private:
		VkResult __vk_res;
		VkDevice __logic_device;
		uint32_t __device_index;
		VkInstance __vk_instance;
		VkQueue __graphics_queue;
		VkSurfaceKHR __vk_surface;
		std::string __algorithm_name;
		VkCommandPool __command_pool;
		VkDescriptorPool __descriptor_pool;
		std::vector<VkPresentModeKHR> __present_mode;
		std::vector<VkSurfaceFormatKHR> __surface_format;
		VkSurfaceCapabilitiesKHR __capable_surface;
		VkPhysicalDevice __physical_device;
		std::vector<const char*> __request_layers;
		std::vector<const char*> __request_extensions;
		std::map<std::string, VkBuffer> __device_buffer;
		std::map<std::string, VkShaderModule> __shader_module;
		std::map<std::string, VkPipeline> __computer_pipeline;
		std::map<std::string, VkDeviceMemory> __device_memory;
		std::map<std::string, VkCommandBuffer> __command_buffer;
		std::map<std::string, VkDescriptorSet> __descriptor_set;
		std::map<std::string, VkPipelineLayout> __pipeline_layout;
		std::map<std::string, VkDescriptorSetLayout> __descriptorset_layout;
	};

}
