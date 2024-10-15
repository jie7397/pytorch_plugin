#include "log.h"

__attribute__((constructor))
void init_logging() {
    spdlog::set_pattern("[%H:%M:%S %z] [%^ %L %$] [thread %t] %v");
    spdlog::cfg::load_env_levels();
}

__attribute__((destructor))
void shutdown_logging() {
    spdlog::shutdown();
}