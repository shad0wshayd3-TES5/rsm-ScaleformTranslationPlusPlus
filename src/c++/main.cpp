#include "Events.h"
#include "Hooks.h"

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kInputLoaded:
			{
				auto UI = RE::UI::GetSingleton();
				UI->AddEventSink(Events::MenuOpenCloseEventHandler::GetSingleton());

				Hooks::InstallHooks();
			}
			break;
		}
	}

	void InitializeLog()
	{
		auto path = logger::log_directory();
		if (!path)
		{
			stl::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("[%^%l%$] %v"s);

		logger::info(FMT_STRING("{:s} v{:s}"), Version::PROJECT, Version::NAME);
	}
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
{
	SKSE::PluginVersionData v;
	v.PluginVersion(Version::MAJOR);
	v.PluginName(Version::PROJECT);
	v.AuthorName("shad0wshayd3");
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	v.CompatibleVersions({ SKSE::RUNTIME_LATEST });

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	logger::info(FMT_STRING("{:s} loaded"), Version::PROJECT);

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(1 << 4);

	const auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		logger::error(FMT_STRING("Failed to register MessageHandler."sv));
		return false;
	}

	return true;
}
