#pragma once

#include "../Core.h"

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>

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
