# if you were designing a web crawler, how would you avoid getting into
# infinite loops

# if an already visited page(or url) is not marked down, the search process
# regardless being DFS or BFS would repeatedly visited one same node over
# and over again.

# keep track of the URLs that are already visited in a set
# expand the crawling process in a directed graph (with cycles)

# it depends on how the crawling starts. If it starts with one, then a BFS
# is all we can do. However if it starts with multiple random URLs, then we
# can parallely expand the networks originating from these URLs.

# it can start with 5 processes: after each expansion, check if the outmost
# layers of each 5 processes intersect. If so, merge the intersecting ones
# into one network and reduce the number of processes accordingly.



# Notes from the book:
# What does it mean to visit a page P? Different URLs (with different
# parameters, reasonable or not) might be the same or similar page and
# the same URLs might have different content if some content is randomly
# generated on the fly.

# There is no perfect way to determine a different page. And we will have to
# use some sort of esitmation based on "similarity". Similar pages are
# DEPRIORITIZED before crawling its children pages. 
