#ifndef FEED_H
#define FEED_H
#include "graph.h"
#include "posts.h"
/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
/**
 * @brief Checks if a node is in a linked list.
 * This function iterates through the linked list to determine if the specified
 * node is present. It returns 1 if the node is found and 0 otherwise.
 * @param list The linked list to search.
 * @param node The node to find.
 * @return int 1 if the node is found, 0 otherwise.
 */
int is_in_linked_list(linked_list_t *list, int node);

/**
 * @brief Implements the Bron-Kerbosch algorithm to find maximal cliques.
 * This function uses the Bron-Kerbosch recursive algorithm to find all maximal
 * cliques in a given graph. It updates the largest clique found so far
 * (`max_clique`) if the current clique is larger. The function considers nodes
 * in the potential_nodes list and excludes those in the excluded_nodes list.
 *
 * @param current_clique The current clique being evaluated.
 * @param potential_nodes Nodes that can potentially be part of the clique.
 * @param excluded_nodes Nodes that are excluded from the current clique.
 * @param graph The graph to search for cliques.
 * @param max_clique The largest clique found so far.
 * @param start_node The starting node for the clique search.
 */
void
bron_kerbosch(linked_list_t *current_clique, linked_list_t *potential_nodes,
			  linked_list_t *excluded_nodes, list_graph_t *graph,
			  linked_list_t **max_clique, int start_node);

/**
 * @brief Searches for reposts of a specific post by a user in the graph.
 *
 * This function recursively searches through the tree of posts to check if a
 * specific user has reposted any post. It sets the value pointed to by `ok` to
 * 1 if a repost by the user is found.
 * @param root The root post to start the search.
 * @param id The user ID to search for reposts.
 * @param graph The graph containing user connections.
 * @param ok Pointer to an int that indicates if a repost is found.
 */
void search_reposts(post *root, int id, list_graph_t *graph, int *ok);

/**
 * @brief Finds the largest clique in the graph starting from a specific node.
 *
 * This function initializes the necessary data structures and calls the
 * Bron-Kerbosch algorithm to find the largest clique that includes the
 * `start_node`. The result is stored in `max_clique`.
 * @param graph The graph to search for cliques.
 * @param start_node The starting node for the clique search.
 * @param max_clique Pointer to the largest clique found.
 */
void find_cliques(list_graph_t *graph, int start_node,
				  linked_list_t **max_clique);
/**
 * @brief Finds the closest friend group (maximal clique) for a user.
 * This function identifies the largest group of users (maximal clique) that
 * the specified user is part of. It prints the user names of all members of
 * this clique.
 * @param graph The graph containing user connections.
 * @param user_id The user ID to find the friend group for.
 */
void common_group(list_graph_t *graph, int user_id);

/**
 * @brief Prints all reposts of a specific post by a user.
 * This function traverses the tree of posts to find and print all reposts made
 * by a specific user. It is used to track how a post has spread through the
 * network.
 * @param root The root post to start the search.
 * @param id The user ID to search for reposts.
 * @param graph The graph containing user connections.
 */
void get_reposts(post *root, int id, list_graph_t *graph);

/**
 * @brief Traverses a post tree to find and print posts by a specific user.
 * This function traverses the tree of posts starting from the `root` to find
 * all posts made by a specific user. If the user's post is a repost, it prints
 * the original post's title.
 * @param root The root post to start the traversal.
 * @param id The user ID to search for posts.
 * @param title The title of the root post.
 */
void traverse_post_tree(post *root, int id, char *title);

/**
 * @brief Handles the feed command to display posts for a user.
 * This function handles the feed command by displaying a specified number of
 * posts for a given user. It checks if the user has an edge in the graph or if
 * the user is the author of the post.
 * @param name1 The name of the user requesting the feed.
 * @param number The number of posts to display.
 * @param graph The graph containing user connections.
 * @param post_array Array of posts to search through.
 */
void handle_feed(char *name1, int number, list_graph_t *graph,
				 post_array_t **post_array);

/**
 * @brief Handles the view-profile command to display a user's posts.
 * This function handles the view-profile command by displaying all posts made
 * by a specific user. It also traverses the post tree to display reposts.
 * @param name1 The name of the user whose profile is being viewed.
 * @param post_array Array of posts to search through.
 */
void handle_view_profile(char *name1, post_array_t **post_array);

/**
 * @brief Handles the friends-repost command to display friends' reposts.
 * This function handles the friends-repost command by displaying all reposts
 * of a specific post made by a user's friends.
 * @param name1 The name of the user requesting the reposts.
 * @param nr The ID of the post to check for reposts.
 * @param graph The graph containing user connections.
 * @param post_array Array of posts to search through.
 */
void handle_friends_repost(char *name1, int nr, list_graph_t *graph,
						   post_array_t **post_array);
/**
 * @brief Handles input commands related to the feed and user interactions.
 * This function processes various commands related to the user's feed and
 * interactions. It can handle commands like "feed", "view-profile",
 * "friends-repost", and "common-group". The appropriate action is taken based
 * on the command and the data structures are updated or queried accordingly.
 * @param input The input command string.
 * @param graph The graph containing user connections.
 * @param post_array Pointer to the array of posts.
 */
void handle_input_feed(char *input, list_graph_t *graph,
					   post_array_t **post_array);

#endif // FEED_H
