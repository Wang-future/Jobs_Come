# -*- coding: utf-8 -*-
import scrapy
import re
from jobspider.items import JobspiderItem
import time
import cpca


class JobSpider(scrapy.Spider):
    name = "job"
    allowed_domains = ["https://gz.58.com"]
    start_urls = ['https://gz.58.com/job/']
    # start_urls = ['https://gz.58.com/kefu/35202294979141x.shtml']
    page = 1
    custom_settings = {
        "USER_AGENT": "Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/19.77.34.5 Safari/537.1", }

    position_pattern = r'>(.*?)</span>'
    company_pattern = r'>(.*?)<'
    nexturl_pattern = r'href="(https.*?)"'

    number_pattern = r'\d'
    experience_pattern = r"\d"
    def parse(self, response):
        self.position_pattern = re.compile(self.position_pattern)
        self.company_pattern = re.compile(self.company_pattern)
        self.nexturl_pattern = re.compile(self.nexturl_pattern)

        httplist = response.css('a[href^="https"]::attr(href)').extract()
        urlpattern = r'(https:.*?x.shtml)'
        urlpattern = re.compile(urlpattern)
        urllist = []
        for http in httplist:
            url = re.findall(urlpattern, http)
            if url == []:
                pass
            else:
                urllist.append(url)
        print(urllist)
        print(len(urllist))

        for href in urllist:
            href = str(href)
            href = href.replace('\'', '')
            href = href.replace('[', '')
            href = href.replace(']', '')
            try:
                url = str(href)
                print(url)
                time.sleep(1)
                yield scrapy.Request(url, callback=self.parse_item, dont_filter=True)
            except:
                pass
        nexturl = response.xpath("//a[@class='next']").extract()[0]
        nexturl = str(re.findall(self.nexturl_pattern, nexturl)[0])
        # next_urls = response.xpath("//div[@class='dw_page']").extract()
        # next_urls = str(next_urls)
        # next_url_pattern = r'href="(.*?)">2<'
        # nexturlpattern = re.compile(next_url_pattern)
        # nexturl = re.findall(nexturlpattern, next_urls)[0]
        print(nexturl)
        #别忘了注释调这个部分之后需要把他再注释回来
        if nexturl:
            time.sleep(2)
            self.page += 1
            print(self.page)
            yield scrapy.Request(nexturl, callback=self.parse, dont_filter=True)

    def delete_blank(self, str):
        while str[len(str) - 1:len(str)] == ' ':
            str = str[: -1]
        while str[0: 1] == ' ':
            str = str[1:]
        return str

    def parse_item(self, response):
        item = JobspiderItem()

        company_name = response.xpath('//div[@class="baseInfo_link"]').extract()[0]
        if re.findall(self.company_pattern, company_name)[0] == '':
            item['COMPANY'] = re.findall(self.company_pattern, company_name)[1]
        else:
            item['COMPANY'] = re.findall(self.company_pattern, company_name)[0]
        position_name = response.xpath('//span[@class="pos_name"]').extract()[0]
        position_name = re.findall(self.position_pattern, position_name)[0]
        position_name = self.delete_blank(position_name)
        item['POSITIONTYPE'] = position_name

        number_education_experience = response.xpath('//div[@class="pos_base_condition"]/span/text()').extract()

        number = self.delete_blank(number_education_experience[0])
        number_1 = re.findall(self.number_pattern, number)
        if len(number_1) == 0:
            item['NUMBER'] = 0
        else:
            item['NUMBER'] = int(number_1[0])

        education = self.delete_blank(number_education_experience[1])
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

        experiment = self.delete_blank(number_education_experience[2])
        experience = re.findall(self.experience_pattern, experiment)
        if len(experience) == 0:
            item['EXPERIENCE'] = int(0)
        elif len(experience) == 1:
            item['EXPERIENCE'] = int(experience[0])
        else:
            item['EXPERIENCE'] = int(experience[0])

        location = response.xpath('//span[@class="pos_area_span pos_address"]/span/text()').extract()
        # for loc in location:
        #     full_location += str(loc) + '-'
        # full_location = full_location[: -1]

        if len(location) == 0:
            full_location = ""
        else:
            full_location = str(location[0])
        if full_location == "":
            item['WORKPLACE'] = ""
        else:
            df = cpca.transform([full_location])
            province = (df[0:1])['省'][0]
            city = (df[0:1])['市'][0]
            item['WORKPLACE'] = province + city
        try:
            salary = response.xpath('//span[@class="pos_salary"]/text()').extract()[0]
            if salary == []:
                item['SALARY'] = 0
            else:
                item['SALARY'] = int(salary.split("-")[0])
        except:
            item['SALARY'] = 0

        the_time = response.xpath('//span[@class="pos_base_num pos_base_update"]/span/text()').extract()[0]
        if the_time == "今日":
            item['DATE'] = int(time.mktime(time.strptime('2019-06-26 00:00:00', '%Y-%m-%d %H:%M:%S')))
        else:
            item['DATE'] = int(time.mktime(time.strptime('2019-06-15 00:00:00', '%Y-%m-%d %H:%M:%S')))
        # item['company_type'] = response.xpath('//a[@class="comp_baseInfo_link"]/text()').extract()[0]
        # item['company_scale'] = response.xpath('//p[@class="comp_baseInfo_scale"]/text()').extract()[0]
        # item['company_welfare'] = response.xpath('//div[@class="pos_welfare"]/span/text()').extract()
        item['SUPPLEMENT'] = ''
        for position_message in response.xpath('//div[@class="posDes"]/div/text()').extract():
            item['SUPPLEMENT'] += str(position_message)
            if (len(item['SUPPLEMENT']) >= 490):
                item['SUPPLEMENT'] = item['SUPPLEMENT'][:490]
        key = self.delete_blank(response.xpath('//span[@class="pos_title"]/text()').extract()[0])
        # item['key'] = key.split('/')
        #这里加上一句将key转换成不是列表的形式的语句，这样就可以存储了吧。
        new_ney = key.split('/')
        # for keymess in new_ney:
        #     item['KEYWORD'] += keymess + "-"
        item['KEYWORD'] = new_ney[0]

        yield item
