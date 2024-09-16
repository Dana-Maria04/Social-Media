#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "feed.h"
#include "users.h"
#include "graph.h"
#include "posts.h"

int is_in_linked_list(linked_list_t *list, int node) {
	ll_node_t *current = list->head;
	while (current) {
		if (*(int *)current->data == node)
			return 1;
		current = current->next;
	}
	return 0;
}

void
bron_kerbosch(linked_list_t *current_clique, linked_list_t *potential_nodes,
			  linked_list_t *excluded_nodes, list_graph_t *graph,
			  linked_list_t **max_clique, int start_node) {
	if (potential_nodes->size == 0 && excluded_nodes->size == 0) {
		/* Found a maximal clique */
		if (is_in_linked_list(current_clique, start_node) &&
			current_clique->size > (*max_clique)->size) {
			ll_free(max_clique);
			*max_clique = ll_create(sizeof(int));
			ll_node_t *node = current_clique->head;
			while (node) {
				ll_add_nth_node(*max_clique, (*max_clique)->size, node->data);
				node = node->next;
			}
		}
		return;
	}

	ll_node_t *node_iter = potential_nodes->head;
	while (node_iter) {
		int node = *(int *)node_iter->data;

		linked_list_t *new_clique = ll_create(sizeof(int));
		linked_list_t *new_potential_nodes = ll_create(sizeof(int));
		linked_list_t *new_excluded_nodes = ll_create(sizeof(int));

		/* Copying current_clique to new_clique */
		ll_node_t *current = current_clique->head;
		while (current) {
			ll_add_nth_node(new_clique, new_clique->size, current->data);
			current = current->next;
		}
		ll_add_nth_node(new_clique, new_clique->size, &node);

		/* Getting neighbors of the current node */
		linked_list_t *neighbors = lg_get_neighbours(graph, node);

		/* Filtering excluded_nodes into new_excluded_nodes */
		current = potential_nodes->head;
		while (current) {
			if (is_in_linked_list(neighbors, *(int *)current->data))
				ll_add_nth_node(new_potential_nodes, new_potential_nodes->size,
								current->data);
			current = current->next;
		}

		current = excluded_nodes->head;
		while (current) {
			if (is_in_linked_list(neighbors, *(int *)current->data))
				ll_add_nth_node(new_excluded_nodes, new_excluded_nodes->size,
								current->data);
			current = current->next;
		}
		/* Recursively determining the clique */
		bron_kerbosch(new_clique, new_potential_nodes, new_excluded_nodes,
					  graph, max_clique, start_node);

		/* Moving node from potential_nodes to excluded_nodes */
		ll_node_t *removed_node = ll_remove_nth_node(potential_nodes, 0);
		free(removed_node->data);
		free(removed_node);
		ll_add_nth_node(excluded_nodes, excluded_nodes->size, &node);

		ll_free(&new_clique);
		ll_free(&new_potential_nodes);
		ll_free(&new_excluded_nodes);

		node_iter = potential_nodes->head;
	}
}

void find_cliques(list_graph_t *graph, int start_node,
				  linked_list_t **max_clique) {
	linked_list_t *current_clique = ll_create(sizeof(int));
	linked_list_t *potential_nodes = ll_create(sizeof(int));
	linked_list_t *excluded_nodes = ll_create(sizeof(int));

	for (int i = 0; i < graph->nodes; ++i)
		ll_add_nth_node(potential_nodes, potential_nodes->size, &i);

	/* Starting the Bron-Kerbosch algorithm */
	bron_kerbosch(current_clique, potential_nodes, excluded_nodes, graph,
				  max_clique, start_node);

	/* Freeing the temporary lists */
	ll_free(&current_clique);
	ll_free(&potential_nodes);
	ll_free(&excluded_nodes);
}

void common_group(list_graph_t *graph, int user_id) {
	linked_list_t *max_clique = ll_create(sizeof(int));
	find_cliques(graph, user_id, &max_clique);

	/*Printing the maximal clique*/
	ll_node_t *node = max_clique->head;
	while (node) {
		printf("%s\n", get_user_name(*(int *)node->data));
		node = node->next;
	}

	ll_free(&max_clique);
}

