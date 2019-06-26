# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class RencaiItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    COMPANY = scrapy.Field()
    DIPLOMA = scrapy.Field()
    NUMBER = scrapy.Field()
    DATE = scrapy.Field()
    SUPPLEMENT = scrapy.Field()
    POSITIONTYPE = scrapy.Field()
    KETWORD = scrapy.Field()
    EXPERIENCE = scrapy.Field()
    SALARY = scrapy.Field()
    pass
