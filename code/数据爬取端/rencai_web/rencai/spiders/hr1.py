# -*- coding: utf-8 -*-
import scrapy

class Hr1Spider(scrapy.Spider):
    name = 'hr1'
    allowed_domains = ['cjol.com']
    start_urls = ['http://s.cjol.com/f0103-0102-0104/?SearchType=4&Industry=7008-7180&RecentSelected=40']

    def parse(self, response):
        tr_list = response.xpath("//div[@id='searchlist']/ul")[1:-1]
        print("output")
        for tr in tr_list:
            item = {}
            item["职位名称"] = tr.xpath(".//li[contains(@class,'list_type_first')]//a/text()").extract_first()
            item["公司名称"] = tr.xpath(".//li[contains(@class,'list_type_second')]//a/text()").extract_first()
            item["工作地点"] = tr.xpath(".//li[contains(@class,'list_type_third')]/text()").extract_first()
            item["教育水平"] = tr.xpath(".//li[contains(@class,'list_type_fifth')]/text()").extract_first()
            item["工作经验"] = tr.xpath(".//li[contains(@class,'list_type_sixth')]/text()").extract_first()
            item["薪水"] = tr.xpath(".//li[contains(@class,'list_type_seventh')]/text()").extract_first()
            item["发布时间"] = tr.xpath(".//li[contains(@class,'list_type_eighth')]/text()").extract_first()
            # item["职位名称_href"] = tr.xpath(".//li[contains(@class,'list_type_first')]//a/@href").extract_first()

            yield scrapy.Request(
                tr.xpath(".//li[contains(@class,'list_type_first')]//a/@href").extract_first(),
                callback=self.parse_detail,
                meta={"item": item}
            )
        print("output3")


        # 找到下一页的url地址
        next_url = response.xpath("//a[@class='next']/@href").extract()[0]
        # 未解决问题:如何在最后一页安全停止
        print("output4")
        if next_url != "":
            print(next_url)
            next_url = next_url.replace("www", "s")
            print(next_url)
            yield scrapy.Request(
                next_url,
                callback=self.parse
            )
        print("output5")

    def parse_detail(self, response):
        print("output6")
        item = response.meta["item"]
        li_list = response.xpath("//ul[@class='require-jobintro clearfix']")
        item["招聘人数"] = li_list.xpath("./li[4]/text()").extract_first()
        p_list = response.xpath("//div[@class='coninfo-jobdesc']")
        item["职位信息"] = p_list.xpath("./p/text()").extract()
        # 消除提取到的信息中的\n\t\xa0等字符
        item["职位信息"] = [x.strip() for x in item["职位信息"] if x.strip() != '']
        item["职位信息"] = [x.replace("\xa0","") for x in item["职位信息"]]
        dl_list = response.xpath("//div[@class='coninfo-otherinfo']")
        item["关键字"] = dl_list.xpath("./dl[3]//a/text()").extract()
        item["关键字"] = [x.strip() for x in item["关键字"] if x.strip() != '']
        item["关键字"] = [x.replace("\xa0", "") for x in item["关键字"]]

        yield item
        print("detail_page")


