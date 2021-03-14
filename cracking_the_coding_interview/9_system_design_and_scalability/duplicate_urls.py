# You have 10 billion URLs. How do you detect the duplicate documents. In
# this case, assume duplicate means that the URLs are identical


# normally, for a group of data, for example an array of data with duplicates,
# we sort it and then linearly search through. However here we have 10 billion
# data points.

# say sorting 10 million is reasonable. So:
# chop the data into 1000 pieces, categorize the URLs with ones that start
# with http and https. Further categorize the initial two characters into aa
# ab, ac, ...
# sort and find out duplicates within each group and there is also no
# intersection between groups. 
