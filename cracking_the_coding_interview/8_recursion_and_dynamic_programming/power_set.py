# write a method to return all subset of a set

def power_set(uniq_list, i=0):
    ret = []
    if not uniq_list or i >= len(uniq_list):
        ret.append(set())
        return ret
    if len(uniq_list) == 1:
        ret.append(set())
        ret.append(set(uniq_list[i:]))
        return ret

    rest_set = power_set(uniq_list, i+1)
    ret.extend(rest_set)

    #ret.append({uniq_list[i]})
    
    for each in rest_set:
        ret.append(each | {uniq_list[i]})
        
    return ret
    

if __name__ == "__main__":
    print(power_set(list({1, 2})))
    print(power_set(list(set([1, 2, 3, 1, 2, 4, 5]))))
