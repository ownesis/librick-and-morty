#include <string.h>
#include <curl/curl.h>
#include "api.h"
#include "types.h"
#include "utils.h"
#include "character.h"
#include "episode.h"
#include "location.h"

void *rm_entity_get_page(rm_info_t *info, uint64_t page, void *filter, rm_entity_t e_entity) {
    void *entity = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (!info)
        return NULL;

    if (page == 0) page = 1;

    switch (e_entity) {
        case RM_LOCATION:
            url = make_url_page(RM_API_URL_LOCATION, page);
            break;
        case RM_CHARACTER:
            url = make_url_page(RM_API_URL_CHARACTER, page);
            break;
        case RM_EPISODE:
            url = make_url_page(RM_API_URL_EPISODE, page);
            break;
    }

    if (!url)
        return NULL;

    if (filter) {
        switch (e_entity) {
            case RM_LOCATION:
                url = location_parse_filter(url, (rm_location_t *)filter);
                break;
            case RM_CHARACTER:
                url = character_parse_filter(url, (rm_character_t *)filter);
                break;
            case RM_EPISODE:
                url = episode_parse_filter(url, (rm_episode_t *)filter);
                break;        
        }
        
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
 
    switch (e_entity) {
        case RM_LOCATION: 
            entity = parse_entity_all(info, jobj, RM_LOCATION);
            break;
        case RM_CHARACTER: 
            entity = parse_entity_all(info, jobj, RM_CHARACTER);
            break;
        case RM_EPISODE: 
            entity = parse_entity_all(info, jobj, RM_EPISODE);
            break;
    }

    json_object_put(jobj);
    
    return entity;
}

void *rm_entity_get_all(void *filter, rm_entity_t e_entity) {
    void *entity[2] = {NULL};
    rm_info_t info = {0};
    void *tmp = NULL;
    size_t total_pages = 0;
    size_t total_size = 0;
    size_t index = 0;

    switch (e_entity) {
        case RM_LOCATION:
            entity[0] = rm_entity_get_page(&info, 1, filter, RM_LOCATION);
            if (!entity[0])
                return NULL;
            break;
        case RM_CHARACTER:
            entity[0] = rm_entity_get_page(&info, 1, filter, RM_CHARACTER);
            if (!entity[0])
                return NULL;
            break;
        case RM_EPISODE:
            entity[0] = rm_entity_get_page(&info, 1, filter, RM_EPISODE);
            if (!entity[0])
                return NULL;
            break;
    } 
    
    total_pages = info.pages;

    clear_all_info(&info);

    total_size = ((total_pages * 20) * sizeof(rm_location_t *));

    switch (e_entity) {
        case RM_LOCATION:
            tmp = realloc(((rm_location_arr_t *)entity[0])->da_location, total_size);

            if (!tmp) {
                rm_da_entity_clear(((rm_location_arr_t *)entity[0]), location);
                return NULL;
            }

            ((rm_location_arr_t *)entity[0])->da_location = tmp;

            index = ((rm_location_arr_t *)entity[0])->len;
            break;

        case RM_CHARACTER:
            tmp = realloc(((rm_character_arr_t *)entity[0])->da_character, total_size);

            if (!tmp) {
                rm_da_entity_clear(((rm_character_arr_t *)entity[0]), character);
                return NULL;
            }

            ((rm_character_arr_t *)entity[0])->da_character = tmp;

            index = ((rm_character_arr_t *)entity[0])->len;
            break;

        case RM_EPISODE:
            tmp = realloc(((rm_episode_arr_t *)entity[0])->da_episode, total_size);

            if (!tmp) {
                rm_da_entity_clear(((rm_episode_arr_t *)entity[0]), episode);
                return NULL;
            }

            ((rm_episode_arr_t *)entity[0])->da_episode = tmp;

            index = ((rm_episode_arr_t *)entity[0])->len;
            break;
    }

    for (size_t i = 2; i <= total_pages; i++) {
        switch (e_entity) {
            case RM_LOCATION:
                entity[1] = rm_entity_get_page(&info, i, filter, RM_LOCATION);
        
                if (!entity[1]) {
                    rm_da_entity_clear(((rm_location_arr_t *)entity[0]), location);
                    return NULL;
                }

                for (size_t j = 0; j < ((rm_location_arr_t *)entity[1])->len; j++)
                    ((rm_location_arr_t *)entity[0])->da_location[index++] = ((rm_location_arr_t *)entity[1])->da_location[j];

                ((rm_location_arr_t *)entity[0])->len += ((rm_location_arr_t *)entity[1])->len;

                free(((rm_location_arr_t *)entity[1])->da_location);
                free(entity[1]);
                break;

            case RM_CHARACTER:
                entity[1] = rm_entity_get_page(&info, i, filter, RM_CHARACTER);
        
                if (!entity[1]) {
                    rm_da_entity_clear(((rm_character_arr_t *)entity[0]), character);
                    return NULL;
                }

                for (size_t j = 0; j < ((rm_character_arr_t *)entity[1])->len; j++)
                    ((rm_character_arr_t *)entity[0])->da_character[index++] = ((rm_character_arr_t *)entity[1])->da_character[j];

                ((rm_character_arr_t *)entity[0])->len += ((rm_character_arr_t *)entity[1])->len;

                free(((rm_character_arr_t *)entity[1])->da_character);
                free(entity[1]);
                break;

            case RM_EPISODE:
                entity[1] = rm_entity_get_page(&info, i, filter, RM_EPISODE);
        
                if (!entity[1]) {
                    rm_da_entity_clear(((rm_episode_arr_t *)entity[0]), episode);
                    return NULL;
                }

                for (size_t j = 0; j < ((rm_episode_arr_t *)entity[1])->len; j++)
                    ((rm_episode_arr_t *)entity[0])->da_episode[index++] = ((rm_episode_arr_t *)entity[1])->da_episode[j];

                ((rm_episode_arr_t *)entity[0])->len += ((rm_episode_arr_t *)entity[1])->len;

                free(((rm_episode_arr_t *)entity[1])->da_episode);
                free(entity[1]);
                break;
        }

        clear_all_info(&info);
    }


    switch (e_entity) {
        case RM_LOCATION:
            tmp = realloc(((rm_location_arr_t *)entity[0])->da_location, (((rm_location_arr_t *)entity[0])->len * sizeof(rm_location_t *)));

            if (!tmp) {
                rm_da_entity_clear(((rm_location_arr_t *)entity[1]), location);
                rm_da_entity_clear(((rm_location_arr_t *)entity[1]), location);
                return NULL;
            }

            ((rm_location_arr_t *)entity[0])->da_location = tmp;
            break;

        case RM_CHARACTER:
            tmp = realloc(((rm_character_arr_t *)entity[0])->da_character, (((rm_character_arr_t *)entity[0])->len * sizeof(rm_character_t *)));

            if (!tmp) {
                rm_da_entity_clear(((rm_character_arr_t *)entity[1]), character);
                rm_da_entity_clear(((rm_character_arr_t *)entity[1]), character);
                return NULL;
            }

            ((rm_character_arr_t *)entity[0])->da_character = tmp;
            break;

        case RM_EPISODE:
            tmp = realloc(((rm_episode_arr_t *)entity[0])->da_episode, (((rm_episode_arr_t *)entity[0])->len * sizeof(rm_episode_t *)));

            if (!tmp) {
                rm_da_entity_clear(((rm_episode_arr_t *)entity[1]), episode);
                rm_da_entity_clear(((rm_episode_arr_t *)entity[1]), episode);
                return NULL;
            }

            ((rm_episode_arr_t *)entity[0])->da_episode = tmp;
            break;
    }

    return entity[0];
}

void *rm_entity_get_list(uint64_t array_id[], size_t array_len, rm_entity_t e_entity) {
    void *entity = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (!array_id || array_len <= 0)
        return NULL;

    switch (e_entity) {
        case RM_LOCATION:
            url = make_url_id_list(RM_API_URL_LOCATION, array_id, array_len);
            break;
        case RM_CHARACTER:
            url = make_url_id_list(RM_API_URL_CHARACTER, array_id, array_len);
            break;
        case RM_EPISODE:
            url = make_url_id_list(RM_API_URL_EPISODE, array_id, array_len);
            break;
    }

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

    switch (e_entity) {
        case RM_LOCATION:
            entity = parse_entity_list(&jobj, RM_LOCATION);
            break;
        case RM_CHARACTER:
            entity = parse_entity_list(&jobj, RM_CHARACTER);
            break;
        case RM_EPISODE:
            entity = parse_entity_list(&jobj, RM_EPISODE);
            break;
    }

    json_object_put(jobj);

    return entity;
}

void *rm_entity_get(uint64_t id, rm_entity_t e_entity) {
    void *entity = NULL;
    struct Mem chunk = {NULL};
    char *url = NULL;
    json_object *jobj = NULL;

    if (id <= 0)
        return NULL;

    switch (e_entity) {
        case RM_LOCATION:
            url = make_url_id(RM_API_URL_LOCATION, id);
            break;
        case RM_CHARACTER:
            url = make_url_id(RM_API_URL_CHARACTER, id);
            break;
        case RM_EPISODE:
            url = make_url_id(RM_API_URL_EPISODE, id);
            break;
    }
    
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

    switch (e_entity) {
        case RM_LOCATION: 
            entity = parse_location(jobj);
            break;
        case RM_CHARACTER: 
            entity = parse_character(jobj);
            break;
        case RM_EPISODE: 
            entity = parse_episode(jobj);
            break;
    }

    json_object_put(jobj);

    return entity;
}

void *parse_entity_list(json_object **jobj, rm_entity_t e_entity) {
    void *entity = NULL;
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
        
    switch (e_entity) {
        case RM_LOCATION:
            entity = parse_entity_array(*jobj, RM_LOCATION);
            break;

        case RM_CHARACTER:
            entity = parse_entity_array(*jobj, RM_CHARACTER);
            break;
        case RM_EPISODE:
            entity = parse_entity_array(*jobj, RM_EPISODE);
            break;
    }

    return entity;
}

void *parse_entity_all(rm_info_t *info, json_object *jobj, rm_entity_t e_entity) {
    void *entity = NULL;
    json_object *tmp_obj = NULL;

    if (!json_object_object_get_ex(jobj, "info", &tmp_obj))
        return NULL;        

    if (!parse_all_info(info, tmp_obj))
        return NULL;

    if (!json_object_object_get_ex(jobj, "results", &tmp_obj))
        return NULL;        

    switch (e_entity) {
        case RM_LOCATION:
            entity = parse_entity_array(tmp_obj, RM_LOCATION);
            break;

        case RM_CHARACTER:
            entity = parse_entity_array(tmp_obj, RM_CHARACTER);
            break;
        case RM_EPISODE:
            entity = parse_entity_array(tmp_obj, RM_EPISODE);
            break;
    }

    return entity;  
}

void *parse_entity_array(json_object *jobj, int e_entity) {
    void *entity = NULL;
    size_t len = 0;
    json_object *tmp_obj = NULL;

    switch (e_entity) {
        case RM_LOCATION:
            RM_OBJECT_NEW(entity, 1, rm_location_arr_t); 
            break;
        case RM_CHARACTER:
            RM_OBJECT_NEW(entity, 1, rm_character_arr_t); 
            break;
        case RM_EPISODE:
            RM_OBJECT_NEW(entity, 1, rm_episode_arr_t); 
            break;
    }
 
    if (!entity)
        return NULL;

    len = json_object_array_length(jobj);
    
    switch (e_entity) {
        case RM_LOCATION:
            RM_OBJECT_NEW_DA((rm_location_arr_t *)entity, len, location);

            if (!((rm_location_arr_t *)entity)->da_location)
                return NULL;
            
            break;
        
        case RM_CHARACTER:
            RM_OBJECT_NEW_DA((rm_character_arr_t *)entity, len, character);
            
            if (!((rm_character_arr_t *)entity)->da_character)
                return NULL;
            
            break;

        case RM_EPISODE:
            RM_OBJECT_NEW_DA((rm_episode_arr_t *)entity, len, episode);
        
            if (!((rm_episode_arr_t *)entity)->da_episode)
                return NULL;
        
            break;
    }

    for (size_t i = 0; i < len; i++) {
        tmp_obj = json_object_array_get_idx(jobj, i);

        if (!tmp_obj) {
            switch (e_entity) {
                case RM_LOCATION:
                    rm_da_entity_clear(((rm_location_arr_t *)entity), location);
                    break;
                case RM_CHARACTER:
                    rm_da_entity_clear(((rm_character_arr_t *)entity), character);
                    break;
                case RM_EPISODE:
                    rm_da_entity_clear(((rm_episode_arr_t *)entity), episode);
                    break; 
            }

            return  NULL;
        }

        switch (e_entity) {
            case RM_LOCATION:
                ((rm_location_arr_t *)entity)->da_location[i] = parse_location(tmp_obj);
                ((rm_location_arr_t *)entity)->len++;
                
                if (!((rm_location_arr_t *)entity)->da_location[i]) {
                    rm_da_location_clear((rm_location_arr_t *)entity);
                    return NULL;
                }

                break;
            
            case RM_CHARACTER:
                ((rm_character_arr_t *)entity)->da_character[i] = parse_character(tmp_obj);
                ((rm_character_arr_t *)entity)->len++;

                if (!((rm_character_arr_t *)entity)->da_character[i]) {
                    rm_da_character_clear((rm_character_arr_t *)entity);
                    return NULL;
                }

                break;

            case RM_EPISODE:
                ((rm_episode_arr_t *)entity)->da_episode[i] = parse_episode(tmp_obj);
                ((rm_episode_arr_t *)entity)->len++;

                if (!((rm_episode_arr_t *)entity)->da_episode[i]) {
                    rm_da_episode_clear((rm_episode_arr_t *)entity);
                    return NULL;
                }

                break; 
        }

    }

    return entity;
}

mem_t rm_api_request(const char *restrict endpoint) {
    CURL *curl_hndl = NULL;
    mem_t chunk = {NULL};
    long response_code = 0;

    if (!endpoint)
        return (mem_t){NULL};

    chunk.ptr = malloc(1);

    if (!chunk.ptr)
        return (mem_t){NULL};

    chunk.ptr[0] = 0;

    curl_hndl = curl_easy_init();

    if (!curl_hndl) {
        free(chunk.ptr);
        return (mem_t){NULL};
    }

    CURL_ERR(curl_easy_setopt(curl_hndl, CURLOPT_URL, endpoint)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};
    }
    
    CURL_ERR(curl_easy_setopt(curl_hndl, CURLOPT_WRITEFUNCTION, write_callback)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};
    }
    
    CURL_ERR(curl_easy_setopt(curl_hndl, CURLOPT_WRITEDATA, (void *)&chunk)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};  
    }
   
    CURL_ERR(curl_easy_perform(curl_hndl)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL}; 
    }
    
    CURL_ERR(curl_easy_getinfo(curl_hndl, CURLINFO_RESPONSE_CODE, &response_code)) {
        free(chunk.ptr);
        curl_easy_cleanup(curl_hndl);
        return (mem_t){NULL};
    }

    curl_easy_cleanup(curl_hndl);

    if (response_code != 200) {
        free(chunk.ptr);
        return (mem_t){NULL};
    }

    chunk.ptr[chunk.size] = '\0';

    return chunk;
}

_Bool parse_all_info(rm_info_t *info, json_object *jobj) {
    JSON_GET_OBJ_UINT(jobj, info, uint64, count);
    JSON_GET_OBJ_UINT(jobj, info, uint64, pages);
    JSON_GET_OBJ_STR(jobj, info, next);
    JSON_GET_OBJ_STR(jobj, info, prev);

    return 1;
}

void clear_all_info(rm_info_t *info) {
    if (!info)
        return;

    FREE_IF_NONULL(info->next);
    FREE_IF_NONULL(info->prev);
}
