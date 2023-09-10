#include "glpch.h"
#include "Logger.h"

////////////////////////////////////////////////////////////////////////////////
// Logger
////////////////////////////////////////////////////////////////////////////////
// Logs the ongoings of the Engine.
// Log - green text
// err - red text 
// evt - white text
////////////////////////////////////////////////////////////////////////////////


namespace Galactica {

	std::vector<LogEntry> Logger::messages;

	std::string CurrentDateTimetoString() {
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string output(30, '\0');
		std::strftime(&output[0], output.size(), "%d-%b-%y %H:%M:%S", std::localtime(&now));
		return output;
	}
	void Logger::Log(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.type = LOG_INFO;
		logEntry.message = "LOG: [" + CurrentDateTimetoString() + "]: " + message;
		std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;

		messages.push_back(logEntry);
	}

	void Logger::Err(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.type = LOG_ERROR;
		logEntry.message = "ERR: [" + CurrentDateTimetoString() + "]: " + message;
		std::cout << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;

		messages.push_back(logEntry);
	}

	void Logger::Evt(const std::string& message)
	{
		LogEntry logEntry;
		logEntry.type = LOG_INFO;
		logEntry.message = "Evt: [" + CurrentDateTimetoString() + "]: " + message;
		std::cout << "\x1B[57m" << logEntry.message << "\033[0m" << std::endl;

		messages.push_back(logEntry);
	}
}