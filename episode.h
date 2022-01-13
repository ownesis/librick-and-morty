#ifndef __RICK_AND_MORTY_EPISODE_H__
#define __RICK_AND_MORTY_EPISODE_H__

/**
 * \file episode.h
 * \author ownesis
 * \date 9 janv. 2022
 * \brief Functions and structures for 'episode' API
 *
 * \see https://rickandmortyapi.com/documentation/#episode
 *
 * */

#include <stddef.h>
#include <stdint.h>

#include "types.h"
#include "api.h"

#define RM_API_URL_EPISODE RM_API_ENDPOINT "/episode/"

/** 
 * \struct RM_episode episode.h
 * \brief Structure for episode attributes.
 *
 * RM_episode have typedef : rm_episode_t
 *
 * \see https://rickandmortyapi.com/documentation/#episode-schema
 * */
struct RM_episode {
    uint64_t id; /**< ID (uint64_t) of the episode */
    char *name; /**< Name (string) of the episode */
    char *air_date; /**< Air date (string) of the episode */
    char *episode; /**< Episode (String) */
    size_t characters_len; /** Len (size_t) of episodes array */
    da_array_str_t characters; /**< Characters list (dynamic array of string urls) who have been seen in the episode */
    char *url; /**< Url (string) of the episode */
    char *created; /**< Date time (string) at which the episode was created in the database */ 
};
typedef struct RM_episode rm_episode_t;

/** \struct RM_episode_array episode.h
 * \brief Structure for an array of RM_episode struct and its length.
 * */
struct RM_episode_array {
    rm_episode_t **da_episode; /**< An "Array" of struct RM_episode */
    size_t len; /**< Len of the "Array" */
};
typedef struct RM_episode_array rm_episode_arr_t;

rm_episode_t *parse_episode(json_object *jobj);
char *episode_parse_filter(char *url, rm_episode_t *filter);

/**
 * \fn void rm_episode_clear(rm_episode_t *episode)
 * \brief This function free the episode array fields and episode.
 * \param[in] ctx (rm_episode_t *) Address of rm_episode_t structure to free.
 *
 * */
void rm_episode_clear(rm_episode_t *episode);

/**
 * \fn void rm_episode_da_clear(rm_episode_arr_t *episode)
 * \brief This function free all episode fields and episode inside da_array episode.
 * \param[in] ctx (rm_episode_arr_t *) Address of RM_episode_array structure to free`.
 *
 * */
void rm_episode_da_clear(rm_episode_arr_t *episode);

/**
 * \fn rm_episode_t *rm_episode_get(uint64_t id)
 * \brief This function get all attributs of the episode with the specific ID.
 * \param[in] id (uint64_t) The ID of the episode to get this attributs.
 * \return (rm_episode_t*) Address of struct RM_episode with field filled to free with `rm_episode_clear`.
 *
 * */
rm_episode_t *rm_episode_get(uint64_t id);

/**
 * \fn rm_episode_arr_t *rm_episode_get_all(rm_episode_t *filter)
 * \brief This function get all episodes and their attributes.
 * \param[in] filter (rm_episode_t *) Address of a RM_episode structure to use like filter (only the following field are verifed: name, status, species, type, gender) he can be NULL. 
 * \return address of struct RM_episode_array to free with `rm_da_episode_clear`.
 *
 * */
rm_episode_arr_t *rm_episode_get_all(rm_episode_t *filter);

/**
 * \fn rm_episode_arr_t *rm_episode_get_list(uint64_t array_id[], size_t array_len)
 * \brief This function get all attributes of episode with id inside array_id.
 * \param[in] array_id (uint64_t[]) An array with the number of ID of characeters to get. 
 * \param[in] array_len (size_t) The len of array. 
 * \return Address of struct RM_episode_array to free with `rm_episode_da_clear`.
 *
 * */
rm_episode_arr_t *rm_episode_get_list(uint64_t array_id[], size_t array_len);

/**
 * \fn rm_episode_arr_t *rm_episode_get_page(rm_info_t *info, uint64_t page, rm_episode_t *filter)
 * \brief This function get an array of all episode found in the specified page.
 * \param[out] info (rm_info_t *) Address of rm_info_t. 
 * \param[in] page (size_t) the page to get. 
 * \param[in] filter (rm_episode_t *) Addres of a RM_episode structure to use like filter (only the following field are verifed: name, status, species, type, gender) he can be NULL. 
 * \return address of struct RM_episode_array to free with `rm_episode_da_clear`.
 *
 * */
rm_episode_arr_t *rm_episode_get_page(rm_info_t *info, uint64_t page, rm_episode_t *filter);

#endif /* __RICK_AND_MORTY_EPISODE_H__ */
