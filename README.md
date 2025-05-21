Problem Statement:  You have to design an algorithm to optimize the feed of a social networking platform with the  goal to display the most relevant posts to a user based on their interactions and preferences. 
Specifications:
1.	Create a network of users (students of your section (around 20 nodes)) where each student posts his doubts/reading material in different subjects (you can take 6 to 10 subjects) and expects someone else from the network will answer his/her question.  
2.	Each user can like , comment, share the solution or problem. These interactions should influence the relevance of posts 
3.	Each post has attributes such as the number of likes, comments, shares, and the time it was posted.
4.	Users have preferences based on the types of posts they interact with the most (e.g. posts with images, posts about specific topics).
5.	Feed Algorithm: The algorithm should prioritize posts based on: 
o	Posts that match the user's content preferences should be prioritized.
o	Newer posts should have higher priority.
o	Posts with more likes, comments, and shares should be prioritized.
o	Posts from users that the current user interacts with frequently should be prioritized.
Example:
•	User A likes posts about course 101 and frequently interacts with User B.
•	User B posts a new article related to course 101.
•	User C, who User A follows but interacts with less frequently, posts a question from another course.
•	The algorithm should prioritize User B's post over User C's post in User A's feed.
Solution Approach:
•	Data Collection: Gather data on user interactions, post attributes, and user preferences.
•	Scoring Function: Develop a scoring function that assigns a relevance score to each post based on the specified criteria.
•	Sorting: Sort the posts in the user's feed based on their relevance scores.
•	Personalization: Continuously update the scoring function based on the user's changing preferences and interactions.
