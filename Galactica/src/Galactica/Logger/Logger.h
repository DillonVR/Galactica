#pragma once

#include "Galactica/Core.h"


namespace Galactica {

	enum LogType {
		LOG_INFO,
		LOG_WARNING,
		LOG_ERROR
	};

	struct LogEntry {
		LogType type;
		std::string message;
	};

	class GALACTICA_API Logger
	{
	public:
		static std::vector<LogEntry> messages;
		static void Log(const std::string& message);
		static void Err(const std::string& message);
		static void Evt(const std::string& message);
	};
}


//Core Log Macros
#define GL_LOGGER_INFO(...) ::Galactica::Logger::Log(__VA_ARGS__)
#define GL_LOGGER_ERR(...) ::Galactica::Logger::Err(__VA_ARGS__)
#define GL_LOGGER_EVT(...) ::Galactica::Logger::Evt(__VA_ARGS__)