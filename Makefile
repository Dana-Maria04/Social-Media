CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

.PHONY: all build clean

all: build

build: friends posts feed

# Any other object file that is needed for the other files
# add it here, separated by space
UTILS = users.o graph.o

friends: $(UTILS) friends.o social_media_friends.o
	$(CC) $(CFLAGS) -o $@ $^

posts: $(UTILS) posts.o social_media_posts.o
	$(CC) $(CFLAGS) -o $@ $^

feed: $(UTILS) posts.o friends.o feed.o social_media_feed.o
	$(CC) $(CFLAGS) -o $@ $^

social_media_friends.o: social_media.c graph.c users.c
	$(CC) $(CFLAGS) -c -D TASK_1 -o $@ social_media.c

social_media_posts.o: posts.c users.c
	$(CC) $(CFLAGS) -c -D TASK_2 -o $@ social_media.c

social_media_feed.o: social_media.c posts.c friends.c feed.c
	$(CC) $(CFLAGS) -c -D TASK_1 -D TASK_2 -D TASK_3 -o $@ social_media.c

clean:
	rm -rf *.o friends posts feed