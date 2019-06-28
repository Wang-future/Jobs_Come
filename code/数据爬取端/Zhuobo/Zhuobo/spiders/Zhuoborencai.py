# -*- coding: utf-8 -*-
import scrapy
from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule
import re
import time
import cpca
from Zhuobo.items import ZhuoboItem




class ZhuoborencaiSpider(scrapy.Spider):
    name = 'Zhuoborencai'
    allowed_domains = ['jobcn.com']
    url = "https://www.jobcn.com/search/listalljob.xhtml#P"
    offset = 1
    start_urls = [url+str(offset)]
    # rules = (
    #     Rule(LinkExtractor(allow=r'/search/listalljob.xhtml#P\d+')),
    #     Rule(LinkExtractor(allow=r'/search/listalljob.xhtml#P\d+'),callback='parse_item',follow=False),
    # )

    def parse(self, response):
        # item = ZlzpItem()
        # item['publictime'] = response.xpath('')
        # yield item
        links = response.xpath('//h4[@class="job_name"]/a/@href').extract()
        # 迭代取出每个集合里的链接
        for link in links:
            # print(link)
            # 提取列表里每个链接，发送请求并调用callback处理
            yield scrapy.Request('http://www.jobcn.com'+link, callback=self.parse_item ,dont_filter=True)

        if self.offset <= 2000:
            self.offset += 1
            yield scrapy.Request(self.url + str(self.offset), callback=self.parse, dont_filter=True)


    def parse_item(self, response):
        # for each in response.xpath("//body[@class='home_company']"):
            item = ZhuoboItem()
            # 公司名
            company = response.xpath("//div[@class='info clearfix']/div[2]/h2/text()").extract()
            if company==[]:
                item['COMPANY'] = '某能力有限公司'
            else:
                item['COMPANY'] =company[0]
            # 工作地点
            workplace = response.xpath("//div[@class='tab_map_title']/text()").extract()[0]
            df = cpca.transform([workplace])
            province=""
            city=""
            province = (df[0:1])['省'][0]
            city = (df[0:1])['市'][0]
            item['WORKPLACE'] = province+"-"+city
            # 文凭要求
            diploma = response.xpath("//div[@class='info_pos']/div/dl[1]/dd/text()").extract()
            if diploma==[]:
                item['DIPLOMA']="不限"
            else:
                item['DIPLOMA'] = diploma[0].replace("高中","大专以下").replace("中专","大专以下").replace("初中及以下","大专以下")
                # item['diploma'] == "大专以下"
            #招聘人数
            number = response.xpath("//div[@class='name_pos']/div[1]/dl[2]/dd[1]/text()").extract()
            if (number[0].find('人')==-1):
                item['NUMBER']=0
            else:
                 item['NUMBER'] = int(number[0].strip('人'))
            #发布时间
            # date = response.xpath("//div[@class='name_pos']/div[1]/dl[4]/dd[1]/text()").extract()
            item['DATE'] = int(time.time())
            #职位信息
            item['SUPPLYMENT'] = response.xpath("//div[@class='desc_pos']/text()").extract()
            # 职位名称
            item['POSITIONTYPE'] = response.xpath("//div[@class='name_pos']/h2/span[1]/text()").extract()[0]
            #关键字
            keyword = response.xpath("//div[@class='tip_pos']/dl[2]/dd/a/text()").extract()
            if keyword==[]:
                item['KEYWORD']=item['POSITIONTYPE'].replace("/","-")
            else:
                item['KEYWORD']=keyword[0].replace("/","-")
            #工作经验
            experience = response.xpath("//div[@class='info_pos']/div/dl[2]/dd/text()").extract()
            if experience==[]:
                item['EXPERIENCE']=0
            else:
                item['EXPERIENCE'] = int(experience[0].strip('年').replace("应届毕业生","0").replace("不限","0"))
            # 薪水
            salary = response.xpath("//div[@class='salary_body']/text()").extract()
            # item['salary'] = float(salary.match("\d+",salary))*1000
            # pattern=re.compile(r'[1-9]\d*\.\d*|0\.\d*[1-9]\d*$')
            if salary==[]:
                item['SALARY']=0
            else:
                s = salary[0].split("-")[0].split("¥")[1]
                if(len(s)==1):
                    item['SALARY']=int(s+'000')
                else:
                    if(s.find('.')!=-1):
                        sa=s.split(".")
                        item['SALARY']=int(sa[0]+sa[1]+'00')
                    else:
                        item['SALARY']=int(s+'000')

            yield item

