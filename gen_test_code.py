from random import choice, randint, randrange
def gen_string(a, b):
    return ''.join([choice(".-") for _ in range(randrange(1,15))])

out = open("test_code","w");
outstr = ""
for i in range(randrange(20,30)):
    outstr+="String {}:\n".format(i+1)+gen_string(1,15) + '\n'
out.write(outstr)
