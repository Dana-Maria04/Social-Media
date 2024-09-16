#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

#define ALLOC_AND_COPY_TITLE(input, cmd, name, title)                  \
	do {                                                               \
		const char *_input = (input);                                  \
		const char *_cmd = (cmd);                                      \
		const char *_name = (name);                                    \
		char **_title = &(title);                                      \
		int start = strlen(_name) + strlen(_cmd) + 2;                  \
		int finish = strlen(_input) - 1;                               \
		*_title = malloc((finish - start + 1) * sizeof(char));         \
		if (!*_title) {                                                \
			perror("Failed to allocate memory for title");             \
			exit(EXIT_FAILURE);                                        \
		}                                                              \
		for (int i = start; i < finish; i++)                           \
			(*_title)[i - start] = _input[i];                          \
		(*_title)[finish - start] = '\0';                              \
	} while (0)

#define EXTRACT_AND_CONVERT_TO_INT(var, delim)                         \
	do {                                                               \
		char *str = strtok(NULL, delim);                               \
		var = atoi(str);                                               \
	} while (0)

#define ALLOCATE_MEMORY(ptr, size)                                     \
	do {                                                               \
		void **_ptr = (void **)&(ptr);                                 \
		*_ptr = malloc(size);                                          \
		if (!*_ptr) {                                                  \
			perror("Failed to allocate memory");                       \
			exit(EXIT_FAILURE);                                        \
		}                                                              \
	} while (0)

#define ALLOCATE_CALLOC(ptr, count, size)                              \
	do {                                                               \
		void **_ptr = (void **)&(ptr);                                 \
		*_ptr = calloc(count, size);                                   \
		if (!*_ptr) {                                                  \
			perror("Failed to allocate memory");                       \
			exit(EXIT_FAILURE);                                        \
		}                                                              \
	} while (0)

#define FREE_POST_ARRAY(post_array, free_post_tree)                    \
	do {                                                               \
		post_array_t **_post_array = (post_array);                     \
		if (!(_post_array) || !*(_post_array) || !(*(_post_array))->num_posts) \
			break;                                                     \
		for (int i = 0; i < (*(_post_array))->num_posts; i++) {        \
			if ((*(_post_array))->posts[i]) {                          \
				free_post_tree(&((*(_post_array))->posts[i]->root));   \
				free((*(_post_array))->posts[i]);                      \
			}                                                          \
		}                                                              \
		free((*(_post_array))->posts);                                 \
		free(*(_post_array));                                          \
		*(_post_array) = NULL;                                         \
	} while (0)

post
*create_node(char *name, char *title, int id, int ok_repost, int parent_id)
{
	post *p;
	ALLOCATE_MEMORY(p, sizeof(post));
	ALLOCATE_MEMORY(p->info, sizeof(p_info));
	/* Initializaing the elements from p_info structure */
	((p_info *)p->info)->user_id = get_user_id(name);
	((p_info *)p->info)->who_liked = calloc(550, sizeof(int));
	((p_info *)p->info)->id = id;
	((p_info *)p->info)->num_likes = 0;
	p->parent_id = parent_id;
	ALLOCATE_MEMORY(p->children, 100 * sizeof(post *));
	p->children_cnt = 0;
	if (!ok_repost)
		strcpy(((p_info *)p->info)->title, title);
	else
		((p_info *)p->info)->title[0] = '\0';
	return p;
}

post_tree *create_tree(size_t data_size)
{
	post_tree *new_tree = malloc(sizeof(post_tree));
	new_tree->root = NULL;
	new_tree->data_size = data_size;
	new_tree->number_of_reposts = 0;
	return new_tree;
}

post_array_t **create(post_array_t **posts, char *name, char *title)
{
	(*posts)->num_posts++;
	(*posts)->total_posts++;
	int n_posts = (*posts)->num_posts;
	/* Checking if I have posts */
	if (n_posts == 1) {
		(*posts)->posts = malloc(1 * sizeof(post_tree *));
	} else {
		post_tree **tree2 =
		realloc((*posts)->posts, (n_posts) * sizeof(post_tree));
		(*posts)->posts = tree2;
	}
	(*posts)->posts[n_posts - 1] = create_tree(sizeof(post));
	(*posts)->posts[n_posts - 1]->root =
	create_node(name, title, (*posts)->total_posts, 0, -1);
	printf("Created %s for %s\n", title, name);
	return posts;
}