void search_reposts(post *root, int id, list_graph_t *graph, int *ok)
{
	if (!root)
		return;
	if (lg_has_edge(graph, ((p_info *)root->info)->user_id, id))
		*ok = 1; /*Setting ok to 1 if there is an edge indicating a repost*/
	for (int i = 0; i < root->children_cnt; i++)
		search_reposts(root->children[i], id, graph, ok);
}

void get_reposts(post *root, int id, list_graph_t *graph)
{
	if (!root)
		return;
	if (lg_has_edge(graph, ((p_info *)root->info)->user_id, id))
		printf("%s\n", get_user_name(((p_info *)root->info)->user_id));
	for (int i = 0; i < root->children_cnt; i++)
		get_reposts(root->children[i], id, graph);
		/* Recursively getting reposts from children */
}

void traverse_post_tree(post *root, int id, char *title)
{
	if (!root)
		return;
	if (((p_info *)root->info)->user_id == id) {
		if (((p_info *)root->info)->title[0] == '\0')
			printf("Reposted: %s\n", title);
			/* Print reposted title if title is empty */
	}

	for (int i = 0; i < root->children_cnt; i++)
		traverse_post_tree(root->children[i], id, title);
}

void handle_feed(char *name1, int number, list_graph_t *graph,
				 post_array_t **post_array) {
	int id1 = get_user_id(name1);
	/* Iterating over posts from the most recent to the oldest */
	for (int i = (*post_array)->num_posts - 1; i >= 0 && number; i--) {
		if (!(*post_array)->posts[i] || !(*post_array)->posts[i]->root) {
			fprintf(stderr,
					"Error: Uninitialized post or root at index %d\n", i);
			continue;
		}
		int id = ((p_info *)(*post_array)->posts[i]->root->info)->user_id;
		/*Checking if the user follows the post author or is the author*/
		if (lg_has_edge(graph, id1, id) || id1 == id) {
			printf("%s: %s\n", get_user_name(id),
				   ((p_info *)(*post_array)->posts[i]->root->info)->title);
			number--;
		}
	}
}

void handle_view_profile(char *name1, post_array_t **post_array) {
	int id1 = get_user_id(name1);
	/*Printing posts created by the user*/
	for (int i = 0; i < (*post_array)->num_posts; i++) {
		if (((p_info *)(*post_array)->posts[i]->root->info)->user_id == id1) {
			printf("Posted: %s\n",
				   ((p_info *)(*post_array)->posts[i]->root->info)->title);
		}
	}
	/* Traversing the post tree to find and print reposts */
	for (int i = 0; i < (*post_array)->num_posts; i++) {
		p_info *post_info = (p_info *)(*post_array)->posts[i]->root->info;
		traverse_post_tree((*post_array)->posts[i]->root, id1,
						   post_info->title);
	}
}

void handle_friends_repost(char *name1, int nr, list_graph_t *graph,
						   post_array_t **post_array) {
	int id1 = get_user_id(name1);
	int poz;
	/* Finding the post with the given ID */
	for (int i = 0; i < (*post_array)->num_posts; i++) {
		if (((p_info *)(*post_array)->posts[i]->root->info)->id == nr) {
			poz = i;
			break;
		}
	}
	int ok = 0;
	/* Searching for reposts of the post by the user's friends */
	search_reposts((*post_array)->posts[poz]->root, id1, graph, &ok);
	if (ok)
		get_reposts((*post_array)->posts[poz]->root, id1, graph);
}

void handle_input_feed(char *input, list_graph_t *graph,
					   post_array_t **post_array)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name1 = strtok(NULL, "\n ");
		int number = atoi(strtok(NULL, "\n "));
		handle_feed(name1, number, graph, post_array);

	} else if (!strcmp(cmd, "view-profile")) {
		char *name1 = strtok(NULL, "\n ");
		handle_view_profile(name1, post_array);

	} else if (!strcmp(cmd, "friends-repost")) {
		char *name1 = strtok(NULL, "\n ");
		int nr = atoi(strtok(NULL, "\n "));
		handle_friends_repost(name1, nr, graph, post_array);

	} else if (!strcmp(cmd, "common-group")) {
		char *name1 = strtok(NULL, "\n ");
		int id1 = get_user_id(name1);
		printf("The closest friend group of %s is:\n", name1);
		common_group(graph, id1);
	}

	free(commands);
}
