#pragma once

#include <iostream>
#include <vector>
#include <SDL2pp/SDL2pp.hh>

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
      unsigned int      screenWidth;
      unsigned int      screenHeight;
      unsigned int      pixelNumber;
      unsigned int      screenSize; // in bytes

    public:

      FrameBuffer() {};
      FrameBuffer( const string, const unsigned int, const unsigned int );
      FrameBuffer& operator= ( SDL_Screen&& other ) noexcept
      {
        if( this != &other ) 
        { 
          this->pixels       = move( other.pixels );           
          this->aspectRatio  = exchange( other.aspectRatio,  0.0 );
          this->screenWidth  = exchange( other.screenWidth,  0 );
          this->screenHeight = exchange( other.screenHeight, 0 );
          this->pixelNumber  = exchange( other.pixelNumber,  0 );
          this->screenSize   = exchange( other.screenSize,   0 );
          this->window       = exchange( other.window,       nullptr );
          this->renderer     = exchange( other.renderer,     nullptr );
        }
        return *this;
      }

      unsigned int getWidth()    const { return this->screenWidth;  }
      unsigned int getHeight()   const { return this->screenHeight; }
      unsigned int getPixelNum() const { return this->pixelNumber;  }
      unsigned int getSize()     const { return this->screenSize;   }
      float getAspectRatio()     const { return this->aspectRatio;  }

      SDL_Pixel& operator() ( const unsigned int rowIndex, const unsigned int colIndex ) 
      {
        return this->pixels[colIndex + this->screenWidth * rowIndex];
      };
      SDL_Pixel& operator[] ( unsigned int index ) 
      { 
        return this->pixels[index]; 
      };
      
      void update();

      ~SDL_Screen(); 

  }
}