post *find_node_by_id(post *root, int post_id)
{
	if (((p_info *)root->info)->id == post_id)
		return root;

	for (int i = 0; i < root->children_cnt; i++) {
		if (!root->children[i])
			continue;
		post *found = find_node_by_id(root->children[i], post_id);
		if (found)
			return found;
	}
	return NULL;
}

post *find_post_by_id(post_array_t **post_array, int id) {
	post *root = NULL;
	int i = 0;
	while (i < (*post_array)->num_posts) {
		if (((p_info *)(*post_array)->posts[i]->root->info)->id == id) {
			root = (*post_array)->posts[i]->root;
			break;
		}
		i++;
	}

	return root;
}

void handle_create(char *input, char *cmd, post_array_t **post_array)
{
	char *name = strtok(NULL, "\n ");
	char *title;
	ALLOC_AND_COPY_TITLE(input, cmd, name, title);
	post_array = create(post_array, name, title);
	free(title);
}

int find_post_index_by_id(post_array_t **post_array, int id) {
	int i = 0;
	/* Iterating through the vector of posts to look for a specific one with
	the given id*/
	while (i < (*post_array)->num_posts) {
		if (((p_info *)(*post_array)->posts[i]->root->info)->id == id)
			return i;
		i++;
	}
	return -1;
}

void
repost_repost(post_array_t **post_array, char *name, int post_id,
			  int repost_id)
{
	int new_post_id = 0;
	post *root = find_post_by_id(post_array, post_id);
	post *found = find_node_by_id(root, repost_id);
	/* Updating the number of posts/new_post_id */
	new_post_id = ++(*post_array)->total_posts;
	int i = find_post_index_by_id(post_array, post_id);
	(*post_array)->posts[i]->number_of_reposts++;
	/* Creating a new child */
	found->children[found->children_cnt] =
	create_node(name, NULL, new_post_id, 1,
				((p_info *)found->info)->id);
	found->children_cnt++;
	printf("Created repost #%d for %s\n", new_post_id, name);
}

void repost_post(post_array_t **posts, char *name, int post_id)
{
	int new_post_id = 0, root_id = 0, repost_nr = 0;
	root_id = find_post_index_by_id(posts, post_id);
	/* Adding the new node */
	repost_nr = (*posts)->posts[root_id]->root->children_cnt;
	(*posts)->posts[root_id]->root->children_cnt++;
	(*posts)->posts[root_id]->number_of_reposts++;
	new_post_id = ++(*posts)->total_posts;
	printf("Created repost #%d for %s\n", new_post_id, name);
	(*posts)->posts[root_id]->root->children[repost_nr] =
	create_node(name, NULL, new_post_id, 1, post_id);
}

void handle_repost(char *input, char *cmd, post_array_t **post_array)
{
	(void)input;
	(void)cmd;
	char *name = strtok(NULL, "\n ");
	int post_id;
	EXTRACT_AND_CONVERT_TO_INT(post_id, "\n ");
	char *repost_id_str = strtok(NULL, "\n ");
	if (!repost_id_str) {
		repost_post(post_array, name, post_id);
	} else {
		int repost_id = atoi(repost_id_str);
		repost_repost(post_array, name, post_id, repost_id);
	}
}

post *common_repost(post *root, int repost_id_1, int repost_id_2)
{
	/* If we didn't find anythong we return NULL */
	if (!root)
		return NULL;

	if (((p_info *)root->info)->id == repost_id_1 ||
		((p_info *)root->info)->id == repost_id_2)
		return root;

	int count = 0;
	post *temp_lca = NULL;
	/* Iterating through the tree */
	for (int i = 0; i < root->children_cnt; i++) {
		post *lca = common_repost(root->children[i], repost_id_1, repost_id_2);
		if (lca) {
			count++;
			temp_lca = lca;
		}
	}

	if (count == 2)
		return root;
	/* Found the lowest common ancestor */
	return temp_lca;
}

void handle_common_repost(char *input, char *cmd, post_array_t **post_array)
{
	(void)input;
	(void)cmd;
	int post_id, repost_id_1, repost_id_2;

	EXTRACT_AND_CONVERT_TO_INT(post_id, "\n ");
	EXTRACT_AND_CONVERT_TO_INT(repost_id_1, "\n ");
	EXTRACT_AND_CONVERT_TO_INT(repost_id_2, " ");
	post *root = find_post_by_id(post_array, post_id);
	/* Finding the lca */
	post *lca = common_repost(root, repost_id_1, repost_id_2);
	printf("The first common repost of %d and %d is %d\n",
		   repost_id_1, repost_id_2, ((p_info *)lca->info)->id);
}

