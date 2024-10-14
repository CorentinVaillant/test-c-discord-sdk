#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "discord_game_sdk.h"
#include "discord_util.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <string.h>
#endif




///! Temp func, do nothing
void UpdateActivityCallback(void* data, enum EDiscordResult result)
{
    DISCORD_REQUIRE(result);
}

int RelationshipPassFilter(void* data, struct DiscordRelationship* relationship)
{
    return (relationship->type == DiscordRelationshipType_Friend);
}

int RelationshipSnowflakeFilter(void* data, struct DiscordRelationship* relationship)
{
    struct Application* app = (struct Application*)data;

    return (relationship->type == DiscordRelationshipType_Friend &&
            relationship->user.id < app->user_id);
}

void OnRelationshipsRefresh(void* data)
{
    struct Application* app = (struct Application*)data;
    struct IDiscordRelationshipManager* module = app->relationships;

    module->filter(module, app, RelationshipPassFilter);

    int32_t unfiltered_count = 0;
    DISCORD_REQUIRE(module->count(module, &unfiltered_count));

    module->filter(module, app, RelationshipSnowflakeFilter);

    int32_t filtered_count = 0;
    DISCORD_REQUIRE(module->count(module, &filtered_count));

    printf("=== Cool Friends ===\n");
    for (int32_t i = 0; i < filtered_count; i += 1) {
        struct DiscordRelationship relationship;
        DISCORD_REQUIRE(module->get_at(module, i, &relationship));

        printf("%lld %s#%s\n",
               relationship.user.id,
               relationship.user.username,
               relationship.user.discriminator);
    }
    printf("(%d friends less cool than you omitted)\n", unfiltered_count - filtered_count);

    struct DiscordActivity activity;
    memset(&activity, 0, sizeof(activity));
    sprintf(activity.details, "Cooler than %d friends", unfiltered_count - filtered_count);
    sprintf(activity.state, "%d friends total", unfiltered_count);

    app->activities->update_activity(app->activities, &activity, app, UpdateActivityCallback);
}

void OnUserUpdated(void* data)
{
    struct Application* app = (struct Application*)data;
    struct DiscordUser user;
    app->users->get_current_user(app->users, &user);
    app->user_id = user.id;
}

void OnOAuth2Token(void* data, enum EDiscordResult result, struct DiscordOAuth2Token* token)
{
    if (result == DiscordResult_Ok) {
        printf("OAuth2 token: %s\n", token->access_token);
    }
    else {
        printf("GetOAuth2Token failed with %d\n", (int)result);
    }
}

void OnLobbyConnect(void* data, enum EDiscordResult result, struct DiscordLobby* lobby)
{
    printf("LobbyConnect returned %d\n", (int)result);
}