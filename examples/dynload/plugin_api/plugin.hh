#pragma once
#include "plugin_api/processor.hh"
#include <functional>
#include <string_view>

struct Plugin {
	// std::function<std::unique_ptr<ProcessorBase>()> createProc;
	std::string_view name;
};
