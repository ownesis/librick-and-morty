#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "utils.h"
#include "api.h"
#include "location.h"

rm_location_t *parse_location(json_object *jobj) {
    rm_location_t *location = NULL;

    if (!jobj)
        return NULL;

    RM_OBJECT_NEW(location, 1, rm_location_t);

    if (!location)
        return NULL;

    if (!memset(location, 0, sizeof(rm_location_t))) {
        free(location);
        return NULL;
    }

    JSON_GET_OBJ_UINT(jobj, location, uint64, id);

    JSON_GET_OBJ_STR(jobj, location, name);
    JSON_GET_OBJ_STR(jobj, location, dimension);
    JSON_GET_OBJ_STR(jobj, location, type);
    JSON_GET_OBJ_STR(jobj, location, url);
    JSON_GET_OBJ_STR(jobj, location, created);

    json_object *_tmp = NULL;
    json_object_object_get_ex(jobj, "residents", &_tmp);

    if (json_object_get_type(_tmp) == json_type_array) {
        location->residents_len = json_object_array_length(_tmp);
        RM_OBJECT_NEW(location->residents, location->residents_len, char *);
        
        if (!location->residents) {
            free(location);   
            return NULL;
        }

        for (size_t i = 0; i < location->residents_len; i++) {
            location->residents[i] = strdup(json_object_get_string(json_object_array_get_idx(_tmp, i)));
        }
    }

    return location;
}

char *location_parse_filter(char *url, rm_location_t *filter) {
    if (!filter)
        return NULL;
    
    APPEND_FILTER(filter, url, name);
    if (!url) return NULL;
    
    APPEND_FILTER(filter, url, type);
    if (!url) return NULL;

    APPEND_FILTER(filter, url, dimension);
    if (!url) return NULL;

    return url;
}

void rm_location_clear(rm_location_t *location) {
    if (!location)
        return;

    FREE_IF_NONULL(location->name);
    FREE_IF_NONULL(location->dimension);
    FREE_IF_NONULL(location->type);
    FREE_IF_NONULL(location->url);
    FREE_IF_NONULL(location->created);
    
    if (location->residents) {
        for (size_t i = 0; i < location->residents_len; i++)
            free(location->residents[i]);

        free(location->residents);
    }

    free(location);
}

rm_location_t *rm_location_get(uint64_t uid) {
    return (rm_location_t *)rm_entity_get(uid, RM_LOCATION);
}

rm_location_arr_t *rm_location_get_list(uint64_t array_id[], size_t array_len) {
    return (rm_location_arr_t *)rm_entity_get_list(array_id, array_len, RM_LOCATION);
}

rm_location_arr_t *rm_location_get_page(rm_info_t *info, uint64_t page, rm_location_t *filter) {
    return (rm_location_arr_t *)rm_entity_get_page(info, page, (void *)filter, RM_LOCATION);
}

rm_location_arr_t *rm_location_get_all(rm_location_t *filter) {
    return (rm_location_arr_t *)rm_entity_get_all((void *)filter, RM_LOCATION);
}

void rm_location_da_clear(rm_location_arr_t *location) {
    rm_entity_da_clear(((rm_location_arr_t *)location),  location);
}
