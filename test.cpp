#include<bits/stdc++.h>
#include <curl/curl.h>

using namespace std;

class API
{
public:
    CURL *curl = curl_easy_init();
    char *url;
    string request_headers;
    string response_string;
    string response_headers;

    // virtual void set_url(string url);
    virtual string get_url() = 0;
    virtual string get_response_headers() = 0;
    virtual void init_curl_request() = 0;
};

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

class GETRequest : public API
{
    private:
    void set_url(string url)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
    }
    
    public:
    GETRequest(string url){
        set_url(url);
        init_curl_request(); //change made
    }

    void init_curl_request()
    {

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_headers);
        curl_easy_perform(curl);
        cout << "Request made succesfully\n";
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
    POSTRequest(string url){
        cout << url  << endl;
    }

    void init_curl_request()
    {

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_headers);
        curl_easy_perform(curl);
    }

    string get_url()
    {
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
        return url;
    }

    string get_response_headers(){
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
        
        return "success";
    }
};

class Creator{
    public:

    string requestedURL;

    CURL *curl = curl_easy_init();
    char *url;
    string request_headers;
    string response_string;
    string response_headers;
    

    void init_curl_request()
    {

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response_headers);
        curl_easy_perform(curl);
    }

    virtual API* FactoryMethod() =0;

    string getUrl(){
        API* api = this->FactoryMethod();
        cout <<  api->get_url();
        return "Success";
    }

    string getHeaders(){
        API* api = this->FactoryMethod();
        cout <<  api->get_response_headers();
        return "Success";
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

    PostCreator(string input){
        requestedURL = input;
    };

    API* FactoryMethod(){
        return new POSTRequest(requestedURL);
    }
};

void ClientCode(Creator &creator){
    
    creator.getUrl();
    creator.getHeaders();
};

int main()
{

    Creator* creator = new GetCreator("https://www.google.co.in/");
    ClientCode(*creator);

    // GETRequest request("https://www.google.co.in/");

    // // request.set_url("https://www.google.co.in/");
    // request.init_curl_request();
    // string myurl = request.get_url();
    // request.curl_cleanup();

    // cout << "gud moning" << endl;
    // cout << myurl << endl;
    // cout << request.get_response_headers();
    // // cout << request.get_response_data();
    return 0;
}