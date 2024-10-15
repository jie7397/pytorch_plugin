#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" 
#include "spdlog/sinks/basic_file_sink.h" 

#define LOG_INFO(...)      spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)      spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)     spdlog::error(__VA_ARGS__)
#define LOG_DEBUG(...)     spdlog::debug(__VA_ARGS__)
#define LOG_CRITICAL(...)  spdlog::critical(__VA_ARGS__)
