import praw

user_agent = 'linux:dank_memes:v4.20'
reddit = praw.Reddit(user_agent=user_agent)


def get_news(options):
    """
    options:
        - name of the subreddit. eg. worldnews
        - number of posts. eg. 10

    returns: eg.
        [
            {title: 'Mr. Goose died', text: 'jk'}
            # ...
        ]
    """
    subreddit, number_posts = options
    sr = reddit.get_subreddit(subreddit)

    result = []

    for post in sr.get_hot(limit=num_posts):
        result.append({'title': post.title, 'text': post.selftext})

    return result
