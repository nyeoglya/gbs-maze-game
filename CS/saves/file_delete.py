import os
import sys

filename = sys.argv[1]
# filename = 'scenedata_boss1'

file_list = os.listdir('.')

asdf = 0

for fileName in file_list:
    if fileName.find(filename+'_') == 0:
        asdf += 1

for i in range(1, asdf+1):
    os.remove(filename + '_' + str(i) + '.txt')
