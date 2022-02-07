# list_=[[1,2],[2,3],[3,5]]
# list__=[[1,2],[2,3],[3,5]]
# list_2=[[1,2],[2,3],[3,7]]
# print(list_.index([1,2]))
# list_=set(map(tuple,list_))
# list_2=set(map(tuple,list_2))
# merged=list_ & list_2
# merged=list(merged)

# print(list(merged[0]) == list__[1])
# print(merged[0])

def test(a):
    if a>0:
        return a,0
    else:
        return a

a,b=test(1)
c=test(-1)