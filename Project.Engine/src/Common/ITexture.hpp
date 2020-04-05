#pragma once

#include <string>
namespace Common
{
	class ITexture
	{
		public:
			virtual ~ITexture(){}
			virtual int Load(std::string path, int nbWidth = -1, int nbHeight = -1) = 0;
			virtual void Use(int x = -1, int y = -1) = 0;
	};
}