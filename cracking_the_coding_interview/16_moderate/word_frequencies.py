# design a method to find the frequency of occurences of any given word in a
# book. What if we were running this algorithm multiple times?

def find_once(b, w):
    count = 0
    for each in b:
        if each == w:
            count += 1
    return count

def build(b):
    counts = {}
    for each in b:
         if each in counts:
             counts[each] += 1
         else:
             counts[each] = 1
    return counts

if __name__ == "__main__":
    book = ["a", "an", "a", "the", "cat", "dog", "the", "time", "act"]

    target_word = "the"

    # one time use
    print(find_once(book, target_word))
    cm = build(book)

    # multiple time usage
    print(cm.get("dog", 0))
    print(cm.get(target_word, 0))
    print(cm.get('hello', 0))
