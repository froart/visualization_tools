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
   
    vector<float>*    boundImage;
    SDL2pp::Window*   window;
    SDL2pp::Renderer* renderer;
    float             aspectRatio;
    unsigned int      width;
    unsigned int      height;
    unsigned int      pixelNumber;
    unsigned int      size; // in bytes
  
  };

  FrameBuffer::FrameBuffer( const string       name, 
                            const unsigned int width,
                            const unsigned int height )
  {
    this->pFrameBufferInfo              = new FrameBufferInfo;
    this->pFrameBufferInfo->pixelNumber = height * width;
    // this->pFrameBufferInfo->size        = height * width * sizeof( Pixel );
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

  }

  FrameBuffer& FrameBuffer::operator= ( FrameBuffer&& other ) noexcept
  {
    if( this != &other ) 
    { 
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

  void FrameBuffer::bind( vector<float>* image ) 
  {
    this->pFrameBufferInfo->boundImage = image;
  }

  void FrameBuffer::update()
  {
    for( int j = 0 ; j < this->pFrameBufferInfo->height ; ++j )
      for( int i = 0 ; i < this->pFrameBufferInfo->width ; ++i )
      {
        unsigned int idx = i + j * this->pFrameBufferInfo->width;
        this->pFrameBufferInfo->renderer->SetDrawColor( 
            static_cast<Uint8>( ( *this->pFrameBufferInfo->boundImage )[idx] * 255 ),
            static_cast<Uint8>( ( *this->pFrameBufferInfo->boundImage )[idx] * 255 ),
            static_cast<Uint8>( ( *this->pFrameBufferInfo->boundImage )[idx] * 255 ),
                                                                          255 );
        this->pFrameBufferInfo->renderer->DrawPoint( i , j );
      }
    this->pFrameBufferInfo->renderer->Present();
  }
  
  bool FrameBuffer::requestedToExit()
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
