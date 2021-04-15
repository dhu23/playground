# a large eCommerce company wishes to list the best-selling products overall
# and by category. For example, one product might be the #1056th best-selling
# product overall but the #13th best selling product under sports equipment.
# and the #24th best-selling product under "safty". Describe how you would
# design the system.


# mock data
#             category 1     category 2     category 3    ....    overall
# product 1      1                5          12345                  1232
# product 2      2             1212              1                  5566
# product 3      3              156             10                   367
# ....
# product 100k  ...             ...             ...                 ....

# request functions:
# rankByCategory(category) and rankByOverall()

# given the fact that these data could constantly change and get updated,
# we might want to verticall partition the database. Each category is updated
# regularly(daily or weekly) and the product information may be duplicated
# for each category and overall rankings. 

