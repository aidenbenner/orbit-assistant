import praw
import requests
import requests.auth 
import json 

client_auth = requests.auth.HTTPBasicAuth('p-jcoLKBynTLew', 'gko_LXELoV07ZBNUXrvWZfzE3aI')
post_data = {"grant_type": "password", "username": "tiva_bot", "password": "ThankMrGoose"}
headers = {"User-Agent": "SE101Project/0.1 by LPan & ABen"}
response = requests.post("https://www.reddit.com/api/v1/access_token", auth=client_auth, data=post_data, headers=headers)

my_user_agent = "linux:SE101_project:v1.0 "
my_client_id = "my client ID"
my_client_secret = "my client secret"

reddit = praw.Reddit(user_agent=my_user_agent,
                     client_id=my_client_id,
                     client_secret=my_client_secret)
