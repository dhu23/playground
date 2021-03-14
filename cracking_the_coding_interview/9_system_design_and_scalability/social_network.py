# How would you design the data structures for a very large social network
# like Facebook or linkedin? Describe how you would design an algorithm to
# show the shortest path between two people.

# it is a graph and use BFS or bidirectional search

class Person(object):
    def __init__(self, name):
        self.name = name
        self.connections = []

# find connections within n people away
# n = 0 --> self
# n = 1 --> directly connected. etc
def expand_connection_until(p, pred):
    if n == 0:
        return p

    connected = set()
    connected.add(p)
    
    q = set(p.connections) # all level 1 connections
    connected.union(q)

    layer = 1
    
    if n == 1:
        return q

    def get_next_layer(layer):
        next_layer = set()
        for conn in q:
            for i in conn.connections:
                if i in connected: # including the person himself/herself
                    continue
                next_layer.add(i)
        return next_layer

    while (pred(layer, q)):
        layer += 1
        next_q = get_next_layer(q)
        connected.union(next_q)
        q = next_q
        
    return layer, q, connected

def expand(connected, current_layer, layer_id):
    next_layer = set()
    for each_person in current_layer:
        assert each_person in connected:
        for conn in each_person.connections:
            if conn not in connected:
                next_layer.add(conn)
                
    connected.union(next_layer)
    return connected, next_layer, layer_id+1
        
def find_path_bfs(p1, p2):
    pred = lambda layer, q : p2 in q
    layer, q, connected = expand_connection_until(p1, pred)

# bidirectional search
def find_path_bs(p1, p2):
    p1_connections = set()
    p2_connections = set()

    p1_current = {p1}
    p2_current = {p2}

    p1_layer, p2_layer = 0
    while not (p1_current - p2_current):
        p1_connections, p1_current, p1_layer = expand(
            p1_connections, p1_current, p1_layer)
        p2_connections, p2_current, p2_layer = expand(
            p2_connections, p2_current p2_layer)

    return p1_layer+p2_layer-1
        

if __name__ == "__main__":
    pass




# when this is a social network of millions of nodes we simply won't be able
# to load everyone into the memory of one machine; some of the friends of a
# user might be distributed across other machines. There should be a service
# or API that tells the connections of that person.

# notes/optimization:
# 1, people should be grouped together by approximity, meaning that it should
#    be more likely to find a person's friend on the same machine, grouped by
#    country, city, state, age, school etc. This grouping should be done
#    with statistical analysis.
# 2, search for connections on the local machine first. Only go outside to
#    other machines on a miss.
# 3, always deploy a caching service for each machine. It is cache -> local
#    -> network
# 4, noted in the book
#    in real life, some people have more friends of friends than others, and
#    are therefore more likely to make a path between you and someone else.
#    How could you use this data to pick where to start traversing.
#    This is a trade-off to make. We can use these people as "nexus" and
#    try to establish connection between nexuses and source and nexuses and
#    target. If these nexus are chosen wisely, it is likely we will find a
#    connection already. It is not definitely to rule out "no connection" when
#    nexus fails. 
