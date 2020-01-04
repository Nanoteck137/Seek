#include <iostream>

#include <Seek/Log.h>

int main(int argc, char **argv)
{
	Seek::Log::Init();

	Seek::Log::GetCoreLogger()->info("Hello World");
	Seek::Log::GetCoreLogger()->warn("Hello World");
	Seek::Log::GetCoreLogger()->error("Hello World");
	Seek::Log::GetCoreLogger()->critical("Hello World");

	SK_CORE_TRACE("Hello World");
	SK_CORE_INFO("Hello World");
	SK_CORE_WARNING("Hello World");
	SK_CORE_ERROR("Hello World");
	SK_CORE_FATAL("Hello World");

	Seek::Log::GetClientLogger()->info("Hello World");
	Seek::Log::GetClientLogger()->warn("Hello World");
	Seek::Log::GetClientLogger()->error("Hello World");
	Seek::Log::GetClientLogger()->critical("Hello World");

	SK_APP_TRACE("Hello World");
	SK_APP_INFO("Hello World");
	SK_APP_WARNING("Hello World");
	SK_APP_ERROR("Hello World");
	SK_APP_FATAL("Hello World");

	return 0;
}