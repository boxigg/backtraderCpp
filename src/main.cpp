
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <time.h>
#include <assert.h>
#include "ta_libc.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "cpr/cpr.h"

#include "Disruptor/IWorkHandler.h"
#include "Disruptor/Disruptor.h"
#include "Disruptor/ThreadPerTaskScheduler.h"
#include "Disruptor/SpinWaitWaitStrategy.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <cstdlib>
#include <time.h>
#include <chrono>

void testCpr()
{
    //测试http访问是否有效
    auto response = cpr::Get(cpr::Url{ "https://www.yahoo.com" },
        cpr::Parameters{ { "hello", "world" } },
        cpr::VerifySsl(false));
    std::cout << response.text << std::endl;

    //test2 
    auto r2 = cpr::Get(cpr::Url{ "https://api.github.com/repos/whoshuu/cpr/contributors" },
        cpr::Authentication{ "user", "pass" },
        cpr::Parameters{ {"anon", "true"}, {"key", "value"} });
    std::cout << r2.status_code << std::endl;
    std::cout << r2.header["content-type"] << std::endl;
    std::cout << r2.text << std::endl;
    //test 3

    //test4
    auto r4 = cpr::Post(cpr::Url{ "http://www.httpbin.org/post" },
        cpr::Payload{ {"key", "value"} });
    std::cout << r4.text << std::endl;

    //test5
    auto r5 = cpr::Get(cpr::Url{ "http://www.httpbin.org/get" },
        cpr::Parameters{ {"hello", "world"}, {"stay", "cool"} });
    std::cout << r5.url << std::endl; // http://www.httpbin.org/get?hello=world&stay=cool
    std::cout << r5.text << std::endl;

    //test 6
     // Constructing it outside
    auto parameters = cpr::Parameters{ {"hello", "world"}, {"stay", "cool"} };
    auto r_outside = cpr::Get(cpr::Url{ "http://www.httpbin.org/get" }, parameters);
    std::cout << r_outside.url << std::endl; // http://www.httpbin.org/get?hello=world&stay=cool
    std::cout << r_outside.text << std::endl; // Same text response as above


    //test7
    //This sends up "key=value" as a "x-www-form-urlencoded" pair in the POST request.
    //To send data raw and unencoded, use Body instead of Payload :

    auto r7 = cpr::Post(cpr::Url{ "http://www.httpbin.org/post" },
        cpr::Body{ "This is raw POST data" },
        cpr::Header{ {"Content-Type", "text/plain"} });
    std::cout << r7.text << std::endl;
    //test 8
    auto r8 = cpr::Post(cpr::Url{ "http://www.httpbin.org/post" },
        cpr::Multipart{ {"key", "large value"},
                       {"name", cpr::File{"d:/temp/log.log"}} });
    std::cout << r8.text << std::endl;


    //test 9
    // STL containers like vector, string, etc.
    std::vector<char> content{ 't', 'e', 's', 't' };
    auto r9 = cpr::Post(cpr::Url{ "http://www.httpbin.org/post" },
        cpr::Multipart{ {"key", "large value"},
                       {"name", cpr::Buffer{content.begin(), content.end(), "filename.txt"}} });

    // C-style pointers
    const char *content2 = "test";
    int length = 4;
    auto r10 = cpr::Post(cpr::Url{ "http://www.httpbin.org/post" },
        cpr::Multipart{ {"key", "large value"},
                       {"name", cpr::Buffer{content2, content2 + length, "filename.txt"}} });
}

