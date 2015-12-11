import tweepy
def tweet(tweet):
    consumer_key="paste_yours_here"
    consumer_secret="paste_yours_here"
    access_token="paste_yours_here"
    access_token_secret="paste_yours_here"
    auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
    auth.secure = True
    auth.set_access_token(access_token, access_token_secret)
    api = tweepy.API(auth)
    api.update_status(status=tweet)
    return ("@" + api.me().name + ": " + tweet)