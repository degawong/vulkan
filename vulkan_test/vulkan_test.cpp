/*
 * @Author: your name
 * @Date: 2021-09-03 16:12:08
 * @LastEditTime: 2021-09-06 09:21:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \arcsoft_algorithm\algorithm\vulkan\vulkan_test\vulkan_test.cpp
 */

#include <iostream>
#include <gtest/gtest.h>
#include <vulkan_algorithm.hpp>
#include <vulkan_management.hpp>

TEST(vulkan, _vulkan_hello_world_) {
    auto res = 0;
	auto data_out = new int[1024] {0};
	auto data_in_1 = new int[1024] {0};
	auto data_in_2 = new int[1024] {0};
	auto vk_engine = harpocrates::vulkan{};
	auto spirv_shader = harpocrates::spirv{};
	do {
		for (auto i = 0; i < 1024; ++i) {
			data_in_1[i] = i;
		}
		res = vk_engine.set_algorithm("hello_world");
		if (res != 0) break;
		res = vk_engine.create_descriptorpool();
		if (res != 0) break;
		res = vk_engine.create_commandpool();
		if (res != 0) break;
		res = vk_engine.create_descriptorsetlayout(2);
		if (res != 0) break;
		res = vk_engine.create_descriptorset();
		if (res != 0) break;
		res = spirv_shader.set_shader("hello_world", "hello_world.spv");
		if (res != 0) break;
		auto hello_world = spirv_shader.get_shader("hello_world");
		if (hello_world.empty()) break;
		res = vk_engine.create_pipeline(hello_world);
		if (res != 0) break;
		res = vk_engine.allocate_gpumemory("memeory", 1024 * 2 * 4);
		if (res != 0) break;
		res = vk_engine.allocate_gpubuffer("in_1", 1024 * 4);
		if (res != 0) break;
		res = vk_engine.allocate_gpubuffer("in_2", 1024 * 4);
		if (res != 0) break;
		res = vk_engine.bind_buffermemory("in_1", "memeory", 0);
		if (res != 0) break;
		res = vk_engine.bind_buffermemory("in_2", "memeory", 1024 * 4);
		if (res != 0) break;
		res = vk_engine.update_descriptorset("in_1", "memeory", 1024 * 4, 0);
		if (res != 0) break;
		res = vk_engine.update_descriptorset("in_2", "memeory", 1024 * 4, 1);
		if (res != 0) break;
		res = vk_engine.transfer_normaldata("memeory", 1024 * 4, 0, data_in_1, harpocrates::copy_oriention::host_to_device);
		if (res != 0) break;
		res = vk_engine.transfer_normaldata("memeory", 1024 * 4, 1024 * 4, data_in_2, harpocrates::copy_oriention::host_to_device);
		if (res != 0) break;
		res = vk_engine.wait_queue(1024, 1, 1);
		if (res != 0) break;
		res = vk_engine.transfer_normaldata("memeory", 1024 * 4, 1024 * 4, data_out, harpocrates::copy_oriention::device_to_host);
		if (res != 0) break;
    } while (false);
	delete[] data_out;
	delete[] data_in_1;
	delete[] data_in_2;
    EXPECT_EQ(res, 0);
}
