# you are building a diving board by placing a bunch of planks of wood
# end-to-end. There are two types of planks, one of length shorter and one
# of length longer. You must use exactly K planks of wood. Write a method
# to generate all possible lengths for the diving boards

# mathematically:
# x+y=k
# y = x*short + y*long = (k-y)*short+y*long =k*shorter+y*(long-short)
# y <- [0..k], so there are k+1 possible lengths


if __name__ == "__main__":
    pass
