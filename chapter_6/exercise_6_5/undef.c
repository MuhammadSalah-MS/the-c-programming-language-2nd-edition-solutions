#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 101

enum boolean
{
  FALSE,
  TRUE
};

struct list_node
{
  char *name;
  char *definition;
  struct list_node *next;
};

// There is a strdup available with POSIX, but it's not part of ISO C.
char *str_dup(char *src);

size_t hash(char *s);
struct list_node *lookup(char *s);
struct list_node *install(char *name, char *definition);
enum boolean undef(char *name);

static struct list_node *hash_table[HASH_SIZE];

int main(void)
{
  install("TEST", "test");

  // Install other collision values for the same hash as for "TEST" -> 51.
  install("TSHe", "test");
  install("UPXD", "test");
  install("9iww", "test");
  install("mY1a", "test");
  install("uuoT", "test");

  struct list_node *node_p = lookup("TEST");
  if (node_p == NULL)
  {
    puts("Error: hash value not found.");
  }
  else
  {
    printf("%s %s\n", node_p->name, node_p->definition);
    if (undef("UPXD") && lookup("UPXD") == NULL)
    {
      printf("'%s' was undefined successfully.\n", "TEST");
    }
    else
    {
      printf("Error: failed to undefine '%s'.\n", "TEST");
    }
  }

  return EXIT_SUCCESS;
}

char *str_dup(char *src)
{
  char *dest = (char *)malloc(strlen(src) + 1);
  if (dest != NULL)
  {
    strcpy(dest, src);
  }
  return dest;
}

size_t hash(char *s)
{
  size_t hash_value = 0;
  while (*s != '\0')
  {
    hash_value = *s + 31 * hash_value;
    ++s;
  }
  return hash_value % HASH_SIZE;
}

struct list_node *lookup(char *s)
{
  struct list_node *node_p;
  for (node_p = hash_table[hash(s)]; node_p != NULL; node_p = node_p->next)
  {
    if (strcmp(s, node_p->name) == 0)
    {
      return node_p;
    }
  }
  return NULL;
}

struct list_node *install(char *name, char *definition)
{
  struct list_node *node_p;
  if ((node_p = lookup(name)) == NULL)
  {
    node_p = (struct list_node *)malloc(sizeof(*node_p));
    if (node_p == NULL || (node_p->name = str_dup(name)) == NULL)
    {
      return NULL;
    }
    size_t hash_value = hash(name);
    node_p->next = hash_table[hash_value];
    hash_table[hash_value] = node_p;
  }
  else
  {
    free(node_p->definition);
  }

  if ((node_p->definition = str_dup(definition)) == NULL)
  {
    return NULL;
  }

  return node_p;
}

enum boolean undef(char *name)
{
  struct list_node *node_p;
  struct list_node *prev_node_p;
  for (node_p = hash_table[hash(name)], prev_node_p = NULL;
       node_p != NULL;
       prev_node_p = node_p, node_p = node_p->next)
  {
    if (strcmp(name, node_p->name) == 0)
    {
      free(node_p->name);
      free(node_p->definition);

      if (prev_node_p == NULL)
      {
        hash_table[hash(name)] = node_p->next;
      }
      else
      {
        prev_node_p->next = node_p->next;
      }

      free(node_p);
      return TRUE;
    }
  }

  return FALSE;
}
