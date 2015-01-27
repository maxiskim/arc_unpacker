#include <stdlib.h>
#include <string.h>
#include "collections/dictionary.h"
#include "collections/linked_list.h"
#include "key_value.h"
#include "assert.h"

// super stupid implementation with O(n) lookup time and O(n) insertion time

struct Dictionary
{
    LinkedList *linked_list;
};

Dictionary *dictionary_create()
{
    Dictionary *dictionary = malloc(sizeof(Dictionary));
    dictionary->linked_list = linked_list_create();
    return dictionary;
}

void dictionary_destroy(Dictionary *dictionary)
{
    KeyValue *kv;
    assert_not_null(dictionary);
    linked_list_reset(dictionary->linked_list);
    while ((kv = (KeyValue*)linked_list_get(dictionary->linked_list)) != NULL)
    {
        free(kv);
        linked_list_advance(dictionary->linked_list);
    }
    linked_list_destroy(dictionary->linked_list);
    free(dictionary);
}

void dictionary_set(
    Dictionary *const dictionary,
    const char *const key,
    const void *const value)
{
    KeyValue *kv;
    linked_list_reset(dictionary->linked_list);
    while ((kv = (KeyValue*)linked_list_get(dictionary->linked_list)) != NULL)
    {
        if (strcmp(kv->key, key) == 0)
        {
            kv->value = value;
            return;
        }
        linked_list_advance(dictionary->linked_list);
    }
    kv = (KeyValue*)malloc(sizeof(KeyValue));
    kv->key = key;
    kv->value = value;
    linked_list_add(dictionary->linked_list, kv);
    return false;
}

const void *dictionary_get(
    const Dictionary *const dictionary,
    const char *const key)
{
    KeyValue *kv;
    linked_list_reset(dictionary->linked_list);
    while ((kv = (KeyValue*)linked_list_get(dictionary->linked_list)) != NULL)
    {
        if (strcmp(kv->key, key) == 0)
            return kv->value;
        linked_list_advance(dictionary->linked_list);
    }
    return NULL;
}

bool dictionary_has_key(
    const Dictionary *const dictionary,
    const char *const key)
{
    KeyValue *kv;
    linked_list_reset(dictionary->linked_list);
    while ((kv = (KeyValue*)linked_list_get(dictionary->linked_list)) != NULL)
    {
        if (strcmp(kv->key, key) == 0)
            return true;
        linked_list_advance(dictionary->linked_list);
    }
    return false;
}
