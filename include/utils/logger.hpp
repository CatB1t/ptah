#pragma once

#include <spdlog/spdlog.h>

#include <memory>

namespace ptah::utils {
class Logger {
 private:
  static std::shared_ptr<spdlog::logger> s_RenderLogger;

 public:
  static void Init();
  static spdlog::logger& Renderer();
};
}  // namespace ptah::utils

#define PTAH_RENDER_DEBUG(...) \
  ptah::utils::Logger::Renderer().debug(__VA_ARGS__)
#define PTAH_RENDER_INFO(...) ptah::utils::Logger::Renderer().info(__VA_ARGS__)
#define PTAH_RENDER_WARN(...) ptah::utils::Logger::Renderer().warn(__VA_ARGS__)
#define PTAH_RENDER_ERROR(...) \
  ptah::utils::Logger::Renderer().error(__VA_ARGS__)