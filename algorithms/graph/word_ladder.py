
# dictionary POON, PLEE, SAME, POIE, PLEA, PLIE, POIN
# start = TOON
# target = PLEA

# path = TOON - POON - POIN - POIE - PLIE - PLEE - PLEA


def is_connected(w1, w2):
    assert len(w1) == len(w2)

    count = 0
    for i in range(len(w1)):
        if w1[i] != w2[i]:
            count += 1
    return True if count == 1 else False


def find_word_ladder(words, start, target):

    def print_path(prev, word):
        print prev, word
        curr = word
        path = [curr]
        while prev[curr] is not None:
            path.append(prev[curr])
            print path
            curr = prev[curr]
            
        path.reverse()
        return path
    
    # BFS to find the shortest path
    q = [(start, 1)]

    visited = set()
    prev = {}
    prev[start] = None
    while len(q) > 0:
        current, depth = q.pop(0)

        for word in words:
            if word not in visited and is_connected(current, word):
                q.append((word, depth+1))
                visited.add(word)

                prev[word] = current

                if word == target:
                    return depth+1, print_path(prev, word)

    return 0, None


if __name__ == '__main__':
    words = ['POON', 'PLEE', 'SAME', 'POIE', 'PLEA', 'PLIE', 'POIN']
    print find_word_ladder(words, 'TOON', 'PLEA')
