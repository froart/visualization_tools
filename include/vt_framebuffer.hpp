#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

/*
typedef uint8_t  Uint8;
typedef uint32_t Uint32;
enum SDL_WindowFlags;

namespace SDL2pp {

  using namespace std;
 
  class Window {
    public:
      Window( const string&, unsigned int, unsigned int, const unsigned int&, const unsigned int&, SDL_WindowFlags );
  };
  class Renderer
  {
    public:
      Renderer( Window, int, Uint32 );
      int SetDrawColor( Uint8, Uint8, Uint8, Uint8 );
      int DrawPoint( int, int );
      int Present();
  };
  class Exception
  {
    public:
      string GetSDLFunction();
      string GetSDLError();
  };
}
*/

namespace vt 
{
  using namespace std;

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
          SDL_Pixel( SDL2pp::Renderer* renderer, int rowIndex, int colIndex ) 
          {
            this->renderer = renderer;
            this->rowIndex = rowIndex;
            this->colIndex = colIndex;
          }; 
          SDL_Pixel( SDL_Pixel&& other ) noexcept : renderer( exchange( other.renderer, nullptr ) ),
                                                    colIndex( exchange( other.colIndex, 0 ) ), 
                                                    rowIndex( exchange( other.rowIndex, 0 ) ) {}; 
          SDL_Pixel& operator= ( SDL_Pixel&& other ) noexcept 
          {
            if( this != &other ) 
            {
              renderer = exchange( other.renderer, nullptr );
              rowIndex = exchange( other.rowIndex, 0 ); 
              colIndex = exchange( other.colIndex, 0 ); 
            }
            return *this; 
          }; 
          SDL_Pixel& operator= ( initializer_list<int> );

          SDL_Pixel( const SDL_Pixel& ) = delete;
          SDL_Pixel& operator= ( SDL_Pixel& ) = delete;
          ~SDL_Pixel();
        /* public */
      }; // class SDL_Pixel
    
      SDL2pp::Window*   window;
      SDL2pp::Renderer* renderer;
      vector<SDL_Pixel> pixels;
      float             aspectRatio;
      unsigned int      width;
      unsigned int      height;
      unsigned int      pixelNumber;
      unsigned int      size; // in bytes

    public:

      FrameBuffer() {};
      FrameBuffer( const string, const unsigned int, const unsigned int );
      FrameBuffer& operator= ( FrameBuffer&& other ) noexcept
      {
        if( this != &other ) 
        { 
          this->pixels       = move( other.pixels );           
          this->aspectRatio  = exchange( other.aspectRatio,  0.0 );
          this->width        = exchange( other.width,          0 );
          this->height       = exchange( other.height,         0 );
          this->pixelNumber  = exchange( other.pixelNumber,    0 );
          this->size         = exchange( other.size,           0 );
          this->window       = exchange( other.window,   nullptr );
          this->renderer     = exchange( other.renderer, nullptr );
        }
        return *this;
      }

      unsigned int getWidth()    const { return this->width;        }
      unsigned int getHeight()   const { return this->height;       }
      unsigned int getPixelNum() const { return this->pixelNumber;  }
      unsigned int getSize()     const { return this->size;         }
      float getAspectRatio()     const { return this->aspectRatio;  }

      SDL_Pixel& operator() ( const unsigned int rowIndex, 
                              const unsigned int colIndex ) 
      {
        return this->pixels[colIndex + this->width * rowIndex];
      };
      SDL_Pixel& operator[] ( unsigned int index ) 
      { 
        return this->pixels[index]; 
      };
      
      void update();
      bool requestedToExit();

      ~FrameBuffer(); 
  };
}
