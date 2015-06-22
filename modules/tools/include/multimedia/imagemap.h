#ifndef __IMAGEMAP_H__
#define __IMAGEMAP_H__

#include <cassert>

//image size limits 10M x 10M
#define TOOLS_IMAGE_MAX_WIDTH 19701760
#define TOOLS_IMAGE_MAX_HEIGHT 19701760
#define TOOLS_IMAGE_MAX_CHANNELS 4

namespace tools
{
   //////////////////////////////////////////////
   /// Bidimensional array of pixels
   //////////////////////////////////////////////
   class imageMap
   {
     
     public:

      typedef unsigned char pixel;

      imageMap();
      imageMap(const unsigned &width, const unsigned &height, const unsigned &channels);
      ~imageMap();


      void reset(const unsigned &width, const unsigned &height, const unsigned &channels);

      inline unsigned width() const { return _width; }
      inline unsigned height() const { return _height; }
      inline unsigned channels() const { return _channels; }
      inline unsigned linearSize() const { return _width*_height*_channels; }
      inline pixel *data() const { return _data; }

      inline pixel operator [](const unsigned &i) const 
      { assert(i < linearSize()); return _data[i]; }
      inline pixel &operator [](const unsigned &i) 
      { assert(i < linearSize()); return *(&(_data[i])); }


      void flipVertical();

     protected:
      
      unsigned _width;
      unsigned _height;
      unsigned _channels;
      
      pixel *_data;

     private:

      imageMap(const imageMap &image);
      imageMap &operator =(const imageMap &image);

   };

}

#endif //__IMAGEMAP_H__
