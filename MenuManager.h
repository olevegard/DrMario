#pragma once

#include <map>
#include <memory>
#include <iostream>

#include "GameInfo.h"

#include "enums/PlussMin.h"
#include "enums/GameState.h"
#include "enums/LobbyState.h"
#include "enums/LobbyMenuItem.h"
#include "enums/ConfigValueType.h"
#include "enums/MainMenuItemType.h"
#include "enums/PauseMenuItemType.h"

//#include "structs/menu_items/ConfigChange.h"
#include "structs/menu_items/ConfigItem.h"
#include "structs/menu_items/ConfigList.h"

#include "ConfigLoader.h"

class Logger;
struct MenuList;
struct MenuItem;
class MenuManager
{
public:
	MenuManager( ConfigLoader &configLoader_ );

	void CheckItemMouseOver( int x, int y );
	void CheckItemMouseOver_Pause( int x, int y );
	void CheckItemMouseOver_MainMenu( int x, int y );
	void CheckItemMouseOver_Lobby( int x, int y );
	void CheckItemMouseOver_Options( int x, int y );

	void CheckItemMouseClick( int x, int y);

	GameState GetGameState() const;
	void SetGameState( const GameState &gs );
	bool IsGameStateChangeValid( const GameState &gs) const;

	GameState GoBackToPreviousMenuState();
	GameState GoToMenu();
	LobbyMenuItem GetLobbyState();

	GameState GetPrevGameState() const;
	bool HasGameStateChanged();
	bool HasLobbyStateChanged();

	bool WasGameStarted() const;
	bool WasGameQuited() const;
	bool WasGameResumed() const;

	bool IsTwoPlayerMode() const;
	bool IsInAMenu() const;

	int32_t GetSelectedGame() const;
	bool IsAnItemSelected() const;
	GameInfo GetSelectedGameInfo() const;

	void SetMainMenuItem( const MainMenuItemType &type, const std::shared_ptr< MenuItem >& button );
	void SetLobbyMenuItem( const LobbyMenuItem &type, const std::shared_ptr< MenuItem >  &button );
	void SetPauseMenuItem( const PauseMenuItemType &type, const std::shared_ptr< MenuItem >  &button );
	void SetGameList( const std::shared_ptr< MenuList >  gameList_ );
	void SetGameList( const std::shared_ptr< ConfigList >  configList_ );
	void SetOptionsMenuItem( const std::shared_ptr< MenuItem >  &button );
	void SetConfigList( const std::shared_ptr < ConfigList > &configList_ );

private:
	void InitConfigList();

	MainMenuItemType CheckIntersections( int x, int y );
	PauseMenuItemType CheckIntersections_Pause( int x, int y );
	LobbyMenuItem CheckIntersections_Lobby( int x, int y );

	PlussMin CheckConfigItemsClick( int32_t x, int32_t y, const std::shared_ptr< ConfigItem > &item );
	void CheckOptionsIntersections( int32_t x, int32_t y );
	void CheckMenuItemIntersections( int32_t x, int32_t y );

	GameState currentGameState;
	GameState prevGameState;
	bool hasGameStateChanged;
	bool hasLobbyStateChanged;
	bool isTwoPlayerMode;

	// Main menu
	std::map< MainMenuItemType, std::shared_ptr< MenuItem > > mainMenuItems;

	// Pause
	std::map< PauseMenuItemType, std::shared_ptr< MenuItem > > pauseMenuItems;

	// Lobby
	std::map< LobbyMenuItem, std::shared_ptr< MenuItem > > lobbyMenuItems;
	std::shared_ptr <MenuList > lobbyGameList;

	// Options
	std::shared_ptr < ConfigList > configList;
	std::shared_ptr< MenuItem > backToMenuButton;

	int32_t selectedGameID;
	LobbyMenuItem lobbyState;

	Logger* logger;
	ConfigLoader &configLoader;
};
