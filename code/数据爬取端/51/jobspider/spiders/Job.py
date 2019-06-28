# -*- coding: utf-8 -*-
import scrapy
import re
from jobspider.items import JobspiderItem
import time
import cpca


class JobSpider(scrapy.Spider):
    name = "job"
    allowed_domains = ["51job.com"]
    start_urls = ['https://search.51job.com/list/000000,000000,0000,00,9,99,%2B,2,1.html?lang=c&stype=1&postchannel=0000&worky'
                  'ear=99&cotype=99&degreefrom=99&jobterm=99&companysize=99&lonlat=0%2C0&radius=-1&ord_field=0&confirmdate=9&fro'
                  'mType=&dibiaoid=0&address=&line=&specialarea=00&from=&welfare=']
    page = 1
    custom_settings = {
        "USER_AGENT": "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36", }

    whether_education_pattern = r'招'
    experience_pattern = r"\d"
    number_pattern = r'\d'

    base_url = 'https://search.51job.com/list/000000,000000,0000,00,9,99,%2B,2,'
    last_url = '.html?lang=c&stype=1&postchannel=0000&workyear=99&cotype=99&degr' \
               'eefrom=99&jobterm=99&companysize=99&lonlat=0%2C0&radius=-1&ord_fiel' \
               'd=0&confirmdate=9&fromType=&dibiaoid=0&address=&line=&specialarea=00&from=&welfare='

    def parse(self, response):

            # self.whether_education_pattern = re.compile(self.whether_education_pattern)
            for href in response.css('a[href$="t=0"]::attr(href)').extract():
                try:
                    url = str(href)
                    yield scrapy.Request(url, callback=self.parse_item)
                except:
                    pass
            self.page += 1
            nexturl = self.base_url + str(self.page) + self.last_url

            if self.page < 2000:
                try:
                    yield scrapy.Request(nexturl, callback=self.parse, dont_filter=True)
                    time.sleep(1)
                    print(self.page)
                except:
                    raise RuntimeError('testError')
            else:
                pass


    def parse_item(self, response):
        item = JobspiderItem()

        company = response.xpath('//div[@class="in"]/div/p/a/@title').extract()[0]
        company = company.replace("(", "_")
        company = company.replace(")", "_")
        item['COMPANY'] = company
        item['POSITIONTYPE'] = response.xpath('//div[@class="in"]/div/h1/@title').extract()[0]
        some_messages = response.xpath('//div[@class="in"]/div/p/@title').extract()[0]
        some_messages = "".join(some_messages.split())
        some_messages = some_messages.split('|')
        whether_education = some_messages[2].find(self.whether_education_pattern)
        if whether_education != -1:
            some_messages.insert(2, "不限")
        else:
            pass
        number = some_messages[3]
        number_1 = re.findall(self.number_pattern, number)
        if len(number_1) == 0:
            item['NUMBER'] = 0
        else:
            item['NUMBER'] = int(number_1[0])

        location = some_messages[0].split("-")
        df = cpca.transform(location)
        province = ""
        city = ""
        province = (df[0:1])['省'][0]
        city = (df[0:1])['市'][0]
        item['WORKPLACE'] = province + city



        salarys = response.xpath('//div[@class="in"]/div/strong/text()').extract()
        try:
            if salarys == []:
                item['SALARY'] = 0
            else:
                salary = salarys[0]
                thousand = salary.find("千")
                ten_thousand = salary.find("万")
                yuan = salary.find("元")
                day = salary.find("天")
                month = salary.find("月")
                year = salary.find("年")

                if thousand != -1:
                    if year != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 1000 / 12)
                    elif month != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 1000)
                    elif day != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 30 * 1000)
                elif ten_thousand != -1:
                    if year != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 10000 / 12)
                    elif month != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 10000)
                    elif day != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 30 * 10000)
                elif yuan != -1:
                    if year != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) / 12)
                    elif month != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]))
                    elif day != -1:
                        item['SALARY'] = int(float(salarys[0].split("-")[0]) * 30)
                else:
                    item['SALARY'] = 0
        except:
            raise RuntimeError('salary太麻烦了，舍弃这个吧')

        the_time = some_messages[4].replace("发布", "")
        the_time = '2019-' + the_time + ' 00:00:00'
        item['DATE'] = int(time.mktime(time.strptime(the_time, '%Y-%m-%d %H:%M:%S')))

        education = some_messages[2]
        if education == "博士":
            item['DIPLOMA'] = "博士"
        elif education == "硕士":
            item['DIPLOMA'] = "硕士"
        elif education == "本科":
            item['DIPLOMA'] = "本科"
        elif education == "大专":
            item['DIPLOMA'] = "大专"
        elif education == "不限":
            item['DIPLOMA'] = "不限"
        else:
            item['DIPLOMA'] = "大专以下"


        experience = re.findall(self.experience_pattern, some_messages[1])
        if len(experience) == 0:
            item['EXPERIENCE'] = int(0)
        elif len(experience) == 1:
            item['EXPERIENCE'] = int(experience[0])
        else:
            item['EXPERIENCE'] = int(experience[0])

        # item['company_type'] = response.xpath('//div[@class="com_tag"]/p[1]/text()').extract()[0]
        # item['company_scale'] = response.xpath('//div[@class="com_tag"]/p[2]/text()').extract()
        # company_welfare = response.xpath('//div[@class="jtag"]/div/span/text()').extract()
        # if company_welfare:
        #     item['company_welfare'] = company_welfare
        # else:
        #     item['company_welfare'] = ['暂无信息']
        keymessage = response.xpath('//div[@class="tCompany_main"]/div/div[@class="bmsg job_msg inbox"]/div/p[@class="fp"][2]').extract()
        keymessage = str(keymessage)
        messagepattern = "[\u4e00-\u9fa5]+"
        messagepattern = re.compile(messagepattern)
        keymessage = messagepattern.findall(keymessage)
        if len(keymessage) == 0:
            item['KEYWORD'] = ""
        else:
            item['KEYWORD'] = ""
            keymessage.remove("关键字")
            for keymess in keymessage:
                item['KEYWORD'] += keymess + "-"
            item['KEYWORD'] = item['KEYWORD'][: -1]
        position_message = response.xpath('//div[@class="tCompany_main"]/div/div[@class="bmsg job_msg inbox"]/p/text()').extract()
        nullpattern = '\xa0'
        if position_message == []:
            # position_message = response.xpath('//div[@class="tCompany_main"]/div/div[@class="bmsg job_msg inbox"]/div/text()').extract()
            pos_mes = response.xpath('//div[@class="bmsg job_msg inbox"]').xpath('string(.)').extract()[0]
            pos_mes = str(pos_mes)
            pos_mes = pos_mes.replace('\t', '')
            pos_mes = pos_mes.replace('\n', '')
            pos_mes = pos_mes.replace('\r', '')
            print(pos_mes)
            if(len(pos_mes) >= 490):
                pos_mes = pos_mes[:490]
            item['SUPPLEMENT'] = pos_mes
        else:
            item['SUPPLEMENT'] = ""
            for message in position_message:
                item['SUPPLEMENT'] += str(message)
                if (len(item['SUPPLEMENT']) >= 490):
                    item['SUPPLEMENT'] = item['SUPPLEMENT'][:490]
                    break
        yield item
