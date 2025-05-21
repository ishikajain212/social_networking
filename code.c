#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_SUBJECTS 6
#define MAX_TEXT_LEN 256
#define W_PREF     2.0
#define W_RECENCY  3.0
#define W_ENGAGE   1.5
#define W_INTERACT 2.5
#define CURRENT_TIME 2025

const char *subjects[NUM_SUBJECTS] = {"Math", "Science", "Sports", "Tech", "Movies", "News"};

const char *subjectKeywords[NUM_SUBJECTS][3] = {
    {"math", "algebra", "calculus"},
    {"science", "physics", "chemistry"},
    {"sports", "football", "cricket"},
    {"tech", "ai", "robot"},
    {"movies", "film", "cinema"},
    {"news", "politics", "world"}
};

typedef struct Post {
    int postId;
    int authorId;
    char text[MAX_TEXT_LEN];
    int subject;
    int likes, comments, shares;
    int timestamp;
} Post;

typedef struct PostNode {
    Post post;
    struct PostNode *next;
} PostNode;

typedef struct User {
    int userId;
    int preferences[NUM_SUBJECTS];
    int interactions[100];  // up to 100 users
} User;

typedef struct UserNode {
    User user;
    struct UserNode *next;
} UserNode;

UserNode *userHead = NULL;
PostNode *postHead = NULL;

// Convert string to lowercase
void toLower(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

// Detect subject from text using keywords
int detectSubjectFromText(char *text) {
    char tempText[MAX_TEXT_LEN];
    strcpy(tempText, text);
    toLower(tempText);
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        for (int j = 0; j < 3; j++) {
            if (strstr(tempText, subjectKeywords[i][j])) {
                return i;
            }
        }
    }
    return NUM_SUBJECTS - 1; // default to last subject (News)
}

// Add user to linked list
void addUser(User user) {
    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
    newNode->user = user;
    newNode->next = userHead;
    userHead = newNode;
}

// Add post to linked list
void addPost(Post post) {
    PostNode *newNode = (PostNode *)malloc(sizeof(PostNode));
    newNode->post = post;
    newNode->next = postHead;
    postHead = newNode;
}

// Compute score of post for a user
double computeScore(User *user, Post *post) {
    double score = 0.0;
    if (user->preferences[post->subject])
        score += W_PREF;
    double recency = 1.0 / (CURRENT_TIME - post->timestamp + 1);
    score += W_RECENCY * recency;
    int engagement = post->likes + post->comments + post->shares;
    score += W_ENGAGE * engagement;
    score += W_INTERACT * user->interactions[post->authorId];
    return score;
}

// Sort posts by score using bubble sort (simple)
void sortFeed(PostNode **headRef, User *user) {
    int swapped;
    PostNode *ptr1;
    PostNode *lptr = NULL;

    if (*headRef == NULL) return;

    do {
        swapped = 0;
        ptr1 = *headRef;

        while (ptr1->next != lptr) {
            if (computeScore(user, &ptr1->post) < computeScore(user, &ptr1->next->post)) {
                Post temp = ptr1->post;
                ptr1->post = ptr1->next->post;
                ptr1->next->post = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Read users from CSV
void readUsers(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open user file");
        exit(1);
    }
    char line[1024];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        User u;
        char *token = strtok(line, ",");
        u.userId = atoi(token);
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            token = strtok(NULL, ",");
            u.preferences[i] = atoi(token);
        }
        for (int i = 0; i < 100; i++) {
            token = strtok(NULL, ",");
            if (token)
                u.interactions[i] = atoi(token);
            else
                u.interactions[i] = 0;
        }
        addUser(u);
    }
    fclose(fp);
}

// Read posts from CSV
void readPosts(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open post file");
        exit(1);
    }
    char line[1024];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        Post p;
        char *token = strtok(line, ",");
        p.postId = atoi(token);
        token = strtok(NULL, ",");
        p.authorId = atoi(token);
        token = strtok(NULL, ",");
        strncpy(p.text, token, MAX_TEXT_LEN);
        p.text[strcspn(p.text, "\n")] = 0;  // remove newline
        p.subject = detectSubjectFromText(p.text);
        token = strtok(NULL, ",");
        p.likes = atoi(token);
        token = strtok(NULL, ",");
        p.comments = atoi(token);
        token = strtok(NULL, ",");
        p.shares = atoi(token);
        token = strtok(NULL, ",");
        p.timestamp = atoi(token);
        addPost(p);
    }
    fclose(fp);
}

User *findUserById(int id) {
    UserNode *temp = userHead;
    while (temp) {
        if (temp->user.userId == id)
            return &temp->user;
        temp = temp->next;
    }
    return NULL;
}

void displayFeed(User *user) {
    PostNode *feed = postHead;
    sortFeed(&feed, user);
    printf("\n---- Personalized Feed for User %d ----\n", user->userId);
    PostNode *temp = feed;
    while (temp) {
        double score = computeScore(user, &temp->post);
        printf("PostID: %d | Author: %d | Subject: %s | Score: %.2f\n", 
            temp->post.postId, temp->post.authorId,
            subjects[temp->post.subject], score);
        printf("Text: %s\nLikes: %d, Comments: %d, Shares: %d, Timestamp: %d\n\n",
            temp->post.text, temp->post.likes, temp->post.comments, temp->post.shares, temp->post.timestamp);
        temp = temp->next;
    }
}

int main() {
    readUsers("G:/DAA/Contest_Project/users_data - Copy.csv");
    readPosts("G:/DAA/Contest_Project/posts_data - Copy.csv");

    int uid;
    printf("Enter your user ID: ");
    scanf("%d", &uid);
    User *u = findUserById(uid);
    if (!u) {
        printf("User ID %d not found.\n", uid);
        return 1;
    }
    displayFeed(u);
    return 0;
}
