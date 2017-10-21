# urs/bin/env python
# -*- coding: utf-8
"""
    @ Author:15331169LiXiaoyun
    datetime: 2017/10/04
    Question: using BFS to find the road between two urls
    采用BFS算法从source和destination双向搜索
    计算已经走过的步数/长度和预估到终点的h值
    两个链接前缀的相似程度可以用作h值
"""

""" Import packages """

import urllib2
import requests
import time
import random
from bs4 import BeautifulSoup
from Queue import PriorityQueue
from difflib import SequenceMatcher

class Node():
    url = ""
    parent = None
    def __init__(self, u, p):
        self.url = u
        self.parent = p

    def __cmp__(self, other):
        return cal_heuristic(self.url, dest) < cal_heuristic(other.url, dest)

    def printPath(self):
        p = self
        paths = []
        while p:
            paths.append(p.url)
            p = p.parent
        paths.reverse()
        for i in paths: print i
        print "Total Length: ", len(paths)

def cal_heuristic(url, other):
    return SequenceMatcher(None, url ,other).ratio()

class Spider():
    def __init__(self):
        pass

    def get_source(self, url):
        user_agents=['Mozilla/5.0 (Windows NT 6.1; WOW64; rv:23.0) Gecko/20130406 Firefox/23.0',
        'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:18.0) Gecko/20100101 Firefox/18.0',
        'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/533+ \(KHTML, like Gecko) Element Browser 5.0',
        'IBM WebExplorer /v0.94', 'Galaxy/1.0 [en] (Mac OS X 10.5.6; U; en)',
        'Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)',
        'Opera/9.80 (Windows NT 6.0) Presto/2.12.388 Version/12.14',
        'Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) \Version/6.0 Mobile/10A5355d Safari/8536.25',
        'Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) \Chrome/28.0.1468.0 Safari/537.36',
        'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.0; Trident/5.0; TheWorld)']
        # user_agent在一堆范围中随机获取
        index = random.randint(0,9)
        user_agent = user_agents[index]
        headers = {'User_agent': user_agent}
        html = requests.get(url, headers=headers)
        return html.text

    def extractUrls(self, htmlText):
        soup = BeautifulSoup(htmlText, "lxml")
        urls = []
        for url in soup.find_all('a'):
            urls.append(url.get('href'))
        # print("extract urls done!")
        return urls

    def bfs(self, source, target):
        visited = []
        q = PriorityQueue()
        src = Node(source, None)
        q.put(src)
        while not q.empty():
            cur = q.get()
            visited.append(cur.url)
            if (cur.url == target):
                cur.printPath()
                break
            for url in self.extractUrls(self.get_source(cur.url)):
                if url and url not in visited and url != "#":
                    # print url
                    q.put(Node(url, cur))


if __name__=='__main__':
    t0 = time.time()
    # source = raw_input('Source url:')
    # dest = raw_input('Destination url:')
    source = "http://www.baidu.com"
    dest = "http://ent.qq.com/movie/"
    # dest = "http://news.baidu.com"
    Spider().bfs(source, dest)
    print "Use Time {}s".format(time.time() - t0)
