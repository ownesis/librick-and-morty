#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "utils.h"
#include "api.h"
#include "character.h"

rm_character_t *parse_character(json_object *jobj);
struct RM_character_array *character_array_new(void);
struct RM_character_array *parse_character_array(json_object *jobj);
struct RM_character_array *parse_character_list(json_object **jobj);
struct RM_character_array *parse_character_all(rm_info_t *info, json_object *jobj);

struct RM_character_array *character_array_new(void) {
    struct RM_character_array *character = NULL;
    
    character = malloc(sizeof(struct RM_character_array));
    
    if (!character)
        return NULL;

    if (!memset(character, 0, sizeof(struct RM_character_array))) {
        free(character);
        return NULL;
    }

    return character;
}

struct RM_character_array *parse_character_array(json_object *jobj) {
    struct RM_character_array *character = NULL;
    size_t len = 0;
    json_object *tmp_obj = NULL;

    character = character_array_new(); 

    len = json_object_array_length(jobj);

    character->da_character = calloc(len, sizeof(rm_character_t *));
   
    if (!character->da_character) {
        free(character);
        return NULL;
    }
 
    if (!memset(character->da_character, 0, (len * sizeof(rm_character_t *)))) {
        free(character->da_character);
        free(character);
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        tmp_obj = json_object_array_get_idx(jobj, i);

        if (!tmp_obj) {
            rm_da_character_clear(character);
            return  NULL;
        }

        character->da_character[i] = parse_character(tmp_obj);
        character->len++;
        
        if (!character->da_character[i]) {
            rm_da_character_clear(character);
            return NULL;
        }
    }

    return character;
}

void rm_character_clear(rm_character_t *character) {
    if (!character)
        return;

    FREE_IF_NONULL(character->name);
    FREE_IF_NONULL(character->status);
    FREE_IF_NONULL(character->species);
    FREE_IF_NONULL(character->type);
    FREE_IF_NONULL(character->gender);
    FREE_IF_NONULL(character->image);
    FREE_IF_NONULL(character->url);
    FREE_IF_NONULL(character->created);
    FREE_IF_NONULL(character->location.name);
    FREE_IF_NONULL(character->location.url);
    FREE_IF_NONULL(character->origin.name);
    FREE_IF_NONULL(character->origin.url);
    
    if (character->episode) {
        for (size_t i = 0; i < character->episode_len; i++)
            free(character->episode[i]);

        free(character->episode);
    }

    free(character);
}

void rm_da_character_clear(struct RM_character_array *character) {
    if (!character || !character->da_character)
        return;

    for (size_t i = 0; i < character->len; i++)
        rm_character_clear(character->da_character[i]);
    
    free(character->da_character);
    free(character);
}

struct RM_character_array *parse_character_list(json_object **jobj) {
    struct RM_character_array *character = NULL;
    json_object *tmp_obj = NULL;

    if (!(*jobj))
        return NULL;

    /* jobj is type json_type_object if only one ID */
    if (json_object_get_type(*jobj) == json_type_object) {
       tmp_obj = put_inside_json_array(*jobj);

       if (!tmp_obj)
           return NULL;

       *jobj = tmp_obj;
    }
        
    character = parse_character_array(*jobj);

    return character;
}

rm_character_t *parse_character(json_object *jobj) {
    rm_character_t *character = NULL;

    if (!jobj)
        return NULL;

    character = malloc(sizeof(rm_character_t));

    if (!character)
        return NULL;

    if (!memset(character, 0, sizeof(rm_character_t))) {
        free(character);
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
            free(character);   
            return NULL;
        }

        for (size_t i = 0; i < character->episode_len; i++) {
            character->episode[i] = strdup(json_object_get_string(json_object_array_get_idx(_tmp, i)));
        }
    }

    return character;
}

struct RM_character_array *parse_character_all(rm_info_t *info, json_object *jobj) {
    struct RM_character_array *character = NULL;
    json_object *tmp_obj = NULL;

