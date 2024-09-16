#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "friends.h"
#include "users.h"
#include "graph.h"
#include "errno.h"

void suggestions(list_graph_t *graph)
{
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	int k = 0;
	if (graph->neighbors[id1]) {
		ll_node_t *node = graph->neighbors[id1]->head;
		/* Iterating through each friend of the user */
		while (node) {
			int id = *((int *)node->data);
			ll_node_t *friends = graph->neighbors[id]->head;
			/* Iterating through each friend of the friend */
			while (friends) {
				int friend_id = *(int *)friends->data;
				if (!lg_has_edge(graph, id1, friend_id) && id1 != friend_id)
					k++; /* Counting potential friends */
				friends = friends->next;
			}
			node = node->next;
		}
	}
	if (k == 0) {
		printf("There are no suggestions for %s", name1);
		printf("\n");
	} else {
		printf("Suggestions for %s:\n", name1);
		int v[MAX_PEOPLE] = {0};
		ll_node_t *node = graph->neighbors[id1]->head;
		while (node) {
			int id = *((int *)node->data);
			ll_node_t *friends = graph->neighbors[id]->head;
			while (friends) {
				int friend_id = *((int *)friends->data);
				/* Checking if the friend is a potential friend and
				not added yet */
				if (!lg_has_edge(graph, id1, friend_id) &&
					id1 != friend_id && !v[friend_id]) {
					v[friend_id] = 1; /* Marking the friend as suggested */
				}
				friends = friends->next;
			}
			node = node->next;
		}
		/* Printing all suggested friends */
		for (int i = 0; i < MAX_PEOPLE; i++)
			if (v[i])
				printf("%s\n", get_user_name(i));
	}
}

void popular(list_graph_t *graph)
{
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	unsigned int max = graph->neighbors[id1]->size;
	char name[281];
	strcpy(name, name1);
	if (graph->neighbors[id1]) {
		ll_node_t *node = graph->neighbors[id1]->head;
		for (unsigned int i = 0;
				i < graph->neighbors[id1]->size; i++) {
			int id = *((int *)node->data);
			/* Checking if this friend has more connections */
			if (graph->neighbors[id]->size > max) {
				max = graph->neighbors[id]->size;
				strcpy(name, get_user_name(id));
			}
			node = node->next;
		}
		node = graph->neighbors[id1]->head;
		if (strcmp(name, name1)) {
			node = graph->neighbors[id1]->head;
			/* Finding another friend with the same number of connections */
			for (unsigned int i = 0;
					i < graph->neighbors[id1]->size; i++) {
				int id = *((int *)node->data);
				if (graph->neighbors[id]->size == max) {
					strcpy(name, get_user_name(id));
					break;
				}
				node = node->next;
			}
		}
	}
	if (!strcmp(name, name1))
		printf("%s is the most popular\n", name);
	else
		printf("%s is the most popular friend of %s\n", name, name1);
}

void common(list_graph_t *graph)
{
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	char *name2 = strtok(NULL, "\n ");
	int id2 = get_user_id(name2);

	int k = 0;
	if (graph->neighbors[id1]) {
		ll_node_t *node = graph->neighbors[id1]->head;
		/* Iterating through each friend of the first user */
		while (node) {
			int id = *(int *)node->data;
			if (lg_has_edge(graph, id, id2))
				k++; /* Counting common friends */
			node = node->next;
		}
	}
	if (k == 0) {
		printf("No common friends for %s and %s", name1, name2);
		printf("\n");
	} else {
		printf("The common friends between %s and %s are:\n", name1, name2);
		ll_node_t *node = graph->neighbors[id1]->head;
		while (node) {
			if (lg_has_edge(graph, *((int *)node->data), id2)) {
				int id = *((int *)node->data);
				char *name_user = get_user_name(id);
				printf("%s\n", name_user); /* Printing common friend */
				k++;
			}
		node = node->next;
		}
	}
}

void add_connection(list_graph_t *graph) {
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	char *name2 = strtok(NULL, "\n ");
	int id2 = get_user_id(name2);
	/* Adding edge in both directions */
	lg_add_edge(graph, id1, id2);
	lg_add_edge(graph, id2, id1);
	printf("Added connection %s - %s\n", name1, name2);
}

void remove_connection(list_graph_t *graph) {
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	char *name2 = strtok(NULL, "\n ");
	int id2 = get_user_id(name2);
	/* Removing edge in both directions */
	lg_remove_edge(graph, id1, id2);
	lg_remove_edge(graph, id2, id1);
	printf("Removed connection %s - %s\n", name1, name2);
}

void distance(list_graph_t *graph) {
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	char *name2 = strtok(NULL, "\n ");
	int id2 = get_user_id(name2);
	/* Calculating distance */
	int distance = print_min_path(graph, id1, id2);
	if (!distance)
		printf("There is no way to get from %s to %s\n", name1, name2);
	else
		printf("The distance between %s - %s is %d\n", name1, name2, distance);
}

void friends(list_graph_t *graph) {
	char *name1 = strtok(NULL, "\n ");
	int id1 = get_user_id(name1);
	printf("%s has %d friends\n", name1, ll_get_size(graph->neighbors[id1]));
}

void handle_input_friends(char *input, list_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add"))
		add_connection(graph);
	else if (!strcmp(cmd, "remove"))
		remove_connection(graph);
	else if (!strcmp(cmd, "suggestions"))
		suggestions(graph);
	else if (!strcmp(cmd, "distance"))
		distance(graph);
	else if (!strcmp(cmd, "common"))
		common(graph);
	else if (!strcmp(cmd, "friends"))
		friends(graph);
	else if (!strcmp(cmd, "popular"))
		popular(graph);
	free(commands);
}
