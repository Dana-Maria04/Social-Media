# Social-Media

Homework 3 SDA team:
tiberiu.gutanu 311CA
dana_maria.caruntu 311CA

#### SOCIAL NETWORK - HOMEWORK 3

#### TASK 1 : FRIENDS NETWORK MANAGMENT

This project implements a simple friends network management system,
allowing users to perform various operations such as
- `add` : adding friends
- `remove` : removing friends
- `suggestions` : suggesting new friends
- `distance` : finding distance between users
- `common` : finding common friends
- `friends` : counting how many friends there are
- `popular` : identifying the most popular friend.

#### Concepts used:
- *data stuctures* :

1. **Graph** : Represents the network with nodes as users and edges as
friendships.
2. **Adjacency List** : Efficient for sparse graphs, allowing fast traversal.

- *algorithms* :

1. **BFS** : Used to calculate the shortest distance between two users.
2. **Iteration**: Used for suggesting friends and finding common friends.

#### Steps Taken:

1. **Graph Representation** : Implemented using an adjacency list.
2. **Command Parsing** : Used strtok for parsing commands.
3. **Add/Remove Operations** : Managed bidirectional connections.
4. **Distance Calculation** : Implemented BFS.
5. **Suggestions** : Iterated over friends of friends.
6. **Common Friends** : Checked for mutual friends.
7. **Popularity Check** : Compared number of connections.

##### --------- COMMANDS -----------

##### `add`

This command creates a mutual connection between two users. Both users
will have each other listed as friends.

When you use the `add` command and type in two names, it’s like you’re
helping two people meet and become friends.

The app makes sure each person is recognized as unique, then updates
their friend lists so each knows about the other.Following this
identification process, each user's list of connections is updated to
include the other.

##### `remove`

Deletes the existing friendship link between two specified users.

Upon receiving the command, the project retrieves the identifiers associated
with the given names and proceeds to remove each user from the other's
list of friends. This operation is direct and efficient.

##### `distance`

Finds the minimum number of friendships (edges) between two users.

The command takes two names, converts them into their respective IDs, and
employs a Breadth-First Search (BFS) algorithm. This search method starts
at one user and radiates outwards, layer by layer, until it either locates
the second user or exhausts all connections. The number of layers traversed
during this search represents the social distance between the two individuals.

##### `suggestions`

Proposes non-friend second-degree connections as potential friends.

This command offers a glimpse into potential friendships by identifying
friends of friends who are not yet directly connected to the user.

After determining the central user’s ID, the project examines the
friends of this user’s friends, compiling a list of unique individuals.

##### `common`

Lists friends common to both specified users.

If you want to see who you and another person both know, you can use the
`common` command. The app looks at both your friend lists and tells you who’s
on both.

After retrieving the lists of friends for both users, the code finds
the intersection of these lists, revealing shared connections

##### `friends`

Reports the number of direct connections a user has.

This command provides a straightforward count of a user’s direct connections.
This count is quickly retrieved from the user’s adjacency list, offering a
snapshot of their social involvement within the network.

##### `popular`

Identifies the friend with the highest number of connections from a user’s
friend list.

The `popular` command seeks to identify the most connected individual within
a user’s circle. By examining the number of connections each friend has, the
project determines who among them is the most influential or, in other words,
the most popular. In cases of a tie, the system favors either the user
themselves or the first friend listed.

### TASK 2 POSTS MANAGMENT

#### Concepts used:
- *data stuctures* :

1. **Generic tree** : Represents the network with root as post and children as
reposts.

- *algorithms* :

1. **Lowest common ancestor** : Used to calculate the lowest common repost for
two reposts.
2. **Iteration through the tree**: Used for accesing the values from the nodes.

##### --------- FUNCTIONS USED -----------

### `create_node`

This function creates a new post node with the given parameters and initializes
its properties. The function allocates memory for a new post structure and its
p_info sub-structure. The user_id field is initialized with the user ID
obtained by calling get_user_id function with the provided name. The who_liked
field is initialized as an array of integers with a size of 550. The id, num_likes,
parent_id fields are also initialized with the provided values. If ok_repost
is 0, the title is copied to the title field of the structure.
Otherwise, the title field is set to an empty string.
Finally, the function returns a pointer to the newly created post node.

