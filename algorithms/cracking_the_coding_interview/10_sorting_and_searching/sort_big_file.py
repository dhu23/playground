# imagine you have a 20GB file with one string per line. Explain how you would
# sort the file

# cut the file into smaller chunks, say 1GB each. Sort each of these smaller
# chunks separately. And then save the sorted files. Merge them one by one. 



# refer to external sorting in wikipedia.
# A cache or internal memory of size M and external memory of blocks of size B
# It sorts M items at a time and puts the sorted lists back to external
# memory. Then recursively does a M/B-way merge on those sorted lists.

# 900MB data using only 100MB RAM
# 1) read 100MB of data in main memory and sort by some conventional method
# 2) write the sorted data to disk
# 3) repeat steps 1 and 2 until all the data is sorted 100MB chunks (9 chunks)
# 4) read the first 10MB of each sorted chunk into input buffer of the main
#    memory and allocate the remaining 10MB for an output buffer. (might be
#    more practical to make output buffer larger and the input buffer slightly
#    smaller with better performance)
# 5) perform a 9-way merge and store the result in the output buffer. Whenever
#    the output buffer fills, write it to the final sorted file and empty it.
#    Whenever any of the 9 input buffers empties, fill it with the next 10MB of
#    its associated 100MB sorted chunk until no more data from the chunk is
#    available. This is the key step that makes external merge sort work
#    externally.

# Additional passes might be necessary if the number of chunks increases
# and memory would be divided into more buffers. There will be many small
# reads rather than fewer larger ones.

import random
import heapq

def generate_sample(n):
    return [random.randint(1, 5000000) for i in range(n)]

def is_sorted(a):
    if len(a) <= 1: return True
    for i in range(1, len(a)):
        if a[i] < a[i-1]:
            return False
    return True

if __name__ == "__main__":
    total_count = 500
    original = generate_sample(total_count)
    piece_count = 10
    piece_size = total_count // piece_count

    chunks = []
    for start in range(0, total_count, piece_size):
        sub_sample = original[start:start+piece_size]
        sub_sample.sort()
        assert is_sorted(sub_sample)
        chunks.append(sub_sample)
        
    # 10-way merge. each chunk at most provides 100//11 elements
    merge_size = piece_size // (piece_count+1)
    
    print(chunks)
        
