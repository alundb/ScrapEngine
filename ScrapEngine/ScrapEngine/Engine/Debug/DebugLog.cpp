#include "DebugLog.h"
#include <iomanip>
#include <chrono>
#include <string>

void ScrapEngine::Debug::DebugLog::print_to_console_log(const std::string& logString)
{
	std::cout << now_to_string().c_str() << logString.c_str() << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_to_console_log(const glm::vec3& vector)
{
	std::cout << now_to_string().c_str() << "Vec3: (" << vector.x << ", " << vector.y << ", " << vector.z << ")" << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_exception_to_console_log(const std::string& message_severity, const std::string& exception_string)
{
	std::cout << "--------------------" << std::endl;
	std::cout << "print_exception_to_console_log() call" << std::endl;
	std::cout << std::endl << now_to_string().c_str() << message_severity << exception_string << std::endl << std::endl;
	std::cout << "--------------------" << std::endl;
}

std::string ScrapEngine::Debug::DebugLog::now_to_string()
{
	struct tm timeinfo{};
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	localtime_s(&timeinfo, &now);

	return "[" + std::to_string(timeinfo.tm_mday) + "-" + std::to_string(timeinfo.tm_mon + 1) + "-" + std::to_string(timeinfo.tm_year + 1900)
		+ " " + std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(timeinfo.tm_sec) + "]";
}
