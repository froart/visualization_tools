#include "../include/vt_framebuffer.hpp"
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <utility>
#include <iostream>
#include <vector>
#include <string>

namespace vt {

  using namespace std;

  struct FrameBufferInfo
  {
    SDL2pp::Window*   window;
    SDL2pp::Renderer* renderer;
    float             aspectRatio;
    unsigned int      width;
    unsigned int      height;
    unsigned int      pixelNumber;
    unsigned int      size; // in bytes
  };

  class FrameBuffer::Pixel
  {

    private:

      SDL2pp::Renderer* renderer;
      unsigned int      rowIndex;
      unsigned int      colIndex;
         
    public:

      Pixel() {}; /* required for vector class */
      Pixel( SDL2pp::Renderer*, unsigned int, unsigned int ); 
      inline Pixel& operator= ( std::initializer_list<int> );

      Pixel( Pixel&& ) noexcept;
      Pixel& operator= ( Pixel&& ) noexcept; 
      Pixel( const Pixel& ) = delete;
      Pixel& operator= ( const Pixel& ) = delete;
      ~Pixel();
    /* public */
  }; // class SDL_Pixel

  FrameBuffer::Pixel::Pixel( SDL2pp::Renderer* renderer, unsigned int rowIndex, unsigned int colIndex ) 
  {
    this->renderer = renderer;
    this->rowIndex = rowIndex;
    this->colIndex = colIndex;
  }; 

  FrameBuffer::Pixel::Pixel( FrameBuffer::Pixel&& other ) noexcept 
  {
    this->renderer = exchange( other.renderer, nullptr );
    this->colIndex = exchange( other.colIndex, 0 ); 
    this->rowIndex = exchange( other.rowIndex, 0 ); 
  }; 

  FrameBuffer::Pixel& FrameBuffer::Pixel::operator= ( FrameBuffer::Pixel&& other ) noexcept 
  {
    if( this != &other ) 
    {
      renderer = exchange( other.renderer, nullptr );
      rowIndex = exchange( other.rowIndex,       0 ); 
      colIndex = exchange( other.colIndex,       0 ); 
    }
     return *this;
   }; 

  inline FrameBuffer::Pixel& FrameBuffer::Pixel::operator= ( initializer_list<int> colorValues ) {
#ifndef NDEBUG
    if( colorValues.size() != 4 ) 
    {
      throw invalid_argument( "Initializer list must be exactly 4 elements to assign!" );
    }
#endif
    // Send color straight to the framebuffer 
    auto it = colorValues.begin();
    this->renderer->SetDrawColor( static_cast<Uint8>( *it     ),
                                  static_cast<Uint8>( *(it+1) ), 
                                  static_cast<Uint8>( *(it+2) ), 
                                  static_cast<Uint8>( *(it+3) )  );
    this->renderer->DrawPoint( this->colIndex , this->rowIndex );
    return *this; 
  };

  FrameBuffer::Pixel::~Pixel() {
    this->renderer = nullptr;
  };

  FrameBuffer::FrameBuffer( const string       name, 
                            const unsigned int width,
                            const unsigned int height )
  {
    this->pFrameBufferInfo              = new FrameBufferInfo;
    this->pFrameBufferInfo->pixelNumber = height * width;
    this->pFrameBufferInfo->size        = height * width * sizeof( Pixel );
    this->pFrameBufferInfo->aspectRatio = static_cast<float>( width ) 
                                          / static_cast<float>( height );
    this->pFrameBufferInfo->width       = width;
    this->pFrameBufferInfo->height      = height;
    try {
      this->pFrameBufferInfo->window   = new SDL2pp::Window( name,
                                                             SDL_WINDOWPOS_UNDEFINED, 
                                                             SDL_WINDOWPOS_UNDEFINED,
                                                             width, height, 
                                                             SDL_WINDOW_SHOWN );
      this->pFrameBufferInfo->renderer = new SDL2pp::Renderer( *this->pFrameBufferInfo->window, 
                                                               -1, 
                                                               SDL_RENDERER_ACCELERATED 
                                                               | SDL_RENDERER_PRESENTVSYNC );
    } 
    catch ( SDL2pp::Exception& e ) 
    {
      std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
      std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
    }

    this->pixels.resize( this->pFrameBufferInfo->pixelNumber );
    for( int j = 0; j < height; ++j )
      for( int i = 0; i < width; ++i )
         ( *this )( j, i ) = Pixel( this->pFrameBufferInfo->renderer, j, i );
  }

  FrameBuffer& FrameBuffer::operator= ( FrameBuffer&& other ) noexcept
  {
    if( this != &other ) 
    { 
      this->pixels                        = move( other.pixels );           
      this->pFrameBufferInfo->aspectRatio = exchange( other.pFrameBufferInfo->aspectRatio,     0.0 );
      this->pFrameBufferInfo->width       = exchange( other.pFrameBufferInfo->width,             0 );
      this->pFrameBufferInfo->height      = exchange( other.pFrameBufferInfo->height,            0 );
      this->pFrameBufferInfo->pixelNumber = exchange( other.pFrameBufferInfo->pixelNumber,       0 );
      this->pFrameBufferInfo->size        = exchange( other.pFrameBufferInfo->size,              0 );
      this->pFrameBufferInfo->window      = exchange( other.pFrameBufferInfo->window,      nullptr );
      this->pFrameBufferInfo->renderer    = exchange( other.pFrameBufferInfo->renderer,    nullptr );
    }
    return *this;
  }

  unsigned int FrameBuffer::getWidth() const 
  { 
    return this->pFrameBufferInfo->width;
  }
  unsigned int FrameBuffer::getHeight() const 
  { 
    return this->pFrameBufferInfo->height;
  }
  unsigned int FrameBuffer::getPixelNum() const 
  { 
    return this->pFrameBufferInfo->pixelNumber;
  }
  unsigned int FrameBuffer::getSize() const 
  { 
    return this->pFrameBufferInfo->size;
  }
  float FrameBuffer::getAspectRatio() const 
  { 
    return this->pFrameBufferInfo->aspectRatio;
  }

  inline FrameBuffer::Pixel& FrameBuffer::operator() ( const unsigned int rowIndex, 
                                                const unsigned int colIndex ) 
  {
    return this->pixels[colIndex + this->pFrameBufferInfo->width * rowIndex];
  };

  inline void FrameBuffer::update()
  {
    this->pFrameBufferInfo->renderer->Present();
  }
  
  inline bool FrameBuffer::requestedToExit()
  {
    SDL_Event event;
    SDL_PollEvent( &event );
    bool result;
    if ( event.type == SDL_QUIT )
      result = true;
    else if ( event.type == SDL_KEYDOWN )
      result = true;
    else if ( event.key.keysym.sym == SDLK_ESCAPE )
      result = true;
    return result;
  }

  FrameBuffer::~FrameBuffer()
  {
    delete this->pFrameBufferInfo->renderer;
    delete this->pFrameBufferInfo->window;
    delete this->pFrameBufferInfo;
  }
}
