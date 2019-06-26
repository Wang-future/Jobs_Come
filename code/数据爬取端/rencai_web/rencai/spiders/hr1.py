# -*- coding: utf-8 -*-
import scrapy

class Hr1Spider(scrapy.Spider):
    name = 'hr1'
    allowed_domains = ['cjol.com']
    start_urls = ['http://s.cjol.com/f0103-0102-0104/?SearchType=4&Industry=7008-7180&RecentSelected=40']

    def parse(self, response):
        tr_list = response.xpath("//div[@id='searchlist']/ul")[1:-1]
        # print("output")
        for tr in tr_list:
            item = {}
            item["POSITIONTYPE"] = tr.xpath(".//li[contains(@class,'list_type_first')]//a/text()").extract_first()
            item["COMPANY"] = tr.xpath(".//li[contains(@class,'list_type_second')]//a/text()").extract_first()
            item["WORKPLACE"] = tr.xpath(".//li[contains(@class,'list_type_third')]/text()").extract_first()
            item["DIPLOMA"] = tr.xpath(".//li[contains(@class,'list_type_fifth')]/text()").extract_first()
            item["EXPERIENCE"] = tr.xpath(".//li[contains(@class,'list_type_sixth')]/text()").extract_first()
            item["SALARY"] = tr.xpath(".//li[contains(@class,'list_type_seventh')]/text()").extract_first()
            item["DATE"] = tr.xpath(".//li[contains(@class,'list_type_eighth')]/text()").extract_first()
            # item["职位名称_href"] = tr.xpath(".//li[contains(@class,'list_type_first')]//a/@href").extract_first()

            yield scrapy.Request(
                tr.xpath(".//li[contains(@class,'list_type_first')]//a/@href").extract_first(),
                callback=self.parse_detail,
                meta={"item": item}
            )
        # print("output3")


        # 找到下一页的url地址
        next_url = response.xpath("//a[@class='next']/@href").extract()[0]

        # print("output4")
        if next_url != "":
            #print(next_url)
            next_url = next_url.replace("www", "s")
            #print(next_url)
            yield scrapy.Request(
                next_url,
                callback=self.parse
            )
        # print("output5")

    def parse_detail(self, response):
        # print("output6")
        item = response.meta["item"]
        li_list = response.xpath("//ul[@class='require-jobintro clearfix']")
        item["NUMBER"] = li_list.xpath("./li[4]/text()").extract_first()
        p_list = response.xpath("//div[@class='coninfo-jobdesc']")
        item["SUPPLEMENT"] = p_list.xpath("./p/text()").extract()
        # 消除提取到的信息中的\n\t\xa0等字符
        item["SUPPLEMENT"] = [x.strip() for x in item["SUPPLEMENT"] if x.strip() != '']
        item["SUPPLEMENT"] = [x.replace("\xa0","") for x in item["SUPPLEMENT"]]
        dl_list = response.xpath("//div[@class='coninfo-otherinfo']")
        item["KEYWORD"] = dl_list.xpath("./dl[3]//a/text()").extract()
        item["KEYWORD"] = [x.strip() for x in item["KEYWORD"] if x.strip() != '']
        item["KEYWORD"] = [x.replace("\xa0", "") for x in item["KEYWORD"]]

        yield item
        # print("detail_page")


