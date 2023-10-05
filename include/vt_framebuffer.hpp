#pragma once
#include <vector>
#include <string>

namespace SDL2pp 
{
  class Window;
  class Renderer;
  class Exception;
}

namespace vt 
{

  // using namespace std;

  class FrameBuffer
  {
    private:
      class SDL_Pixel 
      {
        private:
         
          SDL2pp::Renderer* renderer;
          unsigned int      rowIndex;
          unsigned int      colIndex;
         
        public:

          SDL_Pixel() {};
          SDL_Pixel( SDL2pp::Renderer*, unsigned int, unsigned int ); 
          SDL_Pixel( SDL_Pixel&& ) noexcept;
          SDL_Pixel& operator= ( SDL_Pixel&& ) noexcept; 
          SDL_Pixel& operator= ( std::initializer_list<int> );

          SDL_Pixel( const SDL_Pixel& ) = delete;
          SDL_Pixel& operator= ( const SDL_Pixel& ) = delete;
          ~SDL_Pixel();
        /* public */
      }; // class SDL_Pixel
    
      SDL2pp::Window*   window;
      SDL2pp::Renderer* renderer;
      std::vector<SDL_Pixel> pixels;
      float             aspectRatio;
      unsigned int      width;
      unsigned int      height;
      unsigned int      pixelNumber;
      unsigned int      size; // in bytes

    public:

      FrameBuffer() {};
      FrameBuffer( const std::string, const unsigned int, const unsigned int );
      FrameBuffer& operator= ( FrameBuffer&& ) noexcept;

      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getPixelNum() const;
      unsigned int getSize() const;
      float getAspectRatio() const;

      SDL_Pixel& operator() ( const unsigned int, const unsigned int );
      SDL_Pixel& operator[] ( unsigned int );
     
      void update();
      bool requestedToExit();

      ~FrameBuffer(); 
  };
}
