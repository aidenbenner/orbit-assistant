import praw

user_agent = 'linux:dank_memes:Mr.Goose'
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
    subreddit = options[0]
    number_posts = int(options[1])
    sr = reddit.get_subreddit(subreddit)

    result = []

    for i, post in enumerate(sr.get_hot(limit=number_posts)):
        result.append({'title': str(i + 1) + '. ' + post.title, 'text': post.selftext})

    return result
