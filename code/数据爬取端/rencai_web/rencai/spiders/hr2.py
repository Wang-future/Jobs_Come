# -*- coding: utf-8 -*-
import scrapy

class Hr2Spider(scrapy.Spider):
    name = 'hr2'
    allowed_domains = ['liepin.com']
    start_urls = ['https://www.liepin.com/zhaopin/?init=-1&headckid=0b4da29b02a59e0d&fromSearchBtn=2&ckid=59870f684695c960&degradeFlag=0&subIndustry=&industryType=industry_01&industries=010&siTag=1B2M2Y8AsgTpgAmY7PhCfg%7EUu4d3oMo-zE-ddoy0_BJog&d_sfrom=search_fp_nvbar&d_ckId=4168fa6cc2f6fddaf1ae01d680b8e6ec&d_curPage=99&d_pageSize=40&d_headId=90547514b861a839361c8adf9e51da0d&curPage=0']
    print("output3")

    def parse(self, response):
        tr_list = response.xpath("//div[@class='sojob-result ']/ul/li/div")[1:-1]
        print("output1")
        for tr in tr_list:
            item_2 = {}
            item_2["职位名称"] = tr.xpath("./div[1]/h3/a/text()").extract_first()
            item_2["公司名称"] = tr.xpath(".//p[@class='company-name']/a/text()").extract_first()
            item_2["工作地点"] = tr.xpath("/div[1]/p/a/text()|//span[@class='area']/text()").extract_first()
            item_2["教育水平"] = tr.xpath("./div[1]/p/span[@class='edu']/text()").extract_first()
            item_2["工作经验"] = tr.xpath("./div[1]/p/span[last()]/text()").extract_first()
            item_2["薪水"] = tr.xpath("./div[1]/p/span[@class='text-warning']/text()").extract_first()
            item_2["发布时间"] = tr.xpath(".//p[@class='time-info clearfix']/time/@title").extract_first()
            item_2["职位名称_href"] = tr.xpath("./div[1]/h3/a/@href").extract_first()

            yield scrapy.Request(
                tr.xpath(".//div[1]/h3/a/@href").extract_first(),
                callback=self.parse_detail,
                meta={"item_2": item_2}
            )
        print("output3")

        # 找到下一页的url地址
        next_url = response.xpath("//div[@class='pagerbar']/a[last()-1]/@href").extract()[0]
        if next_url != "javascript:;":
            next_url =  "https://www.liepin.com" + next_url
            yield scrapy.Request(
                next_url,
                callback=self.parse
            )


    def parse_detail(self, response):
        print("output6")
        item_2 = response.meta["item_2"]
        # 此网页没有招聘人数
        # li_list = response.xpath("//div[@class='summary-plane__left']")
        # item_2["招聘人数"] = li_list.xpath("./ul/li[4]/text()").extract_first()
        p_list = response.xpath("//div[@class='about-position']")
        item_2["职位信息"] = p_list.xpath("./div[@class='content content-word']/text()").extract()
        # 消除提取到的信息中的\n\t\xa0等字符
        item_2["职位信息"] = [x.strip() for x in item_2["职位信息"] if x.strip() != '']
        item_2["职位信息"] = [x.replace("\xa0", "") for x in item_2["职位信息"]]

        yield item_2
        print("detail_page")


