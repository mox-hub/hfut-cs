import requests
from bs4 import BeautifulSoup
import time
import os
import tkinter as tk
from tkinter import scrolledtext, messagebox
import re

# 生成请求URL
def create_request(page, poet_name):
    base_url = 'https://www.gushiwen.cn/shiwens/default.aspx'
    params = {
        'page': page,
        'tstr': '',
        'astr': poet_name,  # 根据输入的诗人名字搜索
        'cstr': '',
        'xstr': ''
    }

    try:
        response = requests.get(base_url, params=params, timeout=10)  # 增加超时限制
        response.raise_for_status()  # 如果请求失败，抛出异常
        return response.text
    except requests.exceptions.RequestException as e:
        print(f"请求失败: {e}")
        return None

# 解析网页内容并提取诗歌信息
def parse_content(html):
    if html is None:
        return []

    soup = BeautifulSoup(html, 'html.parser')
    poems = []

    # 获取所有诗歌的相关信息
    poem_elements = soup.find_all('div', class_='sons')

    if not poem_elements:
        return poems

    for poem_element in poem_elements:
        title = poem_element.find('b').get_text() if poem_element.find('b') else "无标题"
        # 如果标题为"无标题"，则跳过这首诗歌
        if title == "无标题":
            continue

        author = poem_element.find('p', class_='source').get_text() if poem_element.find('p', class_='source') else "未知作者"
        content = poem_element.find('div', class_='contson').get_text().strip() if poem_element.find('div', class_='contson') else "无内容"

        poems.append({
            'title': title,
            'author': author,
            'content': content
        })

    return poems

def clean_filename(filename):
    # 定义非法字符正则（这里只替换文件名部分）
    invalid_chars = r'[\\/:*?"<>|\n\r]'
    # 仅清理文件名部分，而保留路径分隔符
    return re.sub(invalid_chars, "_", filename)
# 保存数据到txt文件
def save_to_txt(poet_name, poems):
    folder_path = 'D:/poet'  # 固定保存路径为 D:/poet
    # 检查保存的文件夹路径是否存在
    if not os.path.exists(folder_path):
        try:
            os.makedirs(folder_path)  # 确保路径正确创建
        except Exception as e:
            print(f"文件夹创建失败: {e}")
            return

    # 获取文件名，使用诗人名字作为文件名
    filename = os.path.join(folder_path, f'{clean_filename(poet_name)}_诗歌.txt')

    try:
        # 判断文件是否存在
        file_mode = 'a' if os.path.exists(filename) else 'w'
        with open(filename, file_mode, encoding='utf-8') as f:
            for poem in poems:
                author = poem['author'].strip()
                f.write(f"标题: {poem['title']}\n")
                f.write(f"作者: {author}\n")
                f.write(f"内容: {poem['content']}\n\n")
    except Exception as e:
        print(f"文件保存失败: {e}")

# 主程序，爬取指定页数
def scrape_poems(start_page, end_page, poet_name, progress_text, title_listbox):
    for page in range(start_page, end_page + 1):
        progress_text.insert(tk.END, f"正在爬取第 {page} 页...\n")
        progress_text.yview(tk.END)  # 自动滚动到最新的内容

        html = create_request(page, poet_name)
        if html is None:
            progress_text.insert(tk.END, f"第 {page} 页加载失败，跳过该页。\n")
            progress_text.yview(tk.END)
            continue

        poems = parse_content(html)

        if poems:
            save_to_txt(poet_name, poems)
            progress_text.insert(tk.END, f"第 {page} 页的数据已保存！\n")
            for poem in poems:
                title_listbox.insert(tk.END, poem['title'])  # 只插入有效的诗歌标题
        else:
            progress_text.insert(tk.END, f"第 {page} 页没有找到诗歌。\n")

        progress_text.yview(tk.END)  # 自动滚动到最新的内容
        time.sleep(1)  # 延迟 1 秒，避免过快请求

    progress_text.insert(tk.END, "爬取完成！\n")
    progress_text.yview(tk.END)


# GUI界面实现
def start_scraping():
    poet_name = poet_name_entry.get().strip()
    if not poet_name:
        messagebox.showwarning("警告", "请输入诗人名字！")
        return

    try:
        start_page = int(start_page_entry.get())
        end_page = int(end_page_entry.get())
    except ValueError:
        messagebox.showwarning("警告", "请输入有效的页码！")
        return

    title_listbox.delete(0, tk.END)  # 清空之前的标题列表
    progress_text.delete(1.0, tk.END)  # 清空爬取进程显示

    scrape_poems(start_page, end_page, poet_name, progress_text, title_listbox)

# 创建GUI窗口
root = tk.Tk()
root.title("诗歌爬取工具")

# 设置窗口大小
root.geometry('700x700')
root.configure(bg='#f0f0f0')

# 设置字体
font = ("Arial", 12)

# 设置标题
header_label = tk.Label(root, text="诗歌爬取工具", font=("Arial", 16, "bold"), bg="#4CAF50", fg="white", width=50)
header_label.grid(row=0, column=0, columnspan=2, pady=10)

# 使用 Canvas 设置背景图片
background_image = tk.PhotoImage(file="D:\\图片\\1687828512585698.png")
canvas = tk.Canvas(root, width=700, height=700)
canvas.create_image(0, 0, anchor=tk.NW, image=background_image)
canvas.grid(row=0, column=0, columnspan=2, rowspan=10)

# 诗人名字输入框
poet_name_label = tk.Label(root, text="请输入诗人名字：", font=font, bg="#f0f0f0")
poet_name_label.grid(row=1, column=0, padx=10, pady=5, sticky="w")

poet_name_entry = tk.Entry(root, width=30, font=font)
poet_name_entry.grid(row=1, column=1, padx=10, pady=5, sticky="w")

# 起始页和结束页输入框
start_page_label = tk.Label(root, text="请输入起始页码：", font=font, bg="#f0f0f0")
start_page_label.grid(row=2, column=0, padx=10, pady=5, sticky="w")

start_page_entry = tk.Entry(root, width=10, font=font)
start_page_entry.grid(row=2, column=1, padx=10, pady=5, sticky="w")

end_page_label = tk.Label(root, text="请输入结束页码：", font=font, bg="#f0f0f0")
end_page_label.grid(row=3, column=0, padx=10, pady=5, sticky="w")

end_page_entry = tk.Entry(root, width=10, font=font)
end_page_entry.grid(row=3, column=1, padx=10, pady=5, sticky="w")

# 开始爬取按钮
start_button = tk.Button(root, text="开始爬取", font=("Arial", 12), bg="#4CAF50", fg="white", command=start_scraping, relief="raised")
start_button.grid(row=5, column=0, columnspan=2, pady=20)

# 爬取过程的文本框
progress_text = scrolledtext.ScrolledText(root, width=70, height=10, font=font)
progress_text.grid(row=6, column=0, columnspan=2, padx=10, pady=5)

# 添加一个标题列表框组件
title_listbox_label = tk.Label(root, text="诗歌标题列表：", font=font, bg="#f0f0f0")
title_listbox_label.grid(row=7, column=0, padx=10, pady=5, sticky="w")
title_listbox = tk.Listbox(root, width=70, height=10, font=font)
title_listbox.grid(row=8, column=0, columnspan=2, padx=10, pady=5)

# 启动GUI主循环
root.mainloop()
