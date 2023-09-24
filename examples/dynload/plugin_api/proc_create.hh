#pragma once
#include "plugin_api/plugin.hh"
#include "processor.hh"

template<typename DerivedProc>
ProcessorBase *create(std::string_view name)
{
	static DerivedProc proc;
	return &proc;

	// static auto plugin =
	// 	Plugin{[]() -> std::unique_ptr<ProcessorBase> { return std::make_unique<DerivedProc>(); }, name};

	// return &plugin;
}