void testJson()
{
    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
    writer.StartObject();

    writer.Key("userList");
    writer.StartArray();

    writer.StartObject();
    writer.Key("userId");
    writer.Uint(20);
    writer.Key("productIdList");
    writer.StartArray();
    for (unsigned i = 1; i < 3; i++)
        writer.Uint(i);
    writer.EndArray();
    writer.EndObject();

    writer.StartObject();
    writer.Key("userId");
    writer.Uint(21);
    writer.Key("productIdList");
    writer.StartArray();
    for (unsigned i = 1; i < 3; i++)
        writer.Uint(i);
    writer.EndArray();
    writer.EndObject();

    writer.EndArray();
    writer.EndObject();
    std::cout << buff.GetString() << std::endl;


    // 1. Parse a JSON string into DOM.
    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rapidjson::Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    rapidjson::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer2(buffer);
    d.Accept(writer2);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    //写文件
    std::string filename = "d:/temp/test.json";
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        outfile << buffer.GetString() << std::endl;
        outfile.close();
    }
    else std::cerr << "Error! Unable to open file:" << filename;
}

//https ://blog.csdn.net/u012234115/article/details/81513747 
void testTalib()
{
    std::cout << "ta-lib test" << std::endl;

    // init ta-lib context
    TA_RetCode retcode;
    retcode = TA_Initialize();
    //assert(retcode == TA_SUCCESS);

    // comput moving average price
    TA_Real    close_price_array[400] = { 0 };
    // construct random price
    srand((unsigned)time(0));
    for (int i = 0; i < 400; i++)
        close_price_array[i] = rand() % 50 + 100;

    TA_Real    out[400] = { 0 };
    TA_Integer out_begin = 0;
    TA_Integer out_nb_element = 0;

    retcode = TA_MA(0, 399,
        &close_price_array[0],
        30, TA_MAType_SMA,
        &out_begin, &out_nb_element, &out[0]);

    std::cout << "---- compute output ----" << std::endl;
    std::cout << "out_begin_index: " << out_begin << std::endl;
    std::cout << "out_nb_element: " << out_nb_element << std::endl;
    std::cout << "out price array: " << std::endl;
    for (auto &price : out)
        std::cout << price << " ";
    assert(retcode == TA_SUCCESS);

    //test boll
    int required_close_num = 0;
    if (1 == 1)
    {
        required_close_num = 400;
    }
    TA_Real *outRealUpperBand = new double[required_close_num];
    TA_Real *outRealMiddleBand = new double[required_close_num];
    TA_Real *outRealLowerBand = new double[required_close_num];
    /*
    TA_Real    outRealUpperBand[400] = { 0 };
    TA_Real    outRealMiddleBand[400] = { 0 };
    TA_Real    outRealLowerBand[400] = { 0 };
    */
    retcode = TA_BBANDS(0,
        399,
        &close_price_array[0],
        30,
        1.5,
        1.5,
        TA_MAType_SMA,
        &out_begin, &out_nb_element,
        &outRealUpperBand[0], &outRealMiddleBand[0], &outRealLowerBand[0]);

    for (int i = 0; i < out_nb_element; i++)
    {
        printf("Day %d = %f,%f,%f\n", out_begin + i, outRealUpperBand[i], outRealMiddleBand[i], outRealLowerBand[i]);
    }
    for (int i = 0; i < 399; i++)
    {
        printf("boll array: %d = %f, %f, %f\n", i, outRealUpperBand[i], outRealMiddleBand[i], outRealLowerBand[i]);
    }
    assert(retcode == TA_SUCCESS);

    retcode = TA_Shutdown();
    assert(retcode == TA_SUCCESS);
}

typedef char char_19[19];
typedef char char_21[21];
typedef char char_64[64];
typedef char char_7[7];
typedef char char_9[9];
typedef char char_30[30];
typedef char char_31[31];
typedef char char_16[16];
typedef char char_13[13];
typedef char char_2[2];
typedef char char_11[11];

