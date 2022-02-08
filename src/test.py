list_=[[1,2],[2,3],[3,5]]
list__=[[1,2],[2,3],[3,5]]
list_2=[[1,2],[2,3],[3,7]]
list_3=[]
list_4=[]
# print(list_.index([1,2]))
list_=set(map(tuple,list_3))
list_2=set(map(tuple,list_4))
merged=list_ & list_2
merged=list(merged)
print(merged)

# print(list(merged[0]) == list__[1])
# print(merged[0])

# test_dict={1:[1,2],2:[3,4]}
# dict_list=[test_dict]
# dict_list.append(test_dict)
# print(list(test_dict.values()))