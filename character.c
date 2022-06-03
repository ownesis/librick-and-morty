#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "utils.h"
#include "api.h"
#include "character.h"

void rm_character_clear(rm_character_t *character) {
    if (!character)
        return;

    NULLED_FREE(character->name);
    NULLED_FREE(character->status);
    NULLED_FREE(character->species);
    NULLED_FREE(character->type);
    NULLED_FREE(character->gender);
    NULLED_FREE(character->image);
    NULLED_FREE(character->url);
    NULLED_FREE(character->created);
    NULLED_FREE(character->location.name);
    NULLED_FREE(character->location.url);
    NULLED_FREE(character->origin.name);
    NULLED_FREE(character->origin.url);
    
    if (character->episode) {
        for (size_t i = 0; i < character->episode_len; i++)
            NULLED_FREE(character->episode[i]);

        NULLED_FREE(character->episode);
    }

    NULLED_FREE(character);
}

rm_character_t *parse_character(json_object *jobj) {
    rm_character_t *character = NULL;

    if (!jobj)
        return NULL;

    character = malloc(sizeof(rm_character_t));

    if (!character)
        return NULL;

    if (!memset(character, 0, sizeof(rm_character_t))) {
        NULLED_FREE(character);
        return NULL;
    }

    JSON_GET_OBJ_DICT(jobj, character, location);
    JSON_GET_OBJ_DICT(jobj, character, origin);

    JSON_GET_OBJ_UINT(jobj, character, uint64, id);

    JSON_GET_OBJ_STR(jobj, character, name);
    JSON_GET_OBJ_STR(jobj, character, status);
    JSON_GET_OBJ_STR(jobj, character, species);
    JSON_GET_OBJ_STR(jobj, character, type);
    JSON_GET_OBJ_STR(jobj, character, gender);
    JSON_GET_OBJ_STR(jobj, character, image);
    JSON_GET_OBJ_STR(jobj, character, url);
    JSON_GET_OBJ_STR(jobj, character, created);

    json_object *_tmp = NULL;
    json_object_object_get_ex(jobj, "episode", &_tmp);

    if (json_object_get_type(_tmp) == json_type_array) {
        character->episode_len = json_object_array_length(_tmp);
        character->episode = calloc(character->episode_len, sizeof(char *));
        
        if (!character->episode) {
            NULLED_FREE(character);
            return NULL;
        }

        for (size_t i = 0; i < character->episode_len; i++) {
            character->episode[i] = strdup(json_object_get_string(json_object_array_get_idx(_tmp, i)));
        }
    }

    return character;
}

char *character_parse_filter(char *url, rm_character_t *filter) {
    if (!filter)
        return NULL;
    
    APPEND_FILTER(filter, url, species);
    if (!url) return NULL;
    
    APPEND_FILTER(filter, url, name);
    if (!url) return NULL;
    
    APPEND_FILTER(filter, url, type);
    if (!url) return NULL;
    
    APPEND_FILTER(filter, url, status);
    if (!url) return NULL;
    
    APPEND_FILTER(filter, url, gender);
    if (!url) return NULL;

    return url;
}

rm_character_t *rm_character_get(uint64_t uid) {
    return (rm_character_t *)rm_entity_get(uid, RM_CHARACTER);
}

rm_character_arr_t *rm_character_get_list(uint64_t array_id[], size_t array_len) {
    return (rm_character_arr_t *)rm_entity_get_list(array_id, array_len, RM_CHARACTER);
}

rm_character_arr_t *rm_character_get_page(rm_info_t *info, uint64_t page, rm_character_t *filter) {
    return (rm_character_arr_t *)rm_entity_get_page(info, page, (void *)filter, RM_CHARACTER);
}

rm_character_arr_t *rm_character_get_all(rm_character_t *filter) {
    return (rm_character_arr_t *)rm_entity_get_all((void *)filter, RM_CHARACTER);
}

void rm_character_da_clear(rm_character_arr_t *character) {
    rm_entity_da_clear(((rm_character_arr_t *)character),  character);
}
