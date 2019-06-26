# -*- coding: utf-8 -*-
import scrapy

class Hr2Spider(scrapy.Spider):
    name = 'hr2'
    allowed_domains = ['liepin.com']
    start_urls = ['https://www.liepin.com/zhaopin/?init=-1&headckid=0b4da29b02a59e0d&fromSearchBtn=2&ckid=59870f684695c960&degradeFlag=0&subIndustry=&industryType=industry_01&industries=010&siTag=1B2M2Y8AsgTpgAmY7PhCfg%7EUu4d3oMo-zE-ddoy0_BJog&d_sfrom=search_fp_nvbar&d_ckId=4168fa6cc2f6fddaf1ae01d680b8e6ec&d_curPage=99&d_pageSize=40&d_headId=90547514b861a839361c8adf9e51da0d&curPage=0']
    # print("output3")

    def parse(self, response):
        tr_list = response.xpath("//div[@class='sojob-result ']/ul/li/div")[1:-1]
        # print("output1")
        for tr in tr_list:
            item = {}
            item["POSITIONTYPE"] = tr.xpath("./div[1]/h3/a/text()").extract_first()
            item["COMPANY"] = tr.xpath(".//p[@class='company-name']/a/text()").extract_first()
            item["WORKPLACE"] = tr.xpath("/div[1]/p/a/text()|//span[@class='area']/text()").extract_first()
            item["DIPLOMA"] = tr.xpath("./div[1]/p/span[@class='edu']/text()").extract_first()
            item["EXPERIENCE"] = tr.xpath("./div[1]/p/span[last()]/text()").extract_first()
            item["SALARY"] = tr.xpath("./div[1]/p/span[@class='text-warning']/text()").extract_first()
            item["DATE"] = tr.xpath(".//p[@class='time-info clearfix']/time/@title").extract_first()
            # item["职位名称_href"] = tr.xpath("./div[1]/h3/a/@href").extract_first()

            yield scrapy.Request(
                tr.xpath(".//div[1]/h3/a/@href").extract_first(),
                callback=self.parse_detail,
                meta={"item": item}
            )
        # print("output3")

        # 找到下一页的url地址
        next_url = response.xpath("//div[@class='pagerbar']/a[last()-1]/@href").extract()[0]
        if next_url != "javascript:;":
            next_url =  "https://www.liepin.com" + next_url
            yield scrapy.Request(
                next_url,
                callback=self.parse
            )


    def parse_detail(self, response):
        # print("output6")
        item = response.meta["item"]
        # 此网页没有招聘人数
        # li_list = response.xpath("//div[@class='summary-plane__left']")
        # item["招聘人数"] = li_list.xpath("./ul/li[4]/text()").extract_first()
        item["NUMBER"] = 0
        p_list = response.xpath("//div[@class='about-position']")
        item["SUPPLEMENT"] = p_list.xpath(".//div[@class='content content-word']/text()").extract()
        # 消除提取到的信息中的\n\t\xa0等字符
        item["SUPPLEMENT"] = [x.strip() for x in item["SUPPLEMENT"] if x.strip() != '']
        item["SUPPLEMENT"] = [x.replace("\xa0", "") for x in item["SUPPLEMENT"]]
        # 此网页没有关键字
        item["KEYWORD"] = ""
        yield item
        # print("detail_page")


