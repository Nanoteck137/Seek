#pragma once

#include <memory>

#include <spdlog/spdlog.h>

namespace Seek
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define SK_CORE_TRACE(...) ::Seek::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SK_CORE_INFO(...) ::Seek::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SK_CORE_WARNING(...) ::Seek::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SK_CORE_ERROR(...) ::Seek::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SK_CORE_FATAL(...) ::Seek::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SK_APP_TRACE(...) ::Seek::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SK_APP_INFO(...) ::Seek::Log::GetClientLogger()->info(__VA_ARGS__)
#define SK_APP_WARNING(...) ::Seek::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SK_APP_ERROR(...) ::Seek::Log::GetClientLogger()->error(__VA_ARGS__)
#define SK_APP_FATAL(...) ::Seek::Log::GetClientLogger()->critical(__VA_ARGS__)