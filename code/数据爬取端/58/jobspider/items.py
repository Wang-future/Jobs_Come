# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class JobspiderItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    COMPANY = scrapy.Field()
    WORKPLACE = scrapy.Field()
    DIPLOMA = scrapy.Field()
    NUMBER = scrapy.Field()
    DATE = scrapy.Field()
    # salary = scrapy.Field()
    SUPPLEMENT = scrapy.Field()
    POSITIONTYPE = scrapy.Field()#这个还没有想到怎么加进来
    KEYWORD = scrapy.Field()
    EXPERIENCE = scrapy.Field()
    # company_type = scrapy.Field()
    # company_scale = scrapy.Field()
    # company_welfare = scrapy.Field()
    SALARY = scrapy.Field()
    pass
