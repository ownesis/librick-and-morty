#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "utils.h"
#include "api.h"
#include "episode.h"

void rm_episode_clear(rm_episode_t *episode) {
    if (!episode)
        return;

    FREE_IF_NONULL(episode->name);
    FREE_IF_NONULL(episode->url);
    FREE_IF_NONULL(episode->created);
    FREE_IF_NONULL(episode->air_date);
    FREE_IF_NONULL(episode->episode);
    
    if (episode->characters) {
        for (size_t i = 0; i < episode->characters_len; i++)
            free(episode->characters[i]);

        free(episode->characters);
    }

    free(episode);
}

rm_episode_t *parse_episode(json_object *jobj) {
    rm_episode_t *episode = NULL;

    if (!jobj)
        return NULL;

    episode = malloc(sizeof(rm_episode_t));

    if (!episode)
        return NULL;

    if (!memset(episode, 0, sizeof(rm_episode_t))) {
        free(episode);
        return NULL;
    }

    JSON_GET_OBJ_UINT(jobj, episode, uint64, id);

    JSON_GET_OBJ_STR(jobj, episode, name);
    JSON_GET_OBJ_STR(jobj, episode, air_date);
    JSON_GET_OBJ_STR(jobj, episode, episode);
    JSON_GET_OBJ_STR(jobj, episode, url);
    JSON_GET_OBJ_STR(jobj, episode, created);

    json_object *_tmp = NULL;
    json_object_object_get_ex(jobj, "characters", &_tmp);

    if (json_object_get_type(_tmp) == json_type_array) {
        episode->characters_len = json_object_array_length(_tmp);
        episode->characters = calloc(episode->characters_len, sizeof(char *));
        
        if (!episode->characters) {
            free(episode);   
            return NULL;
        }

        for (size_t i = 0; i < episode->characters_len; i++) {
            episode->characters[i] = strdup(json_object_get_string(json_object_array_get_idx(_tmp, i)));
        }
    }

    return episode;
}

char *episode_parse_filter(char *url, rm_episode_t *filter) {
    if (!filter)
        return NULL;
    
    APPEND_FILTER(filter, url, name);
    if (!url) return NULL;
    
    APPEND_FILTER(filter, url, episode);
    if (!url) return NULL;

    return url;
}

rm_episode_t *rm_episode_get(uint64_t uid) {
    return (rm_episode_t *)rm_entity_get(uid, RM_EPISODE);
}

rm_episode_arr_t *rm_episode_get_list(uint64_t array_id[], size_t array_len) {
    return (rm_episode_arr_t *)rm_entity_get_list(array_id, array_len, RM_EPISODE);
}

rm_episode_arr_t *rm_episode_get_page(rm_info_t *info, uint64_t page, rm_episode_t *filter) {
    return (rm_episode_arr_t *)rm_entity_get_page(info, page, (void *)filter, RM_EPISODE);
}

rm_episode_arr_t *rm_episode_get_all(rm_episode_t *filter) {
    return (rm_episode_arr_t *)rm_entity_get_all((void *)filter, RM_EPISODE);
}

void rm_episode_da_clear(rm_episode_arr_t *episode) {
    rm_entity_da_clear(((rm_episode_arr_t *)episode),  episode);
}
