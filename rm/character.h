#ifndef __RICK_AND_MORTY_CHARACTER_H__
#define __RICK_AND_MORTY_CHARACTER_H__

/**
 * \file character.h
 * \author ownesis
 * \date 9 Janv. 2022
 * \brief Functions and structures for 'character' API
 *
 * \see https://rickandmortyapi.com/documentation/#character
 *
 * */

#include <stddef.h>
#include <stdint.h>

#include "types.h"
#include "api.h"

/**
 * \def RM_API_URL_CHARACTER
 * */
#define RM_API_URL_CHARACTER RM_API_ENDPOINT "/character/"

/**
 * \struct RM_dict character.h
 * \brief Structure for 'name' and 'url' string for 'origin' and 'location' field of RM_character structure.
 *
 * */
struct RM_dict {
      char *name; /**< Name (string) */
      char *url; /**< Url (string url) */
};

/**
 * \struct RM_character character.h "Character attributes"
 * \brief Structure for character attributes
 *
 * RM_character have typedef : rm_character_t
 *
 * \see https://rickandmortyapi.com/documentation/#character-schema
 * */
struct RM_character {
    uint64_t id; /**< ID of the character */
    char *name; /**< Name (string) of the character */
    char *status; /**< Status (string) of the character. ("unknown" | "Dead" | "Alive") */
    char *species; /**< Species (string) of the character */
    char *type; /**< Type (string) of the character */
    char *gender; /**< Gender (string) of the character. ("Female" | "Male" | "Genderless" | "unknown") */
    struct RM_dict origin; /**< Origin (struct RM_dict) of the character */
    struct RM_dict location; /**< Location (struct RM_dict) of the character */ 
    char *image; /**< Image (string url) of the character, image of 300x300px */
    size_t episode_len; /**< (size_t) Length of episode array */
    da_array_str_t episode; /**< Episode list (dynamic array of url string) which this character appeared */
    char *url; /**< Url (string) of the character */
    char *created; /**< Date time (string) at which the character was created in the database. */
};
typedef struct RM_character rm_character_t;

/**
 * \struct RM_character_array character.h
 * \brief Structure for an array of RM_character struct and its length.
 *
 * */
struct RM_character_array {
    struct RM_character **da_character; /**< An "array" of struct RM_character */
    size_t len; /**< Len of the "Array" */
};
typedef struct RM_character_array rm_character_arr_t;

rm_character_t *parse_character(json_object *jobj);
char *character_parse_filter(char *url, rm_character_t *filter);

/**
 * \fn void rm_character_clear(rm_character_t *character)
 * \brief This function free the episode array fields and character.
 * \param[in] ctx (rm_character_t *) Address of rm_character_t structure to free.
 *
 * */
void rm_character_clear(rm_character_t *character);

/**
 * \fn void rm_character_da_clear(rm_character_arr_t *character)
 * \brief This function free all episode fields and character inside da_array character.
 * \param[in] ctx (rm_character_arr_t *) Address of RM_character_array structure to free`.
 *
 * */
void rm_character_da_clear(rm_character_arr_t *character);

/**
 * \fn rm_character_t *rm_character_get(uint64_t id)
 * \brief This function get all attributs of the character with the specific ID.
 * \param[in] id (uint64_t) The ID of the character to get this attributs.
 * \return (rm_character_t*) Address of struct RM_character with field filled to free with `rm_character_clear`.
 *
 * */
rm_character_t *rm_character_get(uint64_t id);

/**
 * \fn rm_character_arr_t *rm_character_get_all(rm_character_t *filter)
 * \brief This function get all characters and their attributes.
 * \param[in] filter (rm_character_t *) Address of a RM_character structure to use like filter (only the following field are verifed: name, status, species, type, gender) he can be NULL. 
 * \return address of struct RM_character_array to free with `rm_da_character_clear`.
 *
 * */
rm_character_arr_t *rm_character_get_all(rm_character_t *filter);

/**
 * \fn rm_character_arr_t *rm_character_get_list(uint64_t array_id[], size_t array_len)
 * \brief This function get all attributes of character with id inside array_id.
 * \param[in] array_id (uint64_t[]) An array with the number of ID of characeters to get. 
 * \param[in] array_len (size_t) The len of array. 
 * \return Address of struct RM_character_array to free with `rm_character_da_clear`.
 *
 * */
rm_character_arr_t *rm_character_get_list(uint64_t array_id[], size_t array_len);

/**
 * \fn rm_character_arr_t *rm_character_get_page(rm_info_t *info, uint64_t page, rm_character_t *filter)
 * \brief This function get an array of all character found in the specified page.
 * \param[out] info (rm_info_t *) Address of rm_info_t. 
 * \param[in] page (size_t) the page to get. 
 * \param[in] filter (rm_character_t *) Addres of a RM_character structure to use like filter (only the following field are verifed: name, status, species, type, gender) he can be NULL. 
 * \return address of struct RM_character_array to free with `rm_character_da_clear`.
 *
 * */
rm_character_arr_t *rm_character_get_page(rm_info_t *info, uint64_t page, rm_character_t *filter);

#endif /* __RICK_AND_MORTY_CHARACTER_H__ */