struct LFMarketDataField
{
    char_13  	TradingDay;            //交易日
    char_31  	InstrumentID;          //合约代码
    char_9   	ExchangeID;            //交易所代码
    char_64  	ExchangeInstID;        //合约在交易所的代码
    double   	LastPrice;             //最新价
    double   	PreSettlementPrice;    //上次结算价
    double   	PreClosePrice;         //昨收盘
    double   	PreOpenInterest;       //昨持仓量
    double   	OpenPrice;             //今开盘
    double   	HighestPrice;          //最高价
    double   	LowestPrice;           //最低价
    int      	Volume;                //数量
    double   	Turnover;              //成交金额
    double   	OpenInterest;          //持仓量
    double   	ClosePrice;            //今收盘
    double   	SettlementPrice;       //本次结算价
    double   	UpperLimitPrice;       //涨停板价
    double   	LowerLimitPrice;       //跌停板价
    double   	PreDelta;              //昨虚实度
    double   	CurrDelta;             //今虚实度
    char_13  	UpdateTime;            //最后修改时间
    int      	UpdateMillisec;        //最后修改毫秒
    double   	BidPrice1;             //申买价一
    int      	BidVolume1;            //申买量一
    double   	AskPrice1;             //申卖价一
    int      	AskVolume1;            //申卖量一
    double   	BidPrice2;             //申买价二
    int      	BidVolume2;            //申买量二
    double   	AskPrice2;             //申卖价二
    int      	AskVolume2;            //申卖量二
    double   	BidPrice3;             //申买价三
    int      	BidVolume3;            //申买量三
    double   	AskPrice3;             //申卖价三
    int      	AskVolume3;            //申卖量三
    double   	BidPrice4;             //申买价四
    int      	BidVolume4;            //申买量四
    double   	AskPrice4;             //申卖价四
    int      	AskVolume4;            //申卖量四
    double   	BidPrice5;             //申买价五
    int      	BidVolume5;            //申买量五
    double   	AskPrice5;             //申卖价五
    int      	AskVolume5;            //申卖量五
};


inline struct LFMarketDataField parseFrom(const struct LFMarketDataField& ori)
{
    struct LFMarketDataField res = {};
    memcpy(res.TradingDay, ori.TradingDay, 9);
    memcpy(res.InstrumentID, ori.InstrumentID, 31);
    memcpy(res.ExchangeID, ori.ExchangeID, 9);
    memcpy(res.ExchangeInstID, ori.ExchangeInstID, 64);
    res.LastPrice = ori.LastPrice;
    res.PreSettlementPrice = ori.PreSettlementPrice;
    res.PreClosePrice = ori.PreClosePrice;
    res.PreOpenInterest = ori.PreOpenInterest;
    res.OpenPrice = ori.OpenPrice;
    res.HighestPrice = ori.HighestPrice;
    res.LowestPrice = ori.LowestPrice;
    res.Volume = ori.Volume;
    res.Turnover = ori.Turnover;
    res.OpenInterest = ori.OpenInterest;
    res.ClosePrice = ori.ClosePrice;
    res.SettlementPrice = ori.SettlementPrice;
    res.UpperLimitPrice = ori.UpperLimitPrice;
    res.LowerLimitPrice = ori.LowerLimitPrice;
    res.PreDelta = ori.PreDelta;
    res.CurrDelta = ori.CurrDelta;
    memcpy(res.UpdateTime, ori.UpdateTime, 9);
    res.UpdateMillisec = ori.UpdateMillisec;
    res.BidPrice1 = ori.BidPrice1;
    res.BidVolume1 = ori.BidVolume1;
    res.AskPrice1 = ori.AskPrice1;
    res.AskVolume1 = ori.AskVolume1;
    res.BidPrice2 = ori.BidPrice2;
    res.BidVolume2 = ori.BidVolume2;
    res.AskPrice2 = ori.AskPrice2;
    res.AskVolume2 = ori.AskVolume2;
    res.BidPrice3 = ori.BidPrice3;
    res.BidVolume3 = ori.BidVolume3;
    res.AskPrice3 = ori.AskPrice3;
    res.AskVolume3 = ori.AskVolume3;
    res.BidPrice4 = ori.BidPrice4;
    res.BidVolume4 = ori.BidVolume4;
    res.AskPrice4 = ori.AskPrice4;
    res.AskVolume4 = ori.AskVolume4;
    res.BidPrice5 = ori.BidPrice5;
    res.BidVolume5 = ori.BidVolume5;
    res.AskPrice5 = ori.AskPrice5;
    res.AskVolume5 = ori.AskVolume5;
    return res;
}

