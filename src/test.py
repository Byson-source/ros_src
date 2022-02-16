def arrange(length):
    list1 = []
    list2 = []
    for val in range(length):
        if val % 2 == 0:
            list1.append(val)
        else:
            list2.append(val)
    map = {}
    input = 0
    for val in list1:
        map[val] = input
        input += 1

    for val in list2:
        map[val] = input
        input += 1

    print(map)


arrange(13)
