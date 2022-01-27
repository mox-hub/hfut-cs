# coding:utf8
import cPickle as pickle
import matplotlib.pyplot as plt
import numpy as np

x_list = [i for i in range(-50, 101, 5)]
f = open('y_list_1', 'rb')
y_list_1 = pickle.load(f)
f.close()
f = open('y_list_2', 'rb')
y_list_2 = pickle.load(f)
f.close()

# font = FontProperties(fname=r"c:\windows\fonts\simsun.ttc", size=14)
# fig, ax = plt.subplots()
plt.figure()
plt.title(u"中文垃圾邮件过滤效果图")
plt.grid(True)
plt.plot(x_list, y_list_1, '-r|', label=u"总体准确率")
plt.plot(x_list, y_list_2, '-b|', label=u"正常邮件获取率")

plt.xticks(np.linspace(-50, 100, 16, endpoint=True))
plt.yticks(np.linspace(0.8, 1, 21, endpoint=True))

plt.legend(loc="best")
plt.xlabel("T")
plt.ylabel(u"百分比")
plt.show()
