#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

static GList * list;

int main()
{
	char *str1 = "str1", *str2 = "str2";
	list = g_list_append(list, str1);
	list = g_list_append(list, str2);
	printf("list=%p, len = %d\n", list, g_list_length(list));
	list = g_list_remove(list, str1);
	list = g_list_remove(list, str2);
	printf("list=%p, len = %d\n", list, g_list_length(list));
	return 0;
}