// 定义事件体
struct testEvent
{
    int64_t id{ 0 };
    LFMarketDataField md{0};
};

// 继承事件处理器接口
class Worker : public Disruptor::IWorkHandler< testEvent >
{
public:
    explicit Worker() {}
    // 重写事件处理回调函数
    void onEvent(testEvent& event) override
    {
        //std::cout << " _actuallyProcessed:" << _actuallyProcessed;
        _actuallyProcessed++;
    }

    void print()
    {
        std::cout << "_actuallyProcessed:" << _actuallyProcessed << std::endl;
    }
private:
    int32_t _actuallyProcessed{ 0 };
};

class Producer
{
public:
    Producer(std::int32_t ringBufferSize, std::int32_t workerCount)
    {
        m_ringBufferSize = ringBufferSize;
        m_workerCount = workerCount;
        // 创建调度器
        m_ptrTaskScheduler = std::make_shared<Disruptor::ThreadPerTaskScheduler>();
        // 创建Disruptor
        m_ptrDisruptor = std::make_shared< Disruptor::disruptor<testEvent>>([]() { return testEvent(); } ,
            m_ringBufferSize,
            m_ptrTaskScheduler,
            Disruptor::ProducerType::Single,
            std::make_shared<Disruptor::SpinWaitWaitStrategy>()
            );
        // 创建事件处理器
        for (size_t i = 0; i < m_workerCount; i++)
        {
            m_workers.push_back(std::make_shared< Worker >());
        }
        // 绑定
        m_ptrDisruptor->handleEventsWithWorkerPool(m_workers);
    }

    ~Producer()
    {
        stop();
    }

    void start()
    {
        m_ptrTaskScheduler->start();
        m_ptrDisruptor->start();
    }

    void push(const testEvent &event)
    {
        auto ringBuffer = m_ptrDisruptor->ringBuffer();
        auto nextSequence = ringBuffer->next();
        (*ringBuffer)[nextSequence] = event;
        ringBuffer->publish(nextSequence);
    }

protected:
    void stop()
    {
        m_ptrDisruptor->shutdown();
        m_ptrTaskScheduler->stop();
    }

private:
    std::shared_ptr<Disruptor::ThreadPerTaskScheduler> m_ptrTaskScheduler;
    std::shared_ptr<Disruptor::disruptor<testEvent> > m_ptrDisruptor;
    std::vector<std::shared_ptr<Disruptor::IWorkHandler<testEvent> > > m_workers;
    std::int32_t m_ringBufferSize{ 1024 };
    std::int32_t m_workerCount{ 1 };
    std::int32_t m_schedulerCount{ 1 };
};


