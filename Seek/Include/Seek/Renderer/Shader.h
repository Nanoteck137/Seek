#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

namespace Seek
{
	class Shader
	{
	public:
		Shader(const String& vertexSource, const String& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32 m_RendererID;
	};
}