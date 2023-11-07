compile_logic:  
	gcc -Wall logic.c linked_list.c hash_table.c inlupp2_tests.c common.c utils.c -g -lcunit -o inlupp2_tests

memtest_logic: 
	valgrind --leak-check=full ./inlupp2_tests

logic_run:
	gcc -Wall logic.c linked_list.c hash_table.c common.c utils.c -g -lcunit -o run
	./run

logic_run_valgrind:
	gcc -Wall logic.c linked_list.c hash_table.c common.c utils.c -g -lcunit -o run
	valgrind --leak-check=full ./run

store_run_valgrind:
	gcc -Wall logic.c ui.c store.c linked_list.c hash_table.c common.c utils.c -g -lcunit -o store
	valgrind --leak-check=full ./store