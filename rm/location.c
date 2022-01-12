#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "utils.h"
#include "api.h"
#include "location.h"

rm_location_t *parse_location(json_object *jobj);
struct RM_location_array *location_array_new(void);
struct RM_location_array *parse_location_array(json_object *jobj);
struct RM_location_array *parse_location_list(json_object **jobj);
struct RM_location_array *parse_location_all(rm_info_t *info, json_object *jobj);

struct RM_location_array *location_array_new(void) {
    struct RM_location_array *location = NULL;
    
    location = malloc(sizeof(struct RM_location_array));
    
    if (!location)
        return NULL;

    if (!memset(location, 0, sizeof(struct RM_location_array))) {
        free(location);
        return NULL;
    }

    return location;
}

struct RM_location_array *parse_location_array(json_object *jobj) {
    struct RM_location_array *location = NULL;
    size_t len = 0;
    json_object *tmp_obj = NULL;

    location = location_array_new(); 

    len = json_object_array_length(jobj);

    location->da_location = calloc(len, sizeof(rm_location_t *));
   
    if (!location->da_location) {
        free(location);
        return NULL;
    }
 
    if (!memset(location->da_location, 0, (len * sizeof(rm_location_t *)))) {
        free(location->da_location);
        free(location);
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        tmp_obj = json_object_array_get_idx(jobj, i);

        if (!tmp_obj) {
            rm_da_location_clear(location);
            return  NULL;
        }

        location->da_location[i] = parse_location(tmp_obj);
        location->len++;
        
        if (!location->da_location[i]) {
            rm_da_location_clear(location);
            return NULL;
        }
    }

    return location;
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

void rm_da_location_clear(struct RM_location_array *location) {
    if (!location || !location->da_location)
        return;

    for (size_t i = 0; i < location->len; i++)
        rm_location_clear(location->da_location[i]);
    
    free(location->da_location);
    free(location);
}

struct RM_location_array *parse_location_list(json_object **jobj) {
    struct RM_location_array *location = NULL;
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
        
    location = parse_location_array(*jobj);

    return location;
}

rm_location_t *parse_location(json_object *jobj) {
    rm_location_t *location = NULL;

    if (!jobj)
        return NULL;

    location = malloc(sizeof(rm_location_t));

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
        location->residents = calloc(location->residents_len, sizeof(char *));
        
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

struct RM_location_array *parse_location_all(rm_info_t *info, json_object *jobj) {
    struct RM_location_array *location = NULL;
    json_object *tmp_obj = NULL;

    if (!json_object_object_get_ex(jobj, "info", &tmp_obj))
        return NULL;        

    if (!parse_all_info(info, tmp_obj))
        return NULL;

    if (!json_object_object_get_ex(jobj, "results", &tmp_obj))
        return NULL;        

    location = parse_location_array(tmp_obj);
    
    return location;  
}

rm_location_t *rm_location_get(uint64_t id) {
    rm_location_t *location = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (id <= 0)
        return NULL;

    url = make_url_id(RM_API_URL_LOCATION, id);

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

    location = parse_location(jobj);

    json_object_put(jobj);

    return location;
}

struct RM_location_array *rm_location_get_list(uint64_t array_id[], size_t array_len) {
    struct RM_location_array *location = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (!array_id || array_len <= 0)
        return NULL;

    url = make_url_id_list(RM_API_URL_LOCATION, array_id, array_len);

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

    location = parse_location_list(&jobj);

    json_object_put(jobj);

    return location;
}

struct RM_location_array *rm_location_get_all(rm_location_t *filter) {
    struct RM_location_array *location[2] = {NULL};
    rm_info_t info = {0};
    void *tmp = NULL;
    size_t total_pages = 0;
    size_t total_size = 0;
    size_t index = 0;

    location[0] = rm_location_get_page(&info, 1, filter);

    if (!location[0])
        return NULL;
    
    total_pages = info.pages;

    clear_all_info(&info);

    total_size = ((total_pages * 20) * sizeof(rm_location_t *));

    tmp = realloc(location[0]->da_location, total_size);

    if (!tmp) {
        rm_da_location_clear(location[0]);
        return NULL;
    }

    location[0]->da_location = tmp;

    index = location[0]->len;

    for (size_t i = 2; i <= total_pages; i++) {
        location[1] = rm_location_get_page(&info, i, filter);

        if (!location[1]) {
            rm_da_location_clear(location[0]);
            return NULL;
        }

        for (size_t j = 0; j < location[1]->len; j++)
            location[0]->da_location[index++] = location[1]->da_location[j];

        location[0]->len += location[1]->len;

        clear_all_info(&info);
        free(location[1]->da_location);
        free(location[1]);
    }

    tmp = realloc(location[0]->da_location, (location[0]->len * sizeof(rm_location_t *)));

    if (!tmp) {
        rm_da_location_clear(location[0]);
        rm_da_location_clear(location[1]);
        return NULL;
    }

    location[0]->da_location = tmp;

    return location[0];
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

struct RM_location_array *rm_location_get_page(rm_info_t *info, uint64_t page, rm_location_t *filter) {
    struct RM_location_array *location = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (!info)
        return NULL;

    if (page == 0) page = 1;

    url = make_url_page(RM_API_URL_LOCATION, page);

    if (!url)
        return NULL;

    if (filter) {
        url = location_parse_filter(url, filter);

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
 
    location = parse_location_all(info, jobj);

    json_object_put(jobj);
    
    return location;
}
