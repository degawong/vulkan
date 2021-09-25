/*
 * @Author: your name
 * @Date: 2021-09-23 09:53:35
 * @LastEditTime: 2021-09-23 09:53:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_management\spirv.hpp
 */

#include <map>
#include <string>

namespace harpocrates {
	//struct shader_information {
	//	size_t size;
	//	uint32_t* data;
	//};
    class spirv {
    public:
        spirv() = default;
        ~spirv() = default;
    public:
        int set_shader(std::string name, std::string path);
        std::string get_shader(std::string name);
    private:
        std::map<std::string, std::string> __shader;
    };
}