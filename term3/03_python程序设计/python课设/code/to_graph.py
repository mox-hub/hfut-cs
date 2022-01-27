#-*- coding utf-8 -*-
import csv
import numpy as np
import matplotlib.pyplot as plt
import re

'''
to solve the chinese in num_data get from bilibili
input :    string
output :   floatS
'''
def change_num(number):
    flag = '万'
    if flag in number:
        # change wan to 10000
        number = re.sub('万','',number)
        return float(number) * 10000 
    else:
        return float(number)

# get data from *.csv
# encoding UTF-8
with open(r"rank_list_of_bilibili.csv",encoding='UTF-8') as bili:
    read_csv = csv.reader(bili)

    # prepare list
    title ,bf_info ,dm_info, bq_info ,score ,page_url = [],[],[],[],[],[]
    
    index = 0;
    for i in read_csv:
        if index != 0:
            title.append(i[0])
            bf_info.append(change_num(i[1]))
            dm_info.append(change_num(i[2]))
            bq_info.append(i[3])
            score.append(change_num(i[4]))
            page_url.append(i[5])
        index += 1
    # set title 
    title_list = ['title','bf_info','dm_info','bq_info','score','page_url']
    lists = {}

lists["title"],lists["bf_info"],lists["dm_info"],lists["bq_info"] ,lists["score"] ,lists["page_url"]  = title ,bf_info ,dm_info ,bq_info ,score ,page_url

print(lists)

# you can chenge the width of the bar(single)
bar_width = 0.4

'''
print the graph :using matplotib
'''
plt.title('B站视频top20')
plt.rcParams['font.sans-serif'] = ['SimHei']  # 显示汉字
plt.xticks(rotation=-90)
plt.xlabel('播放量（百万）')
plt.ylabel('视频名称')
x = np.arange(len(lists["title"]))

plt.plot(x,lists["bf_info"],marker = 'o',markersize = 3)
for a, b in zip(x , lists["bf_info"]):
    plt.text(a,b,b,ha = 'center', va='bottom', fontsize=10)

# plt.barh(x ,lists["bf_info"] ,bar_width ,tick_label = lists["title"] ,label = '热度')
# plt.barh(x + bar_width ,lists["score"] ,bar_width ,tick_label = lists["title"] ,label = '分数')

plt.legend()
plt.show()
