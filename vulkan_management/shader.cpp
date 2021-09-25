/*
 * @Author: your name
 * @Date: 2021-09-23 09:43:39
 * @LastEditTime: 2021-09-23 09:47:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_management\shader.cpp
 */

#include <string>

#include <shader.hpp>

#include <vulkan/vulkan.h>

namespace harpocrates {
	VkShaderStageFlagBits find_shader_stage(const std::string& ext) {
		if (ext == "vert")
		{
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (ext == "frag")
		{
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		}
		else if (ext == "comp")
		{
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		else if (ext == "geom")
		{
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		else if (ext == "tesc")
		{
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		}
		else if (ext == "tese")
		{
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		}
		else if (ext == "rgen")
		{
			return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		}
		else if (ext == "rahit")
		{
			return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
		}
		else if (ext == "rchit")
		{
			return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		}
		else if (ext == "rmiss")
		{
			return VK_SHADER_STAGE_MISS_BIT_KHR;
		}
		else if (ext == "rint")
		{
			return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
		}
		else if (ext == "rcall")
		{
			return VK_SHADER_STAGE_CALLABLE_BIT_KHR;
		}
	}

}