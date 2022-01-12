#ifndef __RICK_AND_MORTY_EPISODE_H__
#define __RICK_AND_MORTY_EPISODE_H__

/*
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

/* 
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
    da_array_str_t characters; /**< Charactes list (dynamic array of string urls) who have been seen in the episode */
    char *url; /**< Url (string) of the episode */
    char *created; /**< Date time (string) at which the episode was created in the database */ 
};
typedef struct RM_episode rm_episode_t;

/* \struct RM_episode_array episode.h
 * \brief Structure for an array of RM_episode struct and its length.
 * */
struct RM_episode_array {
    rm_episode_t *da_episode; /**< An "Array" of struct RM_episode */
    size_t len; /**< Len of the "Array" */
};
typedef struct RM_episode_array rm_episode_arr_t;

/**
 * \fn rm_episode_t rm_episode_get(uint64_t id)
 * \brief This function get all attributs of the episode with the specific ID.
 * \param[in] id (uint64_t) The ID of the episode to get this attributs.
 * \return rm_episode_t (struct RM_episode) with field filled.
 *
 * */
rm_episode_t rm_episode_get(uint64_t);

/**
 * \fn struct RM_episode_array RM_episode_get_all(void)
 * \brief This function get all episodes and their attributes.
 * \return struct RM_episode_array.
 *
 * */
struct RM_episode_array rm_episode_get_all(void);

/**
 * \fn struct RM_episode_array rm_episode_get_list(uint64_t array_id[], size_t array_len)
 * \brief This function get all attributes of episode with id inside array_id.
 * \param[in] array_id (uint64_t[]) An array with the number of ID of characeters to get. 
 * \param[in] array_len (size_t) The len of array. 
 * \return struct RM_episode_array.
 *
 * */
struct RM_episode_array rm_episode_get_list(uint64_t array_id[], size_t array_len);

/**
 * \fn struct RM_episode_array rm_episode_get_filter(rm_episode_t filter)
 * \brief This function get all attributes of episode match with given filter.
 * \param[in] filter (rm_episode_t) RM_episode structure to use like filter (only the following field are verifed: name, episode). 
 * \return struct RM_episode_array.
 *
 * */
struct RM_episode_array rm_episode_get_filter(rm_episode_t filter);

#endif /* __RICK_AND_MORTY_EPISODE_H__ */
