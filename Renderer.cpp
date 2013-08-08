#include "Renderer.h"

#include "Ball.h"
#include "Paddle.h"

Renderer::Renderer()
	:	SCREEN_WIDTH ( 1920 / 2 )
	,	SCREEN_HEIGHT( 1280 / 2 )
	,	SCREEN_BPP ( 32 )

	,	rects()
	,	textures()

	,	backgroundArea(  NULL )
	,	backgroundImage(  NULL )
	,	screen(  NULL )
	,	ballList( /*{ }*/ )
	,	font()
	,	bigFont()
	,	text()
	,	lives()
	,	points()
	,	textColor( { 0, 0, 0, 0 } )
{

}

Renderer::~Renderer()
{
	ballList.empty();

	SDL_FreeSurface( backgroundArea );
	SDL_FreeSurface( backgroundImage );
	SDL_FreeSurface (screen );

	SDL_FreeSurface( text );
	SDL_FreeSurface( lives );
	SDL_FreeSurface( points );

	SDL_Quit();
}

bool Renderer::Init()
{
	// Set up screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF );

	if ( screen == NULL )
		return false;

	if ( TTF_Init( ) == -1 )
		return false;

	// Set window title
	if ( !LoadAllFiles() )
		return false;

	BlitBackground();

	return true;
}

void Renderer::AddBall( const std::shared_ptr< Ball > &ball )
{
	std::cout << "Ball added\n";

	ballList.push_back( ball );
}

void Renderer::RemoveBall(  const std::shared_ptr< Ball > &ball )
{
	for ( auto p = ballList.begin(); p != ballList.end();)
	{
		if ( (*p).get() == ball.get() )
		{
			std::cout << "Ball removed\n";
			(*p).reset();
			ballList.erase( p );
			break;
		} else
			++p;
	}
}

SDL_Surface* Renderer::LoadImage( const std::string &filename, GamePiece::TextureType textureType )
{
	// Temporary stoare for the iamge that's loaded
	SDL_Surface* loadedImage = NULL;

	// The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	// Load the image
	loadedImage = IMG_Load( filename.c_str() );

	// If the image loaded
	if ( loadedImage != NULL )
	{
		//
		// Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		textures[ textureType ] = optimizedImage;
		rects[ textureType ] = optimizedImage->clip_rect;

		// Free the old image
		SDL_FreeSurface( loadedImage );
	}

	// Return the optimized image
	return optimizedImage;
}

void Renderer::SetColorKey( SDL_Surface* source, int r, int g, int b )
{
	if ( source )
	{
		Uint32 colorKey = SDL_MapRGB( source->format, r, g, b );

		SDL_SetColorKey( source, SDL_SRCCOLORKEY, colorKey );
	}
}

void Renderer::SetColorKey( GamePiece::TextureType textureID, int r, int g, int b )
{
	SetColorKey( textures[ textureID ], r, g, b );
}

void Renderer::FillSurface( SDL_Surface* source, int r, int g, int b )
{
	SDL_FillRect( source, NULL, SDL_MapRGBA( source->format, r, g, b, 0 ) );
}

void Renderer::ApplySurface( short x, short y, SDL_Surface* source, SDL_Surface* destination ) const
{
	// Make a temp rect to hold the offsets
	SDL_Rect offset;

	// Give the offsets of the rectangle
	offset.x = x;
	offset.y = y;

	// Blit the surface
	SDL_BlitSurface( source, NULL, destination, &offset );
}
void Renderer::ApplySurface( const SDL_Rect &r, SDL_Surface* source, SDL_Surface* destination ) const
{

	ApplySurface( r.x, r.y, source, destination );

}
bool Renderer::LoadAllFiles( )
{
	LoadImage( "media/paddles/paddle30x120.png", GamePiece::Paddle );
	SetColorKey( GamePiece::Paddle, 0xff,0xff,0xff );

	LoadImage( "media/ball.png", GamePiece::Ball );
	SetColorKey( GamePiece::Ball, 0xff,0xff,0xff );

	backgroundImage= LoadImage( "media/background.png", GamePiece::Background );

	backgroundArea = SDL_CreateRGBSurface( 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0x00ff0000, 0, 0, 0);

	font = TTF_OpenFont( "lazy.ttf", 28 );

	if ( font == NULL )
		return false;

	bigFont = TTF_OpenFont( "lazy.ttf", 57 );

	if ( bigFont == NULL )
		return false;

	return true;
}

bool Renderer::Render( )
{
	BlitBackground();
	BlitText();
	BlitForeground();

	ApplySurface( 0, 0, backgroundArea, screen );

	if ( SDL_Flip( screen ) == -1 )
		return false;
	else
		return true;
}

void Renderer::BlitBackground() const
{
	ApplySurface( 0, 0, backgroundImage, backgroundArea );
}

void Renderer::BlitForeground()
{
	// Draw balls
	for ( std::shared_ptr< Ball > gp : ballList )
	{
		ApplySurface( gp->rect.x, gp->rect.y, textures[GamePiece::Ball], backgroundArea );
	}

	// Draw paddles
	ApplySurface( localPaddle->rect.x, localPaddle->rect.y, textures[GamePiece::Paddle], backgroundArea );
	
}

void Renderer::BlitText()
{
	if ( lives )
		ApplySurface( 0, 0, lives, backgroundArea );

	if ( points )
		ApplySurface( 0, static_cast< short > ( lives->h + 5 ), points, backgroundArea );

	SDL_Rect screenSize = GetWindowSize();
	if ( text )
		ApplySurface(  ( screenSize.w / 2 ) - ( text->clip_rect.w / 2 ), screenSize.h / 2, text, backgroundArea );
}