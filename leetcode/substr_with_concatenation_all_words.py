import collections

# you are given a string, s, and a list of words, words, that are all of the
# same length. Find all starting indices of substring(s) in s that is a
# concatenation of each word in words exactly once and without any intervening
# characters

# s: "barfoothefoobarman"
# words: ["foo", "bar"]

# answer [0, 9]
# order does not matter

# naive solution
def find_concat_naive(s, words):
    word_lengths = [len(w) for w in words]
    assert max(word_lengths) == min(word_lengths)

    slen = len(s)
    sub_slen = sum(word_lengths)
    assert slen >= sub_slen

    words_map = collections.Counter(words)
    def verify_concat(sub):
        word_len = word_lengths[0]

        start, end = 0, word_len
        words_count = collections.Counter(words_map)
        while end <= len(sub):
            s = sub[start:end]
            if s not in words_count or words_count[s] <= 0:
                return False
            
            words_count[s] -= 1
            start += word_len
            end += word_len
            
        return True
    
    for i in range(0, slen-sub_slen+1):
        sub_str = s[i:i+sub_slen]
        if verify_concat(sub_str):
            print(i, sub_str)


# whenever a substring slice [start:end] is matched
# the end points should satisfy start = i + wl * j, where i < wl
# and end = i + wl * (j+len(words))
# in this sense, there are two degree of freedoms, i and j

# for each i, the algorithm attempts to find an appropriate j
# so that [start:end] has the right substring. It goes by extend
# the end to the point that it no longer contains correct words or
# sees repeated words, then moves along the left side window during
# case of repeated words

# it is sorta like a variation of KMP algorithm

def find_substring_window(s, words):

    word_lengths = [len(w) for w in words]
    assert max(word_lengths) == min(word_lengths)

    slen = len(s)
    sub_slen = sum(word_lengths)
    assert slen >= sub_slen

    word_length = word_lengths[0] # wl
    words_map = collections.Counter(words)

    for i in range(0, word_length):

        found_words_map = collections.Counter()   
        start, end = i, i
        
        while end+word_length < slen:
            new_word = s[end:end+word_length]
            if new_word in words_map:
            
                # found a new sub-word
                if found_words_map[new_word] < words_map[new_word]:
                    found_words_map[new_word] += 1
                    end += word_length
                # can't add this new word to the substring, because it
                # bumps the occurance of the new word to more than it is
                # required. shrink the window from the left side
                else:
                    while (found_words_map[new_word] >= words_map[new_word] or
                           start+word_length <= end):
                        word_to_be_removed = s[start:start+word_length]
                        start += word_length
                        found_words_map[word_to_be_removed] -= 1

                if end-start == sub_slen:
                    print(s[start:end], start, end)
                    start += word_length

            # when this new word is not in dictionary
            # meaning there won't be a result found in the existing substring
            else:
                start = end+word_length
                end = start
                found_words_map.clear()

if __name__ == "__main__":
    s = "barfoothefoobarman"
    words = ["foo", "bar"]
    find_concat_naive(s, words)
    print('----- window ------')
    find_substring_window(s, words)
