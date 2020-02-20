#include "editor_buffer.h"
#include <stdio.h>
#include <check.h>
#include <assert.h>


typedef struct DLinkedList DLinkedList;

void affiche(DLinkedList *list)
{
	if (list->current == NULL)
	{
		printf("List vide.\n");
		return;
	}
	struct Node *first = list->current;
	while(first->previous != NULL)
		first = first->previous;
	printf("value: %c\n", *((char *)first->data));
	while(first->next != NULL)
	{	
		first = first->next;
		printf("value: %c\n", *((char *)first->data));
	}
}

void affiche2(DLinkedList *list)
{
	if (list->current == NULL)
	{
		printf("List vide.\n");
		return;
	}
	struct Node *first = list->first;
	while(first != NULL)
	{	
		if (first == list->current)
			printf("[%c] ", *((char *)first->data));
		else 
			printf("%c ", *((char *)first->data));
		first = first->next;
	}
	printf("\n");
}

void affiche_current_stats(DLinkedList *list)
{
	struct Node *cur = list->current;
	printf("----*----\n");
	if (cur == NULL)
	{
		printf("List Vide\n");
		printf("----*----\n");
		return;
	}
	if (cur->previous != NULL)
		printf("previous value: %c\n", *((char *)cur->previous->data));
	else
		printf("previous value: NULL\n");
	printf("current value : %c\n", *((char *)cur->data));
	if (cur->next != NULL)
		printf("next value    : %c\n", *((char *)cur->next->data));
	else 
		printf("next value    : NULL\n");
	printf("----*----\n");
}

void affiche_stats(DLinkedList *list)
{
	if (list->current == NULL)
	{
		printf("List vide.\n");
		return;
	}
	struct Node *first = list->current;
	while(first->previous != NULL)
		first = first->previous;
	affiche_current_stats(list);
	while(first->next != NULL)
	{	
		first = first->next;
		affiche_current_stats(list);
	}
}

void print_line_num(struct editor_buffer* buff)
{
	struct DLinkedList *list = buff->lines;
	if (list->current == NULL)
	{
		printf("List vide.\n");
		return;
	}
	struct Node *first = list->first;
	int count = 0;
	while(first != NULL)
	{	
		count++;
		if (first == list->current)
			printf("[%d] \n" ,count);
		else 
			printf("%d \n", count);
		first = first->next;
	}
	printf("\n");
}

START_TEST(dlinkedlist_test) {
	DLinkedList *list = new_DLinkedList();
	add_previous(list,&ascii['1']);
	ck_assert_int_eq(get_size(list), 1);

	add_next(list,&ascii['2']);
	ck_assert_int_eq(get_size(list), 2);

	add_previous(list,&ascii['3']);
	ck_assert_int_eq(get_size(list), 3);

	go_previous(list);
	ck_assert_int_eq(get_size(list), 3);

	add(list, &ascii['4']);
	ck_assert_int_eq(get_size(list), 4);

	add(list, &ascii['5']);
	ck_assert_int_eq(get_size(list), 5);

	add(list, &ascii['6']);
	ck_assert_int_eq(get_size(list), 6);

	add(list, &ascii['7']);
	ck_assert_int_eq(get_size(list), 7);
	
	go_previous(list);
	go_previous(list);
	ck_assert_int_eq(get_size(list), 7);
	
	rm_current(list);
	ck_assert_int_eq(get_size(list), 6);

	rm_current(list);
	ck_assert_int_eq(get_size(list), 5);

	rm_next(list);
	ck_assert_int_eq(get_size(list), 4);

	rm_previous(list);
	ck_assert_int_eq(get_size(list), 4);

	DLinkedList * list2 = new_DLinkedList();
	add(list2, &ascii['7']);
	add(list2, &ascii['7']);
	add(list2, &ascii['7']);
	concat(&list, &list2);
	ck_assert_int_eq(get_size(list), 7);

	free_DLinkedList(list);
	free_DLinkedList(list2);
}
END_TEST

START_TEST (buff_test) {
	struct editor_buffer *buff = new_buffer();

	add_char(buff, 'G');
	ck_assert_int_eq(get_size(buff->chars), 1);
	ck_assert_int_eq(get_size(buff->lines), 1);

	add_char(buff, 'G');
	ck_assert_int_eq(get_size(buff->chars), 2);
	ck_assert_int_eq(get_size(buff->lines), 1);

	add_char(buff, '1');
	ck_assert_int_eq(get_size(buff->chars), 3);
	ck_assert_int_eq(get_size(buff->lines), 1);

	cur_left(buff);
	cur_left(buff);
	cur_left(buff);

	add_char(buff, '2');
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 1);

	rm_char(buff);
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 1);

	add_char(buff, '3');
	ck_assert_int_eq(get_size(buff->chars), 5);
	ck_assert_int_eq(get_size(buff->lines), 1);

	add_char(buff, '4');
	ck_assert_int_eq(get_size(buff->chars), 6);
	ck_assert_int_eq(get_size(buff->lines), 1);

	add_line(buff);
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 2);

	add_line(buff);
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 3);

	add_line(buff);
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 4);

	add_line(buff);
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 5);

	rm_char(buff);
	ck_assert_int_eq(get_size(buff->chars), 4);
	ck_assert_int_eq(get_size(buff->lines), 4);

	cur_up(buff);

	add_char(buff, '4');
	ck_assert_int_eq(get_size(buff->chars), 1);
	ck_assert_int_eq(get_size(buff->lines), 4);

	cur_up(buff);
	ck_assert_int_eq(get_size(buff->chars), 0);
	ck_assert_int_eq(get_size(buff->lines), 4);

	cur_up(buff);
	cur_down(buff);
	cur_down(buff);
	cur_left(buff);
	cur_left(buff);
	cur_left(buff);
	cur_left(buff);

	rm_char(buff);
	ck_assert_int_eq(get_size(buff->chars), 2);
	ck_assert_int_eq(get_size(buff->lines), 4);

	cur_right(buff);
	cur_right(buff);
	cur_right(buff);
	cur_right(buff);

	rm_char(buff);
	ck_assert_int_eq(get_size(buff->chars), 1);
	ck_assert_int_eq(get_size(buff->lines), 3);

	free_buff(buff);
}
END_TEST

Suite *dll_suite (void) {
	TCase *tc_list, *tc_buff;
	Suite *s;

	tc_list = tcase_create("List");
	tc_buff = tcase_create("Buffer");
	tcase_add_test(tc_list, dlinkedlist_test);
	tcase_add_test(tc_buff, buff_test);

	s = suite_create("DLinkedList");
	suite_add_tcase(s, tc_list);
	suite_add_tcase(s, tc_buff);
	return s;
}

int main()
{

	int number_failed;
    SRunner* sr;
	Suite* s_ddl = dll_suite();

	sr = srunner_create(s_ddl);
	srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}