#pragma once

#include <string>
namespace Common
{
	/**
	 * @brief Base texture interface.
	 * 
	 */
	class ITexture
	{
		public:
			virtual ~ITexture(){}
			/**
			 * @brief Loads a texture in memory.
			 * 
			 * @param path : The path to the texture
			 * @param nbWidth : Number of tiles in width (for tilemaps).
			 * @param nbHeight : Number of tiles in height (for tilemaps).
			 * @return int 
			 */
			virtual int Load(std::string path, int nbWidth = -1, int nbHeight = -1) = 0;

			/**
			 * @brief Tells the graphical context to use the texture for rendering.
			 * 
			 * @param x : Delta x for tilemaps.
			 * @param y : Delta y for tilemaps.
			 */
			virtual void Use(int x = -1, int y = -1) = 0;
	};
}