    if (!json_object_object_get_ex(jobj, "info", &tmp_obj))
        return NULL;        

    if (!parse_all_info(info, tmp_obj))
        return NULL;

    if (!json_object_object_get_ex(jobj, "results", &tmp_obj))
        return NULL;        

    character = parse_character_array(tmp_obj);
    
    return character;  
}

rm_character_t *rm_character_get(uint64_t id) {
    rm_character_t *character = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (id <= 0)
        return NULL;

    url = make_url_id(RM_API_URL_CHARACTER, id);

    if (!url)
        return NULL;

    chunk = rm_api_request(url);

    if (!chunk.ptr)
        return NULL;

    free(url);

    jobj = json_tokener_parse(chunk.ptr);
    
    if (!jobj)
        return NULL;

    free(chunk.ptr);

    character = parse_character(jobj);

    json_object_put(jobj);

    return character;
}

struct RM_character_array *rm_character_get_list(uint64_t array_id[], size_t array_len) {
    struct RM_character_array *character = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (!array_id || array_len <= 0)
        return NULL;

    url = make_url_id_list(RM_API_URL_CHARACTER, array_id, array_len);

    if (!url)
        return NULL;

    chunk = rm_api_request(url);

    free(url);
 
    if (!chunk.ptr)
        return NULL;

    jobj = json_tokener_parse(chunk.ptr);

    if (!jobj)
        return NULL;
 
    free(chunk.ptr);

    character = parse_character_list(&jobj);

    json_object_put(jobj);

    return character;
}

struct RM_character_array *rm_character_get_all(rm_character_t *filter) {
    struct RM_character_array *character[2] = {NULL};
    rm_info_t info = {0};
    void *tmp = NULL;
    size_t total_pages = 0;
    size_t total_size = 0;
    size_t index = 0;

    character[0] = rm_character_get_page(&info, 1, filter);

    if (!character[0])
        return NULL;
    
    total_pages = info.pages;

    clear_all_info(&info);

    total_size = ((total_pages * 20) * sizeof(rm_character_t *));

    tmp = realloc(character[0]->da_character, total_size);

    if (!tmp) {
        rm_da_character_clear(character[0]);
        return NULL;
    }

    character[0]->da_character = tmp;

    index = character[0]->len;

    for (size_t i = 2; i <= total_pages; i++) {
        character[1] = rm_character_get_page(&info, i, filter);

        if (!character[1]) {
            rm_da_character_clear(character[0]);
            return NULL;
        }

        for (size_t j = 0; j < character[1]->len; j++)
            character[0]->da_character[index++] = character[1]->da_character[j];

        character[0]->len += character[1]->len;

        clear_all_info(&info);
        free(character[1]->da_character);
        free(character[1]);
    }

    tmp = realloc(character[0]->da_character, (character[0]->len * sizeof(rm_character_t *)));

    if (!tmp) {
        rm_da_character_clear(character[0]);
        rm_da_character_clear(character[1]);
        return NULL;
    }

    character[0]->da_character = tmp;

    return character[0];
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

struct RM_character_array *rm_character_get_page(rm_info_t *info, uint64_t page, rm_character_t *filter) {
    struct RM_character_array *character = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (!info)
        return NULL;

    if (page == 0) page = 1;

    url = make_url_page(RM_API_URL_CHARACTER, page);

    if (!url)
        return NULL;

    if (filter) {
        url = character_parse_filter(url, filter);

        if (!url)
            return NULL;
    }

    chunk = rm_api_request(url);

    if (!chunk.ptr) {
        free(url);
        return NULL;
    }

    free(url);

    if (!chunk.ptr)
        return NULL;

    jobj = json_tokener_parse(chunk.ptr);

    if (!jobj)
        return NULL;
    
    free(chunk.ptr);
 
    character = parse_character_all(info, jobj);

    json_object_put(jobj);
    
    return character;
}
