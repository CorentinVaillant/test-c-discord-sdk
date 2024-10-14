#include "discord_game_sdk.h"

#define DISCORD_REQUIRE(x) assert(x == DiscordResult_Ok)

/// @brief 
///Abstraction of the application
struct Application {
    struct IDiscordCore* core;
    struct IDiscordUserManager* users;
    struct IDiscordAchievementManager* achievements;
    struct IDiscordActivityManager* activities;
    struct IDiscordRelationshipManager* relationships;
    struct IDiscordApplicationManager* application;
    struct IDiscordLobbyManager* lobbies;
    DiscordUserId user_id;
};


