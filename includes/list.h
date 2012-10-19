#ifndef __LIST_H__
#define __LIST_H__

typedef struct		_list_s
{
  void			*e;
  struct _list_s	*prev;
  struct _list_s	*next;
} list_s;


list_s		*list_new(void *);
int32_t		list_insert(list_s **, list_s *);
int32_t		list_remove(list_s **, list_s *);

#endif /* !__LIST_H__ */
