# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html

#from pymongo import MongoClient

#client = MongoClient()
#collection = client["tencent"]["hr1"]
import json

class RencaiPipeline(object):
    def process_item(self, item, spider):
        # python列表转json数组
        json_array1 = json.dumps(item)
        # python列表转json数组并写入文件
        # fp = open('json_array.json', 'w')
        # json_array1 = json.dump(item, fp)
        # print(type(json_array1))
        print(json_array1)

        # print(item)
        # collection.insert(item)
        return item
class RencaiPipeline(object):
    def process_item(self, item_2, spider):
        print(item_2)
        # collection.insert(item)
        return item_2
