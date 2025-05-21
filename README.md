# Social Networking Feed Optimization

## Problem Statement
Design an algorithm to optimize the feed of a social networking platform with the goal to display the most relevant posts to a user based on their interactions and preferences.

## Specifications

1. **User Network**  
   Create a network of users (e.g., students from your section, around 20 nodes). Each user posts doubts or reading materials across different subjects (6 to 10 subjects). Users expect responses from others in the network.

2. **User Interactions**  
   Each user can **like**, **comment**, and **share** posts (both problems and solutions). These interactions influence the relevance of the posts.

3. **Post Attributes**  
   Each post includes:
   - Number of likes
   - Number of comments
   - Number of shares
   - Time posted

4. **User Preferences**  
   Users have preferences based on the type of posts they interact with most (e.g., posts with images, posts about specific topics).

5. **Feed Algorithm Prioritization**  
   The algorithm prioritizes posts based on:  
   - Matching the user's content preferences  
   - Recency of the post (newer posts prioritized)  
   - Popularity of the post (likes, comments, shares)  
   - Frequency of interaction with the post's author  

## Example

- User A likes posts about Course 101 and frequently interacts with User B.  
- User B posts a new article related to Course 101.  
- User C, who User A follows but interacts less with, posts a question from another course.  

The algorithm should prioritize User B's post over User C's post in User A's feed.

## Solution Approach

- **Data Collection:**  
  Collect data on user interactions, post attributes, and user preferences.

- **Scoring Function:**  
  Develop a scoring function that assigns a relevance score to each post based on the criteria above.

- **Sorting:**  
  Sort posts in the user's feed according to their relevance scores.

- **Personalization:**  
  Continuously update the scoring function based on the user's evolving preferences and interactions.

---

## Technologies & Tools

- Programming language: (e.g., Python, C++, Java)
- Data structures: Graphs for user network
- Algorithms: Scoring and sorting algorithms for feed optimization
- Optional: Database to store posts and interactions

---

