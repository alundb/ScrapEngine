#pragma once
#include <cstdint>

namespace vk
{
	class Buffer;
}

namespace ScrapEngine
{
	namespace Render
	{
		class BufferContainer
		{
		private:
			vk::Buffer* buffer_;
		public:
			BufferContainer(vk::Buffer* input_buffer);
			virtual ~BufferContainer() = 0;

			//Function to get the buffer
			vk::Buffer* get_buffer() const;
			//Conversion operators
			operator vk::Buffer() const;
		};
	}
}
