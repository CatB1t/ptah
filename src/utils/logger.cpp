#include "utils/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace ptah::utils {

std::shared_ptr<spdlog::logger> Logger::s_RenderLogger;

void Logger::Init() {
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  s_RenderLogger = std::make_shared<spdlog::logger>("Renderer", console_sink);
  s_RenderLogger->set_level(spdlog::level::debug);
}

spdlog::logger& Logger::Renderer() { return *s_RenderLogger; }
}  // namespace ptah::utils