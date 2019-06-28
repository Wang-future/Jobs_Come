# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class ZhuoboItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    #公司名
    COMPANY= scrapy.Field()
    #工作地点
    WORKPLACE= scrapy.Field()
    #教育水平
    DIPLOMA= scrapy.Field()
    #招聘人数
    NUMBER=scrapy.Field()
    #发布时间
    DATE= scrapy.Field()
    #岗位要求
    SUPPLYMENT= scrapy.Field()
    # #岗位职责
    # RESPONSIBILITY= scrapy.Field()
    # 职位名
    POSITIONTYPE = scrapy.Field()
    #关键字
    KEYWORD=scrapy.Field()
    #工作经验
    EXPERIENCE=scrapy.Field()
    #薪水
    SALARY= scrapy.Field()

