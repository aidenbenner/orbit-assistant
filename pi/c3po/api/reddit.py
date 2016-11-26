import praw
import requests
import requests.auth 
import json 

my_user_agent = "linux:SE101_project:v1.0"
reddit = praw.Reddit(user_agent=my_user_agent)

"""
returns a string of post titles on r/worldnews pipe delimeted 
"""
def get_news (num_posts) 
    out_str = ""; 
    posts = reddit.get_subreddit('worldnews').get_hot(limit=num_posts)
    for post in posts: 
        out_str += post.title + "|" 
    return out_str 










