#ifndef POSTS_H
#define POSTS_H

typedef struct p_info {
	int id;
	char title[281];
	int user_id;
	int *who_liked;
	int num_likes;

} p_info;

typedef struct post {
	struct post **children;
	int parent_id;
	int children_cnt;
	void *info;
} post;

typedef struct post_tree {
	post *root;
	int number_of_reposts;
	size_t data_size;
} post_tree;

typedef struct post_array_t {
	post_tree **posts;
	int num_posts;
	int total_posts;
} post_array_t;

/**
 * @brief Creates a new post node with the given details.
 *
 * This function allocates memory for a new post node, sets its initial values,
 * and assigns the provided information to the post node.
 *
 * @param name The name of the user creating the post.
 * @param title The title of the post.
 * @param id The unique identifier for the post.
 * @param ok_repost A flag indicating whether a repost is created or not
 * @param parent_id The unique identifier of the parent post.
 * @return A pointer to the newly created post node.
 */
post
*create_node(char *name, char *title, int id, int ok_repost, int parent_id);
/**
 * @brief Creates a new post tree structure.
 *
 * This function allocates memory for a new post tree structure and initializes
 * its values. The root of the tree is initially set to NULL.
 *
 * @param data_size The size of the data to be stored in the tree.
 * @return A pointer to the newly created post tree.
 */
post_tree *create_tree(size_t data_size);
/**
 * @brief Adds a new post to the array of posts.
 *
 * This function increments the post counts, reallocates memory for the posts
 * array if necessary, creates a new post tree, and assigns a root node to it.
 *
 * @param posts A pointer to the array of post arrays.
 * @param name The name of the user creating the post.
 * @param title The title of the post.
 * @return A pointer to the updated array of post arrays.
 */
post_array_t **create(post_array_t **posts, char *name, char *title);
/**
 * @brief Finds a post node by its ID in the post tree.
 *
 * This function recursively searches for a post node with the given ID in the
 * post tree starting from the root node.
 *
 * @param root The root node of the post tree.
 * @param post_id The ID of the post to find.
 * @return A pointer to the post node with the specified ID, or NULL if not
 * found.
 */
post *find_node_by_id(post *root, int post_id);
/**
 * @brief Finds a post by its ID in the post array.
 *
 * This function iterates through the post array to find a post with the
 * specified ID.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param id The ID of the post to find.
 * @return A pointer to the post with the specified ID, or NULL if not found.
 */
post *find_post_by_id(post_array_t **post_array, int id);
/**
 * @brief Handles the creation of a new post based on user input.
 *
 * This function extracts the user's name and the title of the post from the
 * input string, allocates memory for the title, creates a new post, and then
 * frees the allocated memory for the title.
 *
 * @param input The input string containing the user's name and post title.
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_create(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Finds the index of a post by its ID in the post array.
 *
 * This function iterates through the post array to find the index of a post
 * with the specified ID.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param id The ID of the post to find.
 * @return The index of the post with the specified ID, or -1 if not found.
 */
int find_post_index_by_id(post_array_t **post_array, int id);
/**
 * @brief Creates a repost for a specific post and updates the post array.
 *
 * This function finds the original post and the specific repost within that
 * post, creates a new repost node, and updates the repost count.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param name The name of the user creating the repost.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost within the original post.
 */
void
repost_repost(post_array_t **tree_of_posts, char *name, int post_id,
			  int repost_id);
/**
 * @brief Creates a repost for a specific post and updates the post array.
 *
 * This function finds the original post by its ID, increments the repost
 * count, creates a new repost node, and updates the children count and repost
 * count.
 *
 * @param posts Pointer to the array of post arrays.
 * @param name The name of the user creating the repost.
 * @param post_id The ID of the original post.
 */
void repost_post(post_array_t **tree_of_posts, char *name, int post_id);
/**
 * @brief Handles the creation of a repost based on user input.
 *
 * This function extracts the user's name and the post ID from the input
 * string, and decides whether to create a repost of a post or a repost of a
 * repost, based on the presence of a repost ID in the input.
 *
 * @param input The input string containing the user's name and post ID.
 * @param cmd The command string
 * @param post_array Pointer to the array of post arrays.
 */
