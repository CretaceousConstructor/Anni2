#pragma once

#define ANNI_VK_API_VERSION VK_API_VERSION_1_3

#ifndef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef VK_ZERO_FLAG
#define VK_ZERO_FLAG 0
#endif

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#ifndef VULKAN_HPP_NO_EXCEPTIONS
#define VULKAN_HPP_NO_EXCEPTIONS
#endif

//#ifndef VULKAN_HPP_NO_CONSTRUCTORS
//#define VULKAN_HPP_NO_CONSTRUCTORS
//#endif

#ifndef VK_CHECK_RESULT
#define VK_CHECK_RESULT(f)                                                     \
  do {                                                                         \
    VkResult res = (f);                                                        \
    if (res != VK_SUCCESS) {                                                   \
      std::cout << "Fatal: VkResult is " << res << " in " << __FILE__          \
                << " at line " << __LINE__ << "\n";                            \
      assert(res == VK_SUCCESS);                                               \
    }                                                                          \
  } while (false)
#endif

#ifndef ASSERT_WITH_MSG
#define ASSERT_WITH_MSG(condition, message)                                \
  do {                                                                         \
    if (!(condition)) {                                                        \
      std::cerr << "Assertion failed: " << #condition << ", " << message       \
                << '\n';                                                  \
      assert(condition);                                                       \
    }                                                                          \
  } while (false)
#endif


#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_hash.hpp>
#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_shared.hpp>
#include <vulkan/vulkan_static_assertions.hpp>
#include <vulkan/vulkan_hpp_macros.hpp>
