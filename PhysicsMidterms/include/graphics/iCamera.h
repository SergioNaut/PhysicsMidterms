#pragma once

namespace nGraphics
{
	class iCamera
	{
	public:
		virtual ~iCamera() {}
		virtual void OnWindowResize(int newWidth, int newHeight) = 0;
	};
}