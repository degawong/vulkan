/*
 * @Author: your name
 * @Date: 2021-09-23 09:53:40
 * @LastEditTime: 2021-09-23 09:53:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_management\spirv.cpp
 */

#include <vector>
#include <fstream>
#include <spirv.hpp>

namespace harpocrates {
	int spirv::set_shader(std::string name, std::string path) {
		auto res = 0;
		do {
			if (__shader.count(name) == 1) break;
			std::ifstream source_file{ path, std::ios::ate | std::ios::binary};
			//size_t fileSize = (size_t)source_file.tellg();
			source_file.seekg(0);
			std::string shader{ std::istreambuf_iterator<char>(source_file), (std::istreambuf_iterator<char>()) };
			if (shader.size() < 1) { res = 1; break; }
			__shader[name] = shader;
		} while (false);
		return res;
	}
	std::string spirv::get_shader(std::string name) {
		if (__shader.count(name) == 1) return __shader[name];
		return std::string{};
	}
}