import fileinput
 
TEMPLATE = '''
\033[2J\033[1;1f
     AAAAAAAAA
    FF       BB
    FF       BB
    FF       BB
    FF       BB
    GGGGGGGGGG
   EE       CC
   EE       CC
   EE       CC
   EE       CC
    DDDDDDDDD
''' 
BLOCK = {
    0: '\033[37m░\033[0m', # STFW: ANSI Escape Code
    1: '\033[31m█\033[0m',
}
VARS = 'ABCDEFG'

for v in VARS:
    globals()[v] = 0
stdin = fileinput.input()

while True:
    exec(stdin.readline()) # 返回一个A =XX , B = xx 的字符串，然后exec , 给全局变量赋值
    pic = TEMPLATE
    for v in VARS:
        pic = pic.replace(v, BLOCK[globals()[v]]) # 'A' -> BLOCK[A], ...
    print(pic)
