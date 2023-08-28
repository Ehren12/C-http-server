#ifndef MIME_TYPES_H
#define MIME_TYPES_H

struct MimeType
{
    char *key;
    char *value;

    struct MimeType *left, *right;
};

struct MimeType *initializeMime(char *key, char *value);
struct MimeType *addMime(struct MimeType *root, char *key, char *value);
struct MimeType *searchMime(struct MimeType *root, char *key);
void in_order_trav_mime(struct MimeType *root);
void dealloc_mimes(struct MimeType *node);

#endif