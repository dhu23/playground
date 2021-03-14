# there are three types of edits that can be performed on strings:
# insert a character, remove a character and replace a character
# given two strings, write a function to check if they are one edit away
# example:
# pale, ple -> true
# pales, pale -> true
# pale, bale -> true
# pale, bae -> false

def verify_insert_edit(longer, shorter):
    assert len(longer) == len(shorter)+1
    i, j = 0, 0
    while j < len(shorter) and longer[i] == shorter[j]:
        i, j = i+1, j+1
    # now i == j but longer[i] != shorter[j]
    i += 1
    return longer[i:] == shorter[j:]

def check_one_edit_away(s1, s2):
    s1_len, s2_len = len(s1), len(s2)
    if s1_len == s2_len:
        # there should be one character different
        diff_count = sum(1 if x != y else 0 for x, y in zip(list(s1), list(s2)))
        return diff_count == 1
    elif abs(s1_len-s2_len) > 1:
        return False
    elif s1_len > s2_len: # s1_len == s2_len+1
        return verify_insert_edit(s1, s2)
    else: # s1_len+1 == s2_len
        return verify_insert_edit(s2, s1)

if __name__ == "__main__":
    print(check_one_edit_away('pale', 'ple'))
    print(check_one_edit_away('pales', 'pale'))
    print(check_one_edit_away('pale', 'bale'))
    print(check_one_edit_away('pale', 'bae'))
