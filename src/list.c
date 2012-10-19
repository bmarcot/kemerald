#include "types.h"
#include "kernel.h"
#include "list.h"
#include "stdlib.h"
#include "mmu.h"

list_s		*list_new(void		*e)
{
  list_s	*new;

  /* allocate memory for the list node */
  if ((e == NULL) || ((new = malloc(sizeof (list_s))) == NULL))
    return NULL;

  /* set the new node element */
  new->e = e;

  /* initialize links */
  new->prev = NULL;
  new->next = NULL;

  return new;
}

int32_t		list_insert(list_s	**list,
			    list_s	*new)
{
  list_s	*iterator;

  /* argument checking */
  if ((list == NULL) || (new == NULL))
    {
      return ERR_UNKNOWN;
    }

  /* rebuild the double linked-list links */
  if (*list != NULL)
    {
      iterator = *list;
      while (iterator->next)
	iterator = iterator->next;
      iterator->next = new;
      new->prev = iterator;
      new->next = NULL;
    }
  else
    {
      *list = new;
      new->prev = NULL;
      new->next = NULL;
    }

  return ERR_NONE;
}

int32_t		list_remove(list_s	**list,
			    list_s	*rem)
{
  /* arguments checking */
  if ((list == NULL) || (*list == NULL) || (rem == NULL))
    {
      return ERR_UNKNOWN;
    }

  if (rem->prev == NULL)
    {
      /* if the list contains only one element, else remove the first node */
      if (rem->next == NULL)
	{
	  *list = NULL;
	}
      else
	{
	  rem->next->prev = NULL;
	  *list = rem->next;
	}
    }
  else
    {
      /* if removing the last node from the list, else remove a central node */
      if (rem->next == NULL)
	{
	  rem->prev->next = NULL;
	}
      else
	{
	  rem->prev->next = rem->next;
	  rem->next->prev = rem->prev;
	}
    }

  /* reset the removed node links */
  rem->prev = NULL;
  rem->next = NULL;

  return ERR_NONE;
}






/* list_s		*list_insert(list_s	**list, */
/* 			     void	*e) */
/* { */
/*   list_s	*new; */
/*   list_s	*iterator; */

/*   /\* allocate memory for the list node *\/ */
/*   if ((e == NULL) || ((new = malloc(sizeof(list_s))) == NULL)) */
/*     return NULL; */

/*   /\* set the new node element *\/ */
/*   new->e = e; */

/*   /\* the new element cannot have a next element (insert_at_last) *\/ */
/*   new->next = NULL; */

/*   /\* rebuild the double linked-list links *\/ */
/*   if (*list != NULL) */
/*     { */
/*       iterator = *list; */
/*       while (iterator->next) */
/* 	iterator = iterator->next; */
/*       iterator->next = new; */
/*       new->prev = iterator; */
/*     } */
/*   else */
/*     { */
/*       *list = new; */
/*       new->prev = NULL; */
/*     } */

/*   return new; */
/* } */
