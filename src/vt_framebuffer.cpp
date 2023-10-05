#include "../include/vt_framebuffer.hpp"
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <utility>
#include <iostream>
#include <vector>
#include <string>

namespace vt {

  using namespace std;

  FrameBuffer::SDL_Pixel::SDL_Pixel( SDL2pp::Renderer* renderer, unsigned int rowIndex, unsigned int colIndex ) 
  {
    this->renderer = renderer;
    this->rowIndex = rowIndex;
    this->colIndex = colIndex;
  }; 

  FrameBuffer::SDL_Pixel::SDL_Pixel( FrameBuffer::SDL_Pixel&& other ) noexcept 
  {
    this->renderer = exchange( other.renderer, nullptr );
    this->colIndex = exchange( other.colIndex, 0 ); 
    this->rowIndex = exchange( other.rowIndex, 0 ); 
  }; 

  FrameBuffer::SDL_Pixel& FrameBuffer::SDL_Pixel::operator= ( FrameBuffer::SDL_Pixel&& other ) noexcept 
  {
    if( this != &other ) 
    {
      renderer = exchange( other.renderer, nullptr );
      rowIndex = exchange( other.rowIndex, 0 ); 
      colIndex = exchange( other.colIndex, 0 ); 
     }
     return *this; 
   }; 

  FrameBuffer::SDL_Pixel& FrameBuffer::SDL_Pixel::operator= ( initializer_list<int> colorValues ) {
    // TODO: check the size of passed initializer list at compile time, not runtime
    if( colorValues.size() != 4 ) 
    {
      throw invalid_argument( "Initializer list must be exactly 4 elements to assign!" );
    }
    // Send color straight to the screen 
    auto it = colorValues.begin();
    this->renderer->SetDrawColor( static_cast<Uint8>( *it     ),
                                  static_cast<Uint8>( *(it+1) ), 
                                  static_cast<Uint8>( *(it+2) ), 
                                  static_cast<Uint8>( *(it+3) )  );
    this->renderer->DrawPoint( this->colIndex , this->rowIndex );
    return *this; 
  };

  FrameBuffer::SDL_Pixel::~SDL_Pixel() {
    this->renderer = nullptr;
    this->rowIndex = 0;
    this->colIndex = 0;
  };

  FrameBuffer::FrameBuffer( const string       name, 
                            const unsigned int width,
                            const unsigned int height )
  {
    this->pixelNumber = height * width;
    this->size        = height * width * sizeof( SDL_Pixel );
    this->aspectRatio = ( static_cast<float>( width ) / static_cast<float>  ( height ) );
    this->width       = width;
    this->height      = height;
    try {
      this->window   = new SDL2pp::Window( name,
                                           SDL_WINDOWPOS_UNDEFINED, 
                                           SDL_WINDOWPOS_UNDEFINED,
                                           width, height, 
                                           SDL_WINDOW_SHOWN );
      this->renderer = new SDL2pp::Renderer( *this->window, 
                                             -1, 
                                             SDL_RENDERER_ACCELERATED 
                                             | SDL_RENDERER_PRESENTVSYNC );
    } 
    catch ( SDL2pp::Exception& e ) 
    {
      std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
      std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
    }

    this->pixels.resize( this->pixelNumber );
    for( int j = 0; j < height; ++j )
      for( int i = 0; i < width; ++i )
         ( *this )( j, i )= SDL_Pixel(this->renderer, j, i);
  }

  FrameBuffer& FrameBuffer::operator= ( FrameBuffer&& other ) noexcept
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

  unsigned int FrameBuffer::getWidth() const 
  { 
    return this->width;
  }
  unsigned int FrameBuffer::getHeight() const 
  { 
    return this->height;
  }
  unsigned int FrameBuffer::getPixelNum() const 
  { 
    return this->pixelNumber;
  }
  unsigned int FrameBuffer::getSize() const 
  { 
    return this->size;
  }
  float FrameBuffer::getAspectRatio() const 
  { 
    return this->aspectRatio;
  }

  FrameBuffer::SDL_Pixel& FrameBuffer::operator() ( const unsigned int rowIndex, 
                                       const unsigned int colIndex ) 
  {
    return this->pixels[colIndex + this->width * rowIndex];
  };
  FrameBuffer::SDL_Pixel& FrameBuffer::operator[] ( unsigned int index ) 
  { 
    return this->pixels[index]; 
  };

  void FrameBuffer::update()
  {
    this->renderer->Present();
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
    delete this->renderer;
    delete this->window;
  }
}
