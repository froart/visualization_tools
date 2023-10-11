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

      /* constructor */
      FrameBuffer( const std::string, const unsigned int, const unsigned int );
      /* move constuctor */
      FrameBuffer& operator= ( FrameBuffer&& ) noexcept;

      /* Window information */
      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getPixelNum() const;
      unsigned int getSize() const;
      float getAspectRatio() const;

      /* access operator */
      Pixel& operator() ( const unsigned int, const unsigned int );
     
      /* update window */
      void update();
      /* check if the exit button was pressed */
      bool requestedToExit();

      ~FrameBuffer(); 
  };
}
