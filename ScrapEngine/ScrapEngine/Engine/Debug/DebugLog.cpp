#include <Engine/Debug/DebugLog.h>
#include <iomanip>
#include <chrono>
#include <string>
#include <Engine/Rendering/VulkanInclude.h>
#include <Engine/LogicCore/Math/Vector/SVector3.h>
#include <Engine/LogicCore/Math/Quaternion/SQuaternion.h>

void ScrapEngine::Debug::DebugLog::print_to_console_log(const std::string& log_string)
{
	std::cout << now_to_string() << log_string.c_str() << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_to_console_log(const glm::vec3& vector)
{
	std::cout << now_to_string() << "Vec3: (" << vector.x << ", "
		<< vector.y << ", " << vector.z << ")" << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_to_console_log(const glm::quat& quaternion)
{
	std::cout << now_to_string() << "Quat: (" << quaternion.x << ", "
		<< quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")" << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_init_message()
{
	std::cout << "//INIT MESSAGE" << std::endl;
	std::cout << "Day format used: day/month/year" << std::endl;
	std::cout << "Time format used: hours(24h):minutes:seconds" << std::endl;
	std::cout << "Begin time:" << now_to_string() << std::endl;
	std::cout << "------------------------" << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_to_console_log(const Core::SVector3& vector)
{
	print_to_console_log(vector.get_glm_vector());
}

void ScrapEngine::Debug::DebugLog::print_to_console_log(const Core::SQuaternion& quaternion)
{
	print_to_console_log(quaternion.get_glm_quat());
}

void ScrapEngine::Debug::DebugLog::print_to_console_log(const glm::mat4& matrix)
{
	std::cout << "[" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		std::cout << "    [ ";
		for (int k = 0; k < 4; k++)
		{
			std::cout << matrix[i][k];
			if (k + 1 < 4)
			{
				std::cout << " | ";
			}
		}
		std::cout << " ]" << std::endl;
	}
	std::cout << "]" << std::endl;
}

void ScrapEngine::Debug::DebugLog::print_exception_to_console_log(const std::string& message_severity,
                                                                  const std::string& exception_string)
{
	std::cout << "--------------------" << std::endl;
	std::cout << "print_exception_to_console_log() call" << std::endl;
	std::cout << std::endl << now_to_string() << message_severity << exception_string << std::endl << std::endl;
	std::cout << "--------------------" << std::endl;
}

void ScrapEngine::Debug::DebugLog::fatal_error(vk::Result error_result, const std::string& error_message)
{
	VkResult* res = reinterpret_cast<VkResult*>(&error_result);
	fatal_error(*res, error_message);
}

void ScrapEngine::Debug::DebugLog::fatal_error(const VkResult error_result, const std::string& error_message)
{
	const int error_code = error_result;
	if(error_code == -13)
	{
		throw std::runtime_error(error_message + " - (ERROR_UNKNOWN)");
	}
	else {
		throw std::runtime_error(error_message + " - Error code(VkResult):" + std::to_string(error_code));
	}
}

std::string ScrapEngine::Debug::DebugLog::now_to_string()
{
	struct tm timeinfo{};
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        // "As you can see, the standard C and the Microsoft functions localtime_r() have very different interfaces — the arguments are in the reverse order and the return type is different. By contrast, the difference between the Standard C and POSIX functions is just the name." See: https://stackoverflow.com/questions/69992446/c-location-of-localtime-s-in-gcc
	// localtime_s(&timeinfo, &now);
        localtime_r(&now, &timeinfo);

	return "[" + std::to_string(timeinfo.tm_mday) + "-" + std::to_string(timeinfo.tm_mon + 1) + "-" + std::to_string(
			timeinfo.tm_year + 1900)
		+ " " + std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(
			timeinfo.tm_sec) + "]";
}
