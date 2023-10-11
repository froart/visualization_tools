#pragma once

#include <vector>
#include <string>

namespace vt 
{

  class FrameBuffer
  {

    private:

      class Pixel;
      std::vector<Pixel> pixels;
      struct FrameBufferInfo* pFrameBufferInfo;

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
      inline Pixel& operator() ( const unsigned int, const unsigned int );
     
      /* update window */
      inline void update();
      /* check if the exit button was pressed */
      inline bool requestedToExit();

      ~FrameBuffer(); 
  };
}
