import sys

filename = sys.argv[1]
# filename = 'scenedata_boss1'

text = []
text2 = []

with open(filename+'.txt', 'r+', encoding='utf8') as file:
    text = file.readlines()

for textLine in text:
    if textLine != '\n':
        text2.append(textLine.replace('\n', ''))

for i in range(1, len(text2)+1):
    fileTemp = open(filename + '_' + str(i) + '.txt', 'a+', encoding='cp949')
    fileTemp.write(text2[i-1])
    fileTemp.close()
