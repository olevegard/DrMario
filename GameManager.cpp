#include "GameManager.h"
#include <vector>
#include <algorithm>

	GameManager::GameManager()
	:	renderer()
	,	timer()
		,	localPaddle()
		 //,	paddleList()
	,	ballList()
	,	tileSize()
		,	windowSize()
{
}

int GameManager::Init()
{

	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
		return 1;

	if ( !renderer.Init() )
		return 1;

	tileSize = renderer.GetTileSize();
	windowSize = renderer.GetWindowSize();

	std::cout << "tile size " << tileSize.x << " , " << tileSize.y << " , " << tileSize.w << " , " << tileSize.h << std::endl;
	std::cout << "window size " << windowSize.x << " , " << windowSize.y << " , " << windowSize.w << " , " << windowSize.h << std::endl;

	Restart();

	return 0;
}

void GameManager::Restart()
{
	//std::shared_ptr< GamePiece > paddle( new Paddle() );
	localPaddle.reset( new Paddle() );
	localPaddle->textureType = GamePiece::Paddle;
	localPaddle->rect.y = 610;
	localPaddle->rect.w = 120;
	localPaddle->rect.h = 30;
	renderer.SetLocalPaddle( localPaddle );
	//renderer.AddObject( paddle );
	//
	//localPaddle = paddle.get();
	//paddleList.push_back( dynamic_cast< Paddle* > ( paddle.get() ) );

	AddBall();
	AddTile( 340, 120 );
	AddTile( 405, 120 );
	AddTile( 470, 120 );
	AddTile( 340, 145 );
	AddTile( 405, 145 );
	AddTile( 470, 145 );

	renderer.RenderLives( 1 );
	renderer.RenderPoints( 123 );
	renderer.RenderText( "Press enter to start");
}

void GameManager::AddBall()
{
	std::shared_ptr< Ball > ball( new Ball() );
	ball->textureType = GamePiece::Ball;
	ball->SetOwner( 0 );

	ballList.push_back( ball );
	renderer.AddBall( ball );
}

void GameManager::RemoveBall( const std::shared_ptr< Ball >  pBall )
{

	auto p = std::find( ballList.begin(), ballList.end(), pBall );
	if ( p != ballList.end() )
	{
		//ballList.erase( p );
		renderer.RemoveBall( (*p) );
	}
	//std::cout << "No find\n";
	/*
	   for ( auto p = ballList.begin(); p != ballList.end(); ) 
	   {
	   if ( ( *p ).get() == pBall )
	   {
	   renderer.RemoveObject( (*p ) );
	   p =  gameObjectList.erase( p );
	   (*p).reset();
	   }
	   else
	   ++p;
	   }
	   */
}

void GameManager::AddTile( short posX, short posY )
{
	std::shared_ptr< Tile > tile( new Tile() );
	tile->textureType = GamePiece::Tile;
	tile->rect.x = posX;
	tile->rect.y = posY;
	tile->rect.w = 60;
	//tile->rect.w = 120;
	//tile->rect.h = 40;
	tile->rect.h = 20;

	tileList.push_back( tile );

	renderer.AddTile( tile );

}

void GameManager::RemoveTile( std::shared_ptr< Ball > pTile )
{

}
void GameManager::UpdateBalls( double delta )
{
	renderer.RenderLives( ballList.size() );

	if ( ballList.size() > 0 )
	{
		renderer.RemoveText();

		for ( std::vector< std::shared_ptr< Ball > >::iterator p = ballList.begin(); p != ballList.end() && (*p) != nullptr;  )
		{
			(*p)->Update( delta );
			(*p)->BoundCheck( windowSize );
			(*p)->PaddleCheck( localPaddle->rect );
			(*p)->TileCheck( tileList[0]->rect );
			if ( (*p)->DeathCheck( windowSize ) )
			{ (*p)->rect.x = 200;
				(*p)->rect.y = 20;
				RemoveBall( (*p) );
				p = ballList.erase( p );
			}
			++p;
		}
	} else 
		renderer.RenderText( "Press enter to start");
}



void GameManager::Run()
{
	bool quit = false;
	SDL_Event event;

	int halfTileWidth = tileSize.w / 2;

	while ( !quit )
	{
		while ( SDL_PollEvent( &event ) )
		{
			if ( event.type == SDL_QUIT )
				quit = true;

			if ( event.type == SDL_KEYDOWN )
			{
				switch  ( event.key.keysym.sym )
				{
					case SDLK_RETURN:
					case SDLK_b:
						AddBall();
						break;
					case SDLK_q:
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			}

			if ( event.motion.x != 0 && event.motion.y != 0 )
				localPaddle->rect.x = static_cast< short > ( event.motion.x - halfTileWidth );

			if ( ( localPaddle->rect.x + tileSize.w) > windowSize.w )
				localPaddle->rect.x = static_cast< short > ( windowSize.w - tileSize.w );

			if ( localPaddle->rect.x  <= 0  )
				localPaddle->rect.x = 0;

			
		}
		double delta = timer.GetDelta( );
		UpdateBalls( delta );

		renderer.Render( );
	}

}
void GameManager::CheckBallTileIntersection()
{
	for ( std::shared_ptr< Ball > gp : ballList )
	{
		
	}
}
