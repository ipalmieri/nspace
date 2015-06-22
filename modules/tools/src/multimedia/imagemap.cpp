#include "multimedia/imagemap.h"
#include <stdlib.h>
#include <stdexcept>
#include <string.h>


using namespace std;
using namespace tools;

imageMap::imageMap(const unsigned &width, const unsigned &height, const unsigned &channels) 
{
   _width = 0;
   _height = 0;
   _channels = 0;
   _data = (pixel *) NULL;

   reset(width, height, channels);
}

imageMap::imageMap()
{
   _width = 0;
   _height = 0;
   _channels = 0;
   _data = (pixel *) NULL;
}

imageMap::~imageMap()
{
   if (_data) free(_data);
}

void imageMap::flipVertical()
{
   size_t block_size = (size_t) sizeof(pixel) * _width * _channels;
   
   pixel *temp = (pixel *) malloc(block_size);
 
   if (!temp) throw runtime_error("Can't malloc() from imageMap.flipVertical."); 
   

   for (unsigned i = 0; i < _height/2; i++)
   {
      unsigned src_row = i * block_size;
      unsigned dst_row = (_height - 1 - i) * block_size;

      memcpy(temp, &_data[src_row], block_size);

      memcpy(&_data[src_row], &_data[dst_row], block_size);
      
      memcpy(&_data[dst_row], temp, block_size);
   }

   free(temp);
}

void imageMap::reset(const unsigned &width, const unsigned &height, const unsigned &channels)
{
   bool checkWidth = (width >= 0 && width <= TOOLS_IMAGE_MAX_WIDTH);
   bool checkHeight = (height >= 0 && height <= TOOLS_IMAGE_MAX_HEIGHT);
   bool checkChannels = (channels >= 0 && channels <= TOOLS_IMAGE_MAX_CHANNELS);

   if (checkWidth && checkHeight && checkChannels)
   {
      _width = width;
      _height = height;
      _channels = channels;
      
      size_t image_size = _width*_height*_channels;

      if (_data) 
      { 
	 free(_data);
	 _data = (pixel *) NULL;
      }

      if (image_size > 0)
      {
	 _data = (pixel *) malloc(sizeof(pixel) * image_size);

	 if (!_data)
	 {
	    throw runtime_error("Can't malloc() from imageMap."); 
	 }
      }
   }
   else
   {
      throw runtime_error("imageMap size is greater than allowed limits.");
   }
}
