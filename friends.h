#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 550
#define MAX_PEOPLE 550
#include "graph.h"

/**
 * @brief Suggests new friends for a given user based on mutual friends.
 * This function identifies potential new friends for a user by examining
 * the friends of the user's friends. If a user is not already friends
 * with someone and they share a mutual friend, that person is suggested
 * as a new friend.
 * @param graph Pointer to the graph representing the social network.
 */
void suggestions(list_graph_t *graph);

/**
 * @brief Finds the most popular friend of a given user.
 *
 * This function determines which friend of the specified user has the
 * most connections (friends). If the user has friends, it compares the
 * number of friends each friend has and identifies the one with the
 * highest number of connections.
 * @param graph Pointer to the graph representing the social network.
 */
void popular(list_graph_t *graph);

/**
 * @brief Finds common friends between two users.
 * This function checks the list of friends for two given users and
 * identifies which friends they have in common. It then prints the
 * names of these common friends.
 * @param graph Pointer to the graph representing the social network.
 */
void common(list_graph_t *graph);

/**
 * @brief Adds a connection between two users.
 * This function adds an undirected edge between two users in the social
 * network graph, indicating that they are friends.
 * @param graph Pointer to the graph representing the social network.
 */
void add_connection(list_graph_t *graph);

/**
 * @brief Removes a connection between two users.
 * This function removes an undirected edge between two users in the social
 * network graph, indicating that they are no longer friends.
 * @param graph Pointer to the graph representing the social network.
 */
void remove_connection(list_graph_t *graph);

/**
 * @brief Finds the distance between two users in the graph.
 * This function calculates the shortest path between two users in the social
 * network graph and prints the distance. If no path exists, it indicates that
 * there is no way to get from one user to the other.
 * @param graph Pointer to the graph representing the social network.
 */
void distance(list_graph_t *graph);

/**
 * @brief Prints the number of friends a user has.
 * This function prints the number of friends (nodes connected) that a
 * specific user has in the social network graph.
 * @param graph Pointer to the graph representing the social network.
 */
void friends(list_graph_t *graph);

/**
 * @brief Handles input commands related to managing friendships.
 * This function processes a string input containing commands related
 * to adding or removing friends, suggesting friends, finding common
 * friends, checking the number of friends, and identifying popular
 * friends. It modifies the social network graph based on these commands.
 * @param input Command string input.
 * @param graph Pointer to the graph representing the social network.
 */
void handle_input_friends(char *input, list_graph_t *graph);

#endif // FRIENDS_H
