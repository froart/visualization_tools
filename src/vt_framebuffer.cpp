#include "../include/vt_framebuffer.hpp"

namespace vt {

  using namespace std;

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
