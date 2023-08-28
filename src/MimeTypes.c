#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "MimeTypes.h"

struct MimeType *initializeMime(char *key, char *value)
{
    struct MimeType *node = (struct MimeType *)malloc(sizeof(struct MimeType));
    node->key = key;
    node->value = value;

    node->left = node->right = NULL;
    return node;
}

struct MimeType *addMime(struct MimeType *root, char *key, char *value)
{
    if (root == NULL)
    {
        return initializeMime(key, value);
    }

    if (strcmp(key, root->key) == 0)
    {
        printf("This MimeType exsists\n");
    }
    else if (strcmp(key, root->key) < 0)
    {
        root->left = addMime(root->left, key, value);
    }
    else if (strcmp(key, root->key) > 0)
    {
        root->right = addMime(root->right, key, value);
    }

    return root;
}

struct MimeType *searchMime(struct MimeType *root, char *key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (strcmp(root->key, key) == 0)
    {
        return root;
    }
    else if (strcmp(root->key, key) < 0) // Changed from > 0 to < 0
    {
        return searchMime(root->right, key);
    }
    else if (strcmp(root->key, key) > 0)
    {
        return searchMime(root->left, key);
    }
}

void in_order_trav_mime(struct MimeType *root)
{
    if (root != NULL)
    {
        in_order_trav_mime(root->left);
        printf("\n%s ==> %s\n", root->key, root->value);
        in_order_trav_mime(root->right);
    }
}

void dealloc_mimes(struct MimeType *node)
{
    if (node == NULL)
        return;

    dealloc_mimes(node->left);
    dealloc_mimes(node->right);
    free(node);
}