void handle_repost(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Finds the lowest common ancestor (LCA) of two reposts in the post
 * tree.
 *
 * This function recursively searches for the lowest common ancestor of two
 * reposts with the specified IDs in the post tree starting from the root node.
 *
 * @param root The root node of the post tree.
 * @param repost_id_1 The ID of the first repost.
 * @param repost_id_2 The ID of the second repost.
 * @return A pointer to the lowest common ancestor post node, or NULL if not
 * found.
 */
post *common_repost(post *root, int repost_id_1, int repost_id_2);
/**
 * @brief Handles the identification of the lowest common ancestor of two
 * reposts.
 *
 * This function extracts the post ID and the two repost IDs from the input
 * string, finds the root post, and determines the lowest common ancestor of
 * the two reposts. It then prints the ID of the common repost.
 *
 * @param input The input string containing the post ID and repost IDs.
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_common_repost(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Handles liking or unliking a post by a user.
 *
 * This function finds the post by its ID, checks if the user has already liked
 * the post, and toggles the like status. If the post is already liked by the
 * user, it unlikes the post; otherwise, it likes the post.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param name The name of the user liking or unliking the post.
 * @param post_id The ID of the post to be liked or unliked.
 */
void like_post(post_array_t **post_array, char *name, int post_id);
/**
 * @brief Handles liking or unliking a repost by a user.
 *
 * This function finds the original post and the specific repost by their IDs,
 * checks if the user has already liked the repost, and toggles the like status
 * If the repost is already liked by the user, it unlikes the repost; otherwise
 * it likes the repost.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param name The name of the user liking or unliking the repost.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost to be liked or unliked.
 */
void
like_repost(post_array_t **post_array, char *name, int post_id, int repost_id);
/**
 * @brief Handles the liking or unliking of a post or repost based on user
 * input.
 *
 * This function extracts the user's name and post ID from the input string,
 * and determines whether to like or unlike a post or a repost based on the
 * presence of a repost ID in the input.
 *
 * @param input The input string containing the user's name and post ID.
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_like(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Determines if a post has been "ratio'd" by its reposts.
 *
 * This function iterates through the post array to find the original post by
 * its ID, then checks its reposts to see if any repost has more likes than
 * the original post. It prints whether the original post is the highest rated
 * or if it has been "ratio'd" by a repost.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param post_id The ID of the original post.
 */
void ratio_post(post_array_t **post_array, int post_id);
/**
 * @brief Handles the ratio check for a post based on user input.
 *
 * This function extracts the post ID from the input string and calls the
 * `ratio_post` function to determine if the post has been "ratio'd" by its
 * reposts.
 *
 * @param input The input string containing the post ID.
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_ratio(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Frees the memory allocated for a post tree.
 *
 * This function recursively frees the memory allocated for a post tree,
 * including its info.
 *
 * @param root Double pointer to the root node of the post tree.
 */
void free_post_tree(post **root);
/**
 * @brief Deletes a post and frees the associated memory.
 *
 * This function finds the post by its ID, prints a deletion message, frees
 * the memory allocated for the post tree, and updates the post array to
 * reflect the deletion.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param post_id The ID of the post to be deleted.
 */
void delete_post(post_array_t **post_array, int post_id);
/**
 * @brief Deletes a repost and marks it as deleted.
 *
 * This function finds the original post and the specific repost by their IDs,
 * prints a deletion message, and marks the repost as deleted by setting its
 * ID to -1.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost to be deleted.
 */
void delete_repost(post_array_t **post_array, int post_id, int repost_id);
/**
 * @brief Handles the deletion of a post or repost based on user input.
 *
 * This function extracts the post ID from the input string and determines
 * whether to delete a post or a repost based on the presence of a repost ID
 * in the input.
 *
 * @param input The input string containing the post ID and possibly a repost
 * ID.
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_delete(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Prints the number of likes for a given post.
 *
 * This function finds the post by its ID and prints the number of likes it
 * has.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param post_id The ID of the post.
 */
void get_likes_post(post_array_t **post_array, int post_id);
/**
 * @brief Prints the number of likes for a given repost.
 *
 * This function finds the original post and the specific repost by their IDs
 * and prints the number of likes the repost has.
 *
 * @param post_array Pointer to the array of post arrays.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost.
 */
void get_likes_repost(post_array_t **post_array, int post_id, int repost_id);
/**
 * @brief Handles the retrieval of likes for a post or repost based on user
 * input.
 *
 * This function extracts the post ID from the input string and determines
 * whether to get the likes for a post or a repost based on the presence of a
 * repost ID in the input.
 *
 * @param input The input string containing the post ID and possibly a repost
 * ID.
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_get_likes(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Recursively prints all reposts of a given post.
 *
 * This function iterates through the children of the given post node, prints
 * the repost information, and recursively calls itself to print the reposts
 * of the children.
 *
 * @param root Pointer to the root node of the post tree.
 */
void print_reposts(post *root);
/**
 * @brief Prints a post and all its reposts.
 *
 * This function finds the post by its ID, prints the post information, and
 * calls the `print_reposts` function to print all reposts of the given post.
 *
 * @param tree_of_posts Pointer to the array of post arrays.
 * @param post_id The ID of the post.
 */
void get_reposts_post(post_array_t **tree_of_posts, int post_id);
/**
 * @brief Prints a repost and all its sub-reposts.
 *
 * This function finds the original post and the specific repost by their IDs,
 * prints the repost information, and calls the `print_reposts` function to
 * print all sub-reposts of the given repost.
 *
 * @param posts Pointer to the array of post arrays.
 * @param post_id The ID of the original post.
 * @param repost_id The ID of the repost.
 */
void
get_reposts_repost(post_array_t **tree_of_posts, int post_id, int repost_id);
/**
 * @brief Handles the retrieval of reposts for a post or a repost based on
 * user input.
 *
 * This function extracts the post ID from the input string and determines
 * whether to get the reposts for a post or a repost based on the presence of a
 * repost ID in the input.
 *
 * @param input The input string containing the post ID and possibly a repost
 * ID
 * @param cmd The command string.
 * @param post_array Pointer to the array of post arrays.
 */
void handle_get_reposts(char *input, char *cmd, post_array_t **post_array);
/**
 * @brief Frees the memory allocated for the post array.
 *
 * This function frees the memory allocated for the post array by calling
 * the `FREE_POST_ARRAY` macro with the `free_post_tree` function to handle
 * the deallocation of each post tree.
 *
 * @param post_array Pointer to the array of post arrays.
 */
void free_post_array(post_array_t **post_array);
/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, post_array_t **post_array);
#endif // POSTS_H
