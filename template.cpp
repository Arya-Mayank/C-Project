#include <iostream>
#include <curl/curl.h>
#include <string>
#include<memory>
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
        cout << "Get object created" <<endl;
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

    // POSTRequest(string url){
    //     set_url(url);
    //     init_curl_request();
    //     cout << "Post object created" <<endl;
    // }

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


template <class T>
class Creator{
    public:
    shared_ptr<T> ptr;

    string requestedURL;

    Creator<T>(string input){
        // T t(input);
        shared_ptr<T> p1(new T(input));
        ptr = p1;
    }

    Creator<T>(string input, string data, string header){
        shared_ptr<T> p2(new T(input, data, header));
        ptr = p2;
    }

    string GETURL();
    string GETRESPONSE();
    string GETHEADERS();
};

template <class T>
string Creator<T>::GETURL(){
    cout << ptr->get_url();
    return ptr->get_url();
}

template <class T>
string Creator<T>::GETRESPONSE(){
    cout << ptr->get_response_data();
    return ptr->get_response_data();
}

template <class T>
string Creator<T>::GETHEADERS(){
    cout << ptr->get_response_headers();
    return ptr->get_response_headers();
}






////////////////////MAIN CODE //////////////////////////////////

int main()
{

    Creator<POSTRequest> creator("https://www.google.com/", "{\"submit\":\"ishika\",\"submit\":\"1\"}", "Content-Type: application/json");
    creator.GETRESPONSE();
    
    Creator<GETRequest> creator2("https://www.google.com/");
    creator2.GETURL();
    creator2.GETHEADERS();

    return 0;
}


























/////////////////////CLIENT CODE //////////////////////////////
// template <class T>
// void ClientCode(Creator<T> &creator){
    
//     cout << creator.GETURL();
//     // creator.getUrl();
//     // creator.getHeaders();
//     // creator.getResponse();
    
// };