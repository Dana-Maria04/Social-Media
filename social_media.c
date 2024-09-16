/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "graph.h"
/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();
	list_graph_t *graph = lg_create(MAX_PEOPLE);
	char *input = (char *)malloc(MAX_COMMAND_LEN);
	post_array_t *post_array = malloc(sizeof(post_array_t));
	post_array->num_posts = 0;
	post_array->total_posts = 0;
	post_array->posts = NULL;
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graph);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &post_array);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, graph, &post_array);
		#endif
	}
	#ifdef TASK_2
	free_post_array(&post_array);
	#endif
	#ifdef TASK_3
	free_post_array(&post_array);
	#endif
	free(post_array);
	lg_free(graph);
	free_users();
	free(input);
	return 0;
}