### `create_tree`

This function creates a new post tree node with the given data size.
The root of the tree is initialized to None, and the number of reposts is
set to 0.

### `create`

This function creates a new post with the given name and title, and adds it
to the post array.
If this is the first post, the post array is initialized with a size of 1.
The root of the new post is created by calling the `create_node` function.

### `find_node_by_id`

This function finds the post node with the given post ID in the post tree.
If the post ID matches the root node, it returns the root node.
If the post ID matches one of the root's children, it recursively calls
`find_node_by_id` on that child.

### `find_post_by_index`

This function finds the post with the given ID in the post array.
It iterates through the post array and returns the root of the post tree with
the given ID.

### `handle_create`

This function handles the creation of a new post. It extracts the name and
title from the input string, creates a new post with the given name and title,
and adds it to the post array.

### `find_post_index_by_id`

This function finds the index of the post with the given ID in the post array.
It iterates through the post array and returns the index of the post with the
given ID.

### `repost_repost`

This function handles the reposting of an existing post. It finds the post with
the given post ID and the repost with the given repost ID, updates the number
of reposts for the post, and creates a new child node for the repost with the
new post ID.

### `repost_post`

This function creates a new repost for the post with the given ID. It finds the
post in the post array, adds a new child node to the post's root node, and
increments the number of reposts for the post.

### `handle_repost`

This function handles the reposting of a post or repost. It extracts the name
and post ID from the input string, and creates a new repost for the post with
the given ID. If the input string contains a repost ID, it creates a repost for
that repost instead.

### `common_repost`

This function finds the lowest common reposter between two reposts in the post
tree. It recursively searches the tree for the two reposts, and returns the
lowest node that contains both reposts.

### `handle_common_repost`

This function handles the finding of the lowest common reposter between two
reposts. It extracts the post ID and repost IDs from the input string, finds
the post in the post array, and calls common_repost to find the lowest
common reposter.

### `like_post`

This function handles the liking of a post. It finds the post in the post array,
checks if the user has already liked the post, and updates the number of likes
and the user's like status accordingly.

### `like_repost`

This function handles the liking of a repost. It finds the post in the post
array, finds the repost in the post tree, checks if the user has already
liked the repost, and updates the number of likes and the user's like status
accordingly.

### `handle_like`

This function handles the liking of a post or repost. It extracts the name,
post ID, and optionally repost ID from the input string, and calls like_post
or like_repost accordingly.

### `ratio_post`

This function finds the post with the given ID in the post array, and checks if
any of its reposts have more likes than the original post. If a repost has more
likes, it prints a message indicating that the post has been ratio'd by the
repost.

### `handle_ratio`

This function handles the finding of the ratio of a post. It extracts the post
ID from the input string, finds the post in the post array, and calls ratio_post
to find the ratio.

### `free_post_tree`

This function frees the memory used by a post tree. It recursively frees the
child nodes, the who_liked array, the info struct, the children array, and the
root node.

### `delete_post`

This function deletes a post from the post array. It finds the post in the
post array, prints a message indicating that the post has been deleted,
finds the index of the post in the post array, frees the memory used by
the post tree, frees the memory used by the post node, and decrements the
number of posts in the post array.

### `delete_repost`

This function deletes a repost from a post. It finds the post and repost in
the post array, prints a message indicating that the repost has been deleted,
marks the repost as deleted, and updates the number of reposts for the post.

### `handle_delete`

This function handles the deletion of a post or repost. It extracts the post ID
and optionally repost ID from the input string, and calls delete_post or
delete_repost accordingly.

### `get_likes_post`

This function finds the post in the post array, and prints the number of likes
for the post.

### `get_likes_repost`

This function finds the post and repost in the post array, and prints the
number of likes for the repost.

### `handle_get_likes`