void like_post(post_array_t **post_array, char *name, int post_id)
{
	post *root = find_post_by_id(post_array, post_id);
	/* Checking if the post is already liked */
	if (((p_info *)root->info)->who_liked[get_user_id(name)] == 1) {
		printf("User %s unliked post %s\n", name,
			   ((p_info *)root->info)->title);
		((p_info *)root->info)->who_liked[get_user_id(name)] = 0;
		((p_info *)root->info)->num_likes--;
		return;
	}
	/* Printing who liked the post */
	printf("User %s liked post %s\n", name, ((p_info *)root->info)->title);
	((p_info *)root->info)->who_liked[get_user_id(name)] = 1;
	((p_info *)root->info)->num_likes++;
}

void
like_repost(post_array_t **post_array, char *name, int post_id, int repost_id)
{
	post *root = find_post_by_id(post_array, post_id);
	post *found = find_node_by_id(root, repost_id);
	/* Checking if the repost is already liked */
	if (((p_info *)found->info)->who_liked[get_user_id(name)] == 1) {
		printf("User %s unliked repost %s\n", name,
			   ((p_info *)root->info)->title);
		((p_info *)found->info)->who_liked[get_user_id(name)] = 0;
		((p_info *)found->info)->num_likes--;
		return;
	}

	printf("User %s liked repost %s\n", name, ((p_info *)root->info)->title);
	((p_info *)found->info)->who_liked[get_user_id(name)] = 1;
	((p_info *)found->info)->num_likes++;
}

void handle_like(char *input, char *cmd, post_array_t **post_array)
{
	(void)input;
	(void)cmd;
	char *name = strtok(NULL, " ");
	int post_id;
	EXTRACT_AND_CONVERT_TO_INT(post_id, " ");
	char *repost_id_str = strtok(NULL, " ");
	if (!repost_id_str) {
		like_post(post_array, name, post_id);
	} else {
		int repost_id = atoi(repost_id_str);
		like_repost(post_array, name, post_id, repost_id);
	}
}

void ratio_post(post_array_t **post_array, int post_id)
{
	for (int i = 0; i < (*post_array)->num_posts; i++) {
		if (((p_info *)(*post_array)->posts[i]->root->info)->id == post_id) {
			post *root = (*post_array)->posts[i]->root;
			int max_likes = ((p_info *)root->info)->num_likes;
			int max_id = ((p_info *)root->info)->id;
			for (int j = 0; j < root->children_cnt; j++) {
				if (!root->children[j])
					continue;
				/* Checking if a repost has more likes than the original
				post */
				if (((p_info *)root->children[j]->info)->num_likes >
					max_likes) {
					max_likes = ((p_info *)root->children[j]->info)->num_likes;
					max_id = ((p_info *)root->children[j]->info)->id;
				}
			}
			if (max_id == ((p_info *)root->info)->id)
				printf("The original post is the highest rated\n");
			else
				printf("Post %d got ratio'd by repost %d\n", post_id, max_id);
			return;
		}
	}
}

void handle_ratio(char *input, char *cmd, post_array_t **post_array)
{
	(void)cmd;
	(void)input;
	int post_id;
	EXTRACT_AND_CONVERT_TO_INT(post_id, "\n ");
	ratio_post(post_array, post_id);
}

void free_post_tree(post **root)
{
	for (int i = 0; i < (*root)->children_cnt; i++) {
		if (!(*root)->children[i])
			continue;
		free_post_tree(&((*root)->children[i]));
	}

	free(((p_info *)(*root)->info)->who_liked);
	free((*root)->info);
	free((*root)->children);
	free(*root);
}

void delete_post(post_array_t **post_array, int post_id)
{
	post *root = find_post_by_id(post_array, post_id);
	printf("Deleted %s\n", ((p_info *)root->info)->title);
	int index = find_post_index_by_id(post_array, post_id);
	free_post_tree(&root);
	free((*post_array)->posts[index]);
	(*post_array)->num_posts--;
}

