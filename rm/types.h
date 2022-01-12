#ifndef __RICK_AND_MORTY_TYPES_H__
#define __RICK_AND_MORTY_TYPES_H__

typedef char **da_array_str_t;

struct Mem {
    char *ptr;
    size_t size;
};
typedef struct Mem mem_t;

enum RM_ENTITY {
    RM_LOCATION,
    RM_CHARACTER,
    RM_EPISODE,
};
typedef enum RM_ENTITY rm_entity_t;



#endif /* __RICK_AND_MORTY_TYPES_H__ */