This function handles the getting of the number of likes for a post or repost.
It extracts the post ID and optionally repost ID from the input string, and
calls get_likes_post or get_likes_repost accordingly.

### `print_reposts`

This function prints the reposts of a post. It iterates through the children
of the root node, and if the repost has not been deleted, it prints the repost
and recursively calls print_reposts for the repost.

### `get_reposts_post`

This function finds the post in the post array, prints the title and user of
the post, and calls print_reposts to print the reposts of the post.

### `get_reposts_repost`

This function finds the post and repost in the post array, prints the title
and user of the repost, and calls print_reposts to print the reposts of the
repost.

### `handle_get_reposts`

This function handles the getting of the reposts of a post or repost. It
extracts the post ID and optionally repost ID from the input string, and
calls get_reposts_post or get_reposts_repost accordingly.

### `free_post_array`

This function frees the memory used by the post array. It recursively frees the
child nodes, the who_liked array, the info struct, the children array, and the
root node.

### TASK 3 FEED MANAGMENT

#### Concepts used:
- *data stuctures* :

1. **Generic tree** : Represents the network with root as post and children as
reposts.
2. **Graph** : Represents the network with nodes as users and edges as
friendships.
3. **Adjacency List** : Efficient for sparse graphs, allowing fast traversal.

- *algorithms* :

1. **Bron Kerbosch** : Used to compute the maximal clique a user is in.
2. **Iteration through the tree**: Used for accesing the values from the nodes.

##### --------- FUNCTIONS USED -----------

### `is_in_linked_list`

This function checks if a given node is present in a linked list. It takes a
linked list and an integer node as input and returns 1 if the node is found in
the list, and 0 otherwise.

### `bron_kerbosch`

This function implements the Bron-Kerbosch algorithm to find maximal cliques in
a graph. It takes a current clique, potential nodes, excluded nodes, a graph,
a pointer to a maximum clique, and a start node as input. It recursively
determines the clique and updates the maximum clique if a larger clique is
found.

### `find_cliques`

This function finds the maximal cliques in a graph using the Bron-Kerbosch
algorithm. It takes a graph, a start node, and a pointer to a maximum clique as
input. It initializes the current clique, potential nodes, and excluded nodes,
and then calls the bron_kerbosch function to find the maximal cliques.

### `common_group`

This function finds the closest friend group of a user in a graph.
It takes a graph, a user ID, and a pointer to a maximum clique as input.
It calls the find_cliques function to find the maximal cliques and then
prints the maximal clique with the largest size.

### `search_reposts`

This function searches for reposts of a given post by a user's friends in a
graph. It takes a root post, a user ID, a graph, and a pointer to an integer
as input. It recursively searches for reposts of the post by the user's
friends and sets the integer to 1 if a repost is found.

### `get_reposts`

This function prints the reposts of a given post by a user's friends in a
graph. It takes a root post, a user ID, and a graph as input. It
recursively prints the reposts of the post by the user's friends.

### `traverse_post_tree`

This function traverses a post tree to find and print reposts of a given post
by a user. It takes a root post, a user ID, and a title as input. It
recursively traverses the post tree and prints the reposted title if the title
is empty.

### `handle_feed`

This function handles the "feed" command to display the posts of a user and
their friends. It takes a user name, a number of posts to display, a graph,
and a pointer to a post array as input. It iterates over the posts from the
most recent to the oldest and displays the posts of the user and their friends.

### `handle_view_profile`

This function handles the "view-profile" command to display the posts created
by a user. It takes a user name and a pointer to a post array as input.
It iterates over the posts and displays the posts created by the user and their
reposts.

### `handle_friends_repost`

This function handles the "friends-repost" command to display the reposts of a
given post by a user's friends. It takes a user name, a post ID, a graph, and a
pointer to a post array as input. It searches for reposts of the post by the
user's friends and displays the reposts.

### `handle_input_feed`

This function handles the input for the feed, view-profile, and friends-repost
commands. It takes an input string, a graph, and a pointer to a post array as
input. It parses the input string and calls the appropriate function based on
the command.