void delete_repost(post_array_t **post_array, int post_id, int repost_id)
{
	post *root = find_post_by_id(post_array, post_id);
	post *found = find_node_by_id(root, repost_id);
	printf("Deleted repost #%d of post %s\n",
		   ((p_info *)found->info)->id,
		   ((p_info *)root->info)->title);
	/* Marking the fact that the repost was deleted */
	((p_info *)found->info)->id = -1;
}

void handle_delete(char *input, char *cmd, post_array_t **post_array)
{
	(void)input;
	(void)cmd;
	int post_id;
	EXTRACT_AND_CONVERT_TO_INT(post_id, " ");
	char *repost_id_str = strtok(NULL, "\n ");
	if (!repost_id_str) {
		delete_post(post_array, post_id);
	} else {
		int repost_id = atoi(repost_id_str);
		delete_repost(post_array, post_id, repost_id);
	}
}

void get_likes_post(post_array_t **post_array, int post_id)
{
	post *root = find_post_by_id(post_array, post_id);
	printf("Post %s has %d likes\n", ((p_info *)root->info)->title,
		   ((p_info *)root->info)->num_likes);
}

void get_likes_repost(post_array_t **post_array, int post_id, int repost_id)
{
	post *root = find_post_by_id(post_array, post_id);
	post *found = find_node_by_id(root, repost_id);
	printf("Repost #%d has %d likes\n", ((p_info *)found->info)->id,
		   ((p_info *)found->info)->num_likes);
}

void handle_get_likes(char *input, char *cmd, post_array_t **post_array)
{
	(void)input;
	(void)cmd;
	int post_id;
	EXTRACT_AND_CONVERT_TO_INT(post_id, " ");
	char *repost_id_str = strtok(NULL, "\n ");
	if (!repost_id_str) {
		get_likes_post(post_array, post_id);
	} else {
		int repost_id = atoi(repost_id_str);
		get_likes_repost(post_array, post_id, repost_id);
	}
}

void print_reposts(post *root)
{
	for (int i = 0; i < root->children_cnt; i++) {
		if (((p_info *)root->children[i]->info)->id == -1)
			continue;
		printf("Repost #%d by %s\n", ((p_info *)root->children[i]->info)->id,
			   get_user_name(((p_info *)root->children[i]->info)->user_id));
		print_reposts(root->children[i]);
	}
}

void get_reposts_post(post_array_t **tree_of_posts, int post_id)
{
	post *root = find_post_by_id(tree_of_posts, post_id);
	printf("%s - Post by %s\n", ((p_info *)root->info)->title,
		   get_user_name(((p_info *)root->info)->user_id));
	print_reposts(root);
}

void
get_reposts_repost(post_array_t **posts, int post_id, int repost_id)
{
	post *root = find_post_by_id(posts, post_id);
	post *found = find_node_by_id(root, repost_id);
	printf("Repost #%d by %s\n", ((p_info *)found->info)->id,
		   get_user_name(((p_info *)found->info)->user_id));
	print_reposts(found);
}

void handle_get_reposts(char *input, char *cmd, post_array_t **post_array)
{
	(void)input;
	(void)cmd;
	int post_id;
	EXTRACT_AND_CONVERT_TO_INT(post_id, "\n ");
	char *repost_id_str = strtok(NULL, "\n ");
	if (!repost_id_str) {
		get_reposts_post(post_array, post_id);
	} else {
		int repost_id = atoi(repost_id_str);
		get_reposts_repost(post_array, post_id, repost_id);
	}
}

void free_post_array(post_array_t **post_array) {
	FREE_POST_ARRAY(post_array, free_post_tree);
}

void handle_input_posts(char *input, post_array_t **post_array)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "create"))
		handle_create(input, cmd, post_array);
	else if (!strcmp(cmd, "repost"))
		handle_repost(input, cmd, post_array);
	else if (!strcmp(cmd, "common-repost"))
		handle_common_repost(input, cmd, post_array);
	else if (!strcmp(cmd, "like"))
		handle_like(input, cmd, post_array);
	else if (!strcmp(cmd, "ratio"))
		handle_ratio(input, cmd, post_array);
	else if (!strcmp(cmd, "delete"))
		handle_delete(input, cmd, post_array);
	else if (!strcmp(cmd, "get-likes"))
		handle_get_likes(input, cmd, post_array);
	else if (!strcmp(cmd, "get-reposts"))
		handle_get_reposts(input, cmd, post_array);
	free(commands);
}
