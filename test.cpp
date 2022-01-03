#include <iostream>
#include <curl/curl.h>
#include <string>
#include<gtest/gtest.h>

using namespace std;

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

class API
{
public:

    CURL *curl = curl_easy_init();
    char *url;
    long response_code;
    string request_headers;
    string response_string;
    string response_headers;

    void set_url(string url)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
    }

    void init_curl_request()
    {

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_headers);
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        
        // cout << "Request made succesfully\n " << response_code;
    }

    long get_response_code(){
        return response_code;
    }

    virtual string get_url() = 0;
    virtual string get_response_headers() = 0;
    virtual string get_response_data() =0;
    
    // virtual bool isHTTP() =0;
};



class GETRequest : public API
{
    public:
    GETRequest(string url){
        set_url(url);
        init_curl_request(); 
    }

    string get_url()
    {
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
        return url;
    }

    string get_response_headers()
    {
        return response_headers;
    }

    string get_response_data()
    {
        return response_string;
    }

    void curl_cleanup()
    {
        curl_easy_cleanup(curl);
    }


    
};

class POSTRequest : public API{
    public:
    POSTRequest(string url, string data, string header){
        set_url(url);
        set_data(data);
        add_headers(header);
        cout << "Data set \n";
        init_curl_request();
    }

    void set_data(string input)
    {
        const char *data = input.c_str();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    }
    void add_headers(string header_value){
        struct curl_slist *headers = NULL;
        const char *data = header_value.c_str();
        headers = curl_slist_append(headers, data);
        // headers = curl_slist_append(headers, data);
        // headers = curl_slist_append(headers, "constant: JSESSIONID=B35E9391E2D699737D30AC6652B8A90E");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    string get_url()
    {
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
        return url;
    }

    string get_response_headers(){
        
        return response_headers;
    }

    string get_response_data(){
        return response_string;
    }
};

class Creator{
    public:

    string requestedURL;

    virtual API* FactoryMethod() =0;

    string getUrl(){
        API* api = this->FactoryMethod();
        cout << "URL" << api->get_url();
        return api->get_url();
    }

    string getHeaders(){
        API* api = this->FactoryMethod();
        cout << "RESPONSE HEADERS: " <<  api->get_response_headers();
        return "Success";
    }

    string getResponse(){
        API* api = this->FactoryMethod();
        cout << "RESPONSE DATA" <<  api->get_response_data();
        return "Success";
    }

    long getResponseCode(){
        API* api = this->FactoryMethod();
        return api->response_code;
    }
};

class GetCreator: public Creator{
    public:

    GetCreator(string input){
        requestedURL = input;
    };

    API* FactoryMethod(){
        return new GETRequest(requestedURL);
    }
};

class PostCreator: public Creator{
    public:
    string param;
    string head;

    PostCreator(string input, string data, string h){
        requestedURL = input;
        param = data;
        head = h;
    };

    API* FactoryMethod(){
        return new POSTRequest(requestedURL, param, head);
    }
};
void ClientCode(Creator &creator){
    
    // creator.getUrl();
    creator.getHeaders();
    // creator.getResponse();
    
};


TEST(GETRequestTEST, testingForGET)
{  
    Creator* creator = new GetCreator("https://google.co.in/");
    cout<< creator->getUrl() <<endl;
    ASSERT_STREQ(creator->getUrl().c_str(), "https://google.co.in/");
}


TEST (APItest, isHTTPS){
    Creator* creator = new GetCreator("http://google.co.in/");
    string url = creator->getUrl();
    string sub = url.substr(0,5);
    ASSERT_STREQ(sub.c_str(), "https");
}

TEST (statusCode, is200OK){
    Creator * creator = new GetCreator("https://www.google.co.in/");
    long code = creator->getResponseCode();
    ASSERT_EQ(code, 200);
}

int main()
{

    // Creator* creator = new GetCreator("https://www.google.com");
    // ClientCode(*creator);    
    
    //Creator *creator2 = new PostCreator("http://ptsv2.com/t/lhwz4-1641123474/post", "{\"submit\":\"ishika\",\"submit\":\"1\"}", "Content-Type: application/json");
    //ClientCode(*creator2);

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();

    return 0;
}