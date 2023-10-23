#pragma once

#include <vector>
#include <string>

namespace vt 
{

  class FrameBuffer
  {

    private:

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

      /* bind value array */
      void bind( std::vector<float>* image );
      /* update window */
      void update();
      /* check if the exit button was pressed */
      bool requestedToExit();

      ~FrameBuffer(); 
  };
}
