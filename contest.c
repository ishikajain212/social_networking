/*
 * feed_algorithm.c - Updated Version
 * Reads data from CSV files ("users_data.csv" and "posts_data.csv")
 * Computes relevance scores and sorts posts using quicksort.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 /* Constants */
 #define MAX_USERS 20
 #define MAX_POSTS 10
 #define NUM_SUBJECTS 6
 #define INTERACT_SIZE MAX_USERS  // Interaction frequency array size
 #define MAX_LINE_LENGTH 1024  // Buffer size for reading CSV lines
 
 /* Scoring weight parameters */
 #define W_PREF     2.0
 #define W_RECENCY  3.0
 #define W_ENGAGE   1.5
 #define W_INTERACT 2.5
 
 /* Data structures */
 typedef struct {
     int userId;
     int preferences[NUM_SUBJECTS];  // P0 - P5
     int interactions[INTERACT_SIZE];  // I0 - I19
 } User;
 
 typedef struct {
     int postId;
     int authorId;
     int subject;
     int likes;
     int comments;
     int shares;
     int timestamp;
 } Post;
 
 /* Global data storage */
 User users[MAX_USERS];
 Post posts[MAX_POSTS];
 
 /* Function to compute relevance score */
 double computeScore(User *user, Post *post, int currentTime) {
     double score = 0.0;
 
     // 1. Content Preference
     if (user->preferences[post->subject] == 1) {
         score += W_PREF;
     }
 
     // 2. Recency: Higher score for newer posts
     double recency = 1.0 / (currentTime - post->timestamp + 1);
     score += W_RECENCY * recency;
 
     // 3. Engagement Score
     int engagement = post->likes + post->comments + post->shares;
     score += W_ENGAGE * engagement;
 
     // 4. Interaction Score
     score += W_INTERACT * user->interactions[post->authorId];
 
     return score;
 }
 
 /* Swap two Post pointers */
 void swap(Post **a, Post **b) {
     Post *temp = *a;
     *a = *b;
     *b = temp;
 }
 
 /* Quicksort partition function */
 int partition(Post **arr, int low, int high, User *user, int currentTime) {
     double pivotScore = computeScore(user, arr[high], currentTime);
     int i = low - 1;
     
     for (int j = low; j < high; j++) {
         double scoreJ = computeScore(user, arr[j], currentTime);
         if (scoreJ > pivotScore) {
             i++;
             swap(&arr[i], &arr[j]);
         }
     }
     swap(&arr[i + 1], &arr[high]);
     return i + 1;
 }
 
 /* Custom quicksort */
 void quickSort(Post **arr, int low, int high, User *user, int currentTime) {
     if (low < high) {
         int pi = partition(arr, low, high, user, currentTime);
         quickSort(arr, low, pi - 1, user, currentTime);
         quickSort(arr, pi + 1, high, user, currentTime);
     }
 }
 
 /* Read users data from CSV file */
 void readUsersData(const char *filename) {
     FILE *fp = fopen(filename, "r");
     if (!fp) {
         perror("Error opening users data file");
         exit(EXIT_FAILURE);
     }
 
     char line[MAX_LINE_LENGTH];
     fgets(line, sizeof(line), fp);  // Skip header
 
     int i = 0;
     while (fgets(line, sizeof(line), fp) && i < MAX_USERS) {
         char *token = strtok(line, ",");
         users[i].userId = atoi(token);
 
         // Read preferences (P0 - P5)
         for (int j = 0; j < NUM_SUBJECTS; j++) {
             token = strtok(NULL, ",");
             users[i].preferences[j] = atoi(token);
         }
 
         // Read interactions (I0 - I19)
         for (int j = 0; j < INTERACT_SIZE; j++) {
             token = strtok(NULL, ",");
             users[i].interactions[j] = atoi(token);
         }
 
         i++;
     }
 
     fclose(fp);
 }
 
 /* Read posts data from CSV file */
 void readPostsData(const char *filename) {
     FILE *fp = fopen(filename, "r");
     if (!fp) {
         perror("Error opening posts data file");
         exit(EXIT_FAILURE);
     }
 
     char line[MAX_LINE_LENGTH];
     fgets(line, sizeof(line), fp);  // Skip header
 
     int i = 0;
     while (fgets(line, sizeof(line), fp) && i < MAX_POSTS) {
         char *token = strtok(line, ",");
         posts[i].postId = atoi(token);
 
         token = strtok(NULL, ",");
         posts[i].authorId = atoi(token);
 
         token = strtok(NULL, ",");
         posts[i].subject = atoi(token);
 
         token = strtok(NULL, ",");
         posts[i].likes = atoi(token);
 
         token = strtok(NULL, ",");
         posts[i].comments = atoi(token);
 
         token = strtok(NULL, ",");
         posts[i].shares = atoi(token);
 
         token = strtok(NULL, ",");
         posts[i].timestamp = atoi(token);
 
         i++;
     }
 
     fclose(fp);
 }
 
 /* Main function */
 /* Updated Main Function (User Input Version) */
int main() {
    int currentTime = 2025;
    int targetUserId;  // Variable to store the desired user ID

    // Read users and posts from CSV files
    readUsersData("users_data.csv");
    readPostsData("posts_data.csv");

    // Ask the user which profile they want to see
    printf("Enter User ID to generate feed (0-%d): ", MAX_USERS - 1);
    scanf("%d", &targetUserId);

    // Find the requested user
    User *currentUser = NULL;
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].userId == targetUserId) {
            currentUser = &users[i];
            break;
        }
    }
    if (currentUser == NULL) {
        fprintf(stderr, "User with ID %d not found.\n", targetUserId);
        exit(EXIT_FAILURE);
    }

    // Rest of the code remains the same...
    Post *sortedPosts[MAX_POSTS];
    for (int i = 0; i < MAX_POSTS; i++) {
        sortedPosts[i] = &posts[i];
    }

    quickSort(sortedPosts, 0, MAX_POSTS - 1, currentUser, currentTime);

    printf("\nFeed for User %d (sorted by relevance score):\n", currentUser->userId);
    for (int i = 0; i < MAX_POSTS; i++) {
        double score = computeScore(currentUser, sortedPosts[i], currentTime);
        printf("PostID: %d | Author: %d | Subject: %d | Likes: %d | Comments: %d | Shares: %d | Timestamp: %d | Score: %.2f\n",
               sortedPosts[i]->postId,
               sortedPosts[i]->authorId,
               sortedPosts[i]->subject,
               sortedPosts[i]->likes,
               sortedPosts[i]->comments,
               sortedPosts[i]->shares,
               sortedPosts[i]->timestamp,
               score);
    }

    return 0;
}
 