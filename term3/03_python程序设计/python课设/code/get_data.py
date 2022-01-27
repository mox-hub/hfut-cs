#-*- coding utf-8 -*-
import requests
import parsel
import csv


f = open('rank_list_of_bilibili.csv', mode='a', encoding='utf-8-sig', newline='')

csv_writer = csv.DictWriter(f, fieldnames=['标题', '播放量', '弹幕量', '作者', '综合得分', '视频地址'])
csv_writer.writeheader()
# get data from url of bilibili
url = 'https://www.bilibili.com/v/popular/rank/all?spm_id_from=333.851.b_7072696d61727950616765546162.3'
headers = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36'
} 
# get response
response = requests.get(url= url, headers = headers)

selector = parsel.Selector(response.text)
lis = selector.css('.rank-list li')
flag = 0
# make dit of data
dit = {}
for li in lis:
    # 标题
    title = li.css('.info a::text').get()    
    # 播放量
    bf_info = li.css('div.content > div.info > div.detail > span:nth-child(1)::text').get().strip()    
    # 弹幕量 
    dm_info = li.css('div.content > div.info > div.detail > span:nth-child(2)::text').get().strip()
    # 作者    
    bq_info = li.css('div.content > div.info > div.detail > a > span::text').get().strip()  
    # 综合得分
    score = li.css('.pts div::text').get()      
    # 视频地址
    page_url = li.css('.img a::attr(href)').get()  
    # put data input dit
    dit = {
        '标题': title,
        '播放量': bf_info,
        '弹幕量': dm_info,
        '作者': bq_info,
        '综合得分': score,
        '视频地址': page_url,
    }
    # flag is use to control the number of list 
    if flag == 25:
        break

    csv_writer.writerow(dit)
    flag += 1
    # to show the ditail of data 
    print(dit)