void testDisruptor()
{
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    Producer producer(1024, 1);
    producer.start();
    testEvent e;


    LFMarketDataField ori;

    memcpy(ori.TradingDay, "2019-01-23", 9);
    memcpy(ori.InstrumentID, "rb1905", 31);
    memcpy(ori.ExchangeID, "SHFE", 9);
    memcpy(ori.ExchangeInstID, "rb1905", 64);
    ori.LastPrice = 3567;
    ori.PreSettlementPrice = 3400;
    ori.PreClosePrice = 3456;
    ori.PreOpenInterest = 1234;
    ori.OpenPrice = 3567;
    ori.HighestPrice = 1234;
    ori.LowestPrice = 1234;
    ori.Volume = 1234;
    ori.Turnover = 12345;
    ori.OpenInterest = 123456;
    ori.ClosePrice = 3434;
    ori.SettlementPrice = 123;
    ori.UpperLimitPrice = 5555;
    ori.LowerLimitPrice = 3333;
    ori.PreDelta = 12;
    ori.CurrDelta = 12;
    memcpy(ori.UpdateTime, "18:18:18", 9);
    ori.UpdateMillisec = 500;
    ori.BidPrice1 = 3456;
    ori.BidVolume1 = 3456;
    ori.AskPrice1 = 3456;
    ori.AskVolume1 = 3456;
    ori.BidPrice2 = 1;
    ori.BidVolume2 = 2;
    ori.AskPrice2 = 3;
    ori.AskVolume2 = 4;
    ori.BidPrice3 = 5;
    ori.BidVolume3 = 6;
    ori.AskPrice3 = 7;
    ori.AskVolume3 = 8;
    ori.BidPrice4 = 9;
    ori.BidVolume4 = 10;
    ori.AskPrice4 = 11;
    ori.AskVolume4 = 12;
    ori.BidPrice5 = 13;
    ori.BidVolume5 = 14;
    ori.AskPrice5 = 15;
    ori.AskVolume5 = 16;
    
    for (size_t i = 0; i < 2000000; i++)
    {
        e.id = i;
        producer.push(e);
    }

    int64_t average = 0;
    for (int loop = 0; loop < 100; loop++)
    {
        int64_t start_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
            ).count();
        for (size_t i = 0; i < 2000000; i++)
        {
            e.id = i;
            memcpy(e.md.TradingDay, ori.TradingDay, 9);
            memcpy(e.md.InstrumentID, ori.InstrumentID, 31);
            memcpy(e.md.ExchangeID, ori.ExchangeID, 9);
            memcpy(e.md.ExchangeInstID, ori.ExchangeInstID, 64);
            e.md.LastPrice = ori.LastPrice;
            e.md.PreSettlementPrice = ori.PreSettlementPrice;
            e.md.PreClosePrice = ori.PreClosePrice;
            e.md.PreOpenInterest = ori.PreOpenInterest;
            e.md.OpenPrice = ori.OpenPrice;
            e.md.HighestPrice = ori.HighestPrice;
            e.md.LowestPrice = ori.LowestPrice;
            e.md.Volume = ori.Volume;
            e.md.Turnover = ori.Turnover;
            e.md.OpenInterest = ori.OpenInterest;
            e.md.ClosePrice = ori.ClosePrice;
            e.md.SettlementPrice = ori.SettlementPrice;
            e.md.UpperLimitPrice = ori.UpperLimitPrice;
            e.md.LowerLimitPrice = ori.LowerLimitPrice;
            e.md.PreDelta = ori.PreDelta;
            e.md.CurrDelta = ori.CurrDelta;
            memcpy(e.md.UpdateTime, ori.UpdateTime, 9);
            e.md.UpdateMillisec = ori.UpdateMillisec;
            e.md.BidPrice1 = ori.BidPrice1;
            e.md.BidVolume1 = ori.BidVolume1;
            e.md.AskPrice1 = ori.AskPrice1;
            e.md.AskVolume1 = ori.AskVolume1;
            e.md.BidPrice2 = ori.BidPrice2;
            e.md.BidVolume2 = ori.BidVolume2;
            e.md.AskPrice2 = ori.AskPrice2;
            e.md.AskVolume2 = ori.AskVolume2;
            e.md.BidPrice3 = ori.BidPrice3;
            e.md.BidVolume3 = ori.BidVolume3;
            e.md.AskPrice3 = ori.AskPrice3;
            e.md.AskVolume3 = ori.AskVolume3;
            e.md.BidPrice4 = ori.BidPrice4;
            e.md.BidVolume4 = ori.BidVolume4;
            e.md.AskPrice4 = ori.AskPrice4;
            e.md.AskVolume4 = ori.AskVolume4;
            e.md.BidPrice5 = ori.BidPrice5;
            e.md.BidVolume5 = ori.BidVolume5;
            e.md.AskPrice5 = ori.AskPrice5;
            e.md.AskVolume5 = ori.AskVolume5;
            producer.push(e);
        }
        int64_t end_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
            ).count();

        average += (end_nano - start_nano);
    }
    std::cout << "use time:" << std::to_string(average / 100) << std::endl;


    //std::this_thread::sleep_for(std::chrono::seconds(10));
}


int main(int argc, char* argv[]) {
    //testTalib();
    //testCpr();
    //testJson();
    testDisruptor();//warm up

    system("pause");
    return 0;
}