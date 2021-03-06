#pragma once

#include <memory>
#include "NetManager.h"

struct GamePiece;
struct Vector2f;
struct BonusBox;
struct Bullet;
struct Paddle;
struct Ball;
struct Tile;
struct Rect;
class TCPMessage;
class Logger;
enum class MessageTarget : int;
enum class GameState : int;
enum class TileType : int;
class MessageSender
{
public:
	MessageSender( NetManager &netMan );

	void SendBulletKilledMessage( uint32_t bulletID );
	void SendBulletFireMessage( const std::shared_ptr< Bullet > &bulletLeft, const std::shared_ptr< Bullet > &bulletRight, double height  );
	void SendBonusBoxSpawnedMessage( const std::shared_ptr< BonusBox > &bonusBox, double height );
	void SendBonusBoxPickupMessage( uint32_t boxID );
	void SendBallKilledMessage( uint32_t ballID );
	void SendBallSpawnMessage( const std::shared_ptr<Ball> &ball, double height );
	void SendBallDataMessage( const std::shared_ptr<Ball> &ball, double height );
	void SendBallRespawnMessage();
	void SendLevelDoneMessage( );
	void SendPaddlePosMessage( double xPos  );
	void SendPlayerName( const std::string &playerName );
	void SendGameSettingsMessage( const Vector2f &size, double scale );
	void SendGameStateChangedMessage( const GameState &gameState );

	void SendTileSpawnMessage( const std::shared_ptr<Tile> &tile, double height );
	void SendTileHitMessage( uint32_t tileID, bool tileKilled = false );
	void SendLastTileMessage( );

	void SendNewGameMessage( const std::string &ip, uint16_t port, const std::string &name );
	void SendJoinGameMessage( int32_t gameID );
	void SendEndGameMessage( int32_t gameID, const std::string &ip, uint16_t port );
	void SendGetGameListMessage();

	void SendLevelNameMessage( const std::string levelName );

private:
	void SendMessage( const TCPMessage &message, const MessageTarget &target, bool print = false );
	void PrintSend( const TCPMessage &msg );
	Vector2f FlipPosition( Rect originalPos, double height );

	NetManager &netManager;
	Logger *logger;
};
