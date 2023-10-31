compile_logic:  
	gcc -Wall logic.c linked_list.c hash_table.c inlupp2_tests.c common.c utils.c -g -lcunit -o inlupp2_tests

memtest_logic: 
	valgrind --leak-check=full ./inlupp2_tests

logic_run:
	gcc -Wall logic.c linked_list.c hash_table.c common.c utils.c -g -lcunit -o run
	./run < test.txt