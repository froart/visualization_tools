#pragma once

#include <vector>
#include <string>

namespace vt 
{

  class FrameBuffer
  {

    private:

      class Pixel;
      struct WindowInfo* pWindowInfo;
      std::vector<Pixel> pixels;
      float              aspectRatio;
      unsigned int       width;
      unsigned int       height;
      unsigned int       pixelNumber;
      unsigned int       size; // in bytes

    public:

      FrameBuffer() {};
      FrameBuffer( const std::string, const unsigned int, const unsigned int );
      FrameBuffer& operator= ( FrameBuffer&& ) noexcept;

      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getPixelNum() const;
      unsigned int getSize() const;
      float getAspectRatio() const;

      Pixel& operator() ( const unsigned int, const unsigned int );
      Pixel& operator[] ( unsigned int );
     
      void update();
      bool requestedToExit();

      ~FrameBuffer(); 
  };
}
