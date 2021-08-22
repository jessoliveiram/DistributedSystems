import random

block = False
result = 0
#Returns a random number between -100 and 100
def generate_number():
    return random.randrange(-100,100)

#Returns a list with n random numbers between -100 and 100
#In python numbers are imutable objects and exist only once,
#it means that each element of a interger list is a reference
#to a number object that exists once in memory.
#This can verified by:
# a = [1,2,3]
# b = 1
# c = {"a":1}
# id(a[0]) == id(b) == id(c["a"])
# The id function returns the identify of the object.
def generate_numbers_list(n):
    numbers_list = []
    for i in range(n):
        numbers_list.append(generate_number())
    return numbers_list

def sum_list_elements(numbers_list):
    total = 0
    for number in numbers_list:
        total += number
    return total

def acquire():
    block = True

def release():
    block = False

#The thread wait in loop until the global result is free to write
def thread_safe_sum(numbers_list):
    thread_total = sum_list_elements(numbers_list)
    global result
    while True:
        if not block:
            acquire()
            result += thread_total
            release()
            break

def paralel_sum_list_elements(numbers_list, n_threads):
    pass


if __name__ == "__main__":
    numbers_list = generate_numbers_list(10)
    thread_safe_sum(numbers_list)
    print(numbers_list)
    print(sum(numbers_list))
    print(result)