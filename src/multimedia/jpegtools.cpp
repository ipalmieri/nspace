
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
}

#include "multimedia.h"
#include "logger.h"

using namespace tools;

//http://mamedev.org/source/src/lib/libjpeg/libjpeg.txt

void tools::writeJPEG(const char *filename, const imageMap &img)
{
    struct jpeg_compress_struct cinfo;
    unsigned char *data_ptr;
    //JSAMPROW row_pointer;
    struct jpeg_error_mgr jerr; 
    unsigned int i;
    FILE *fp;

    //if (img.channels != 1 && img.channels != 3) return;

    fp = fopen(filename, "w+");
    
    if (!fp)
    {
       logger::logMsg("writeJPEG: Can't open file " + std::string(filename), LGR_ERROR);
       return;
    }

    
        
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = img.width();
    cinfo.image_height = img.height();
    cinfo.input_components = img.channels();
    cinfo.in_color_space = (img.channels() == 3)? JCS_RGB : JCS_GRAYSCALE;
    cinfo.err = jpeg_std_error(&jerr);

    //fix this - whats the default? comment it
    jpeg_set_quality(&cinfo, 100, TRUE);

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);

    for (i = 0; i < img.height(); i++) {

       data_ptr = (imageMap::pixel *) &img.data()[i * (img.width() * img.channels())];

       //row_pointer = (JSAMPROW) data_ptr;
       jpeg_write_scanlines(&cinfo, &data_ptr, 1);
    }
    
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(fp);

}


void tools::readJPEG(const char *filename, imageMap &img)
{
   struct jpeg_decompress_struct cinfo;
   struct jpeg_error_mgr jerr;

   FILE *fp;
   int i;
   //JSAMPARRAY pRows;

   fp = fopen(filename, "rb");
   
   if (!fp)
   {
       logger::logMsg("readJPEG: Can't open file " + std::string(filename), LGR_ERROR);
       return;
   }

   cinfo.err = jpeg_std_error(&jerr);

   jpeg_create_decompress(&cinfo);

   jpeg_stdio_src(&cinfo, fp);

   jpeg_read_header(&cinfo, TRUE);

   //cinfo.out_color_space = JCS_RGB;

   jpeg_start_decompress(&cinfo);   

   
   img.reset(cinfo.output_width, cinfo.output_height, cinfo.output_components);


   if (!img.data())
   {
      jpeg_destroy_decompress(&cinfo);
      fclose(fp);
      return;
   }


   i = 0;
   while (cinfo.output_scanline < cinfo.output_height)
   {
      imageMap::pixel *bptr = &img[cinfo.output_width * cinfo.output_components * i];
          
      jpeg_read_scanlines(&cinfo, (JSAMPARRAY) &bptr, 1); // decode
     
      i++;
   }


   jpeg_finish_decompress(&cinfo);

   jpeg_destroy_decompress(&cinfo);
   
   fclose(fp);

}
