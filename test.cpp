#include<bits/stdc++.h>
#include <curl/curl.h>

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
        cout << "Request made succesfully\n";
    }

    virtual string get_url() = 0;
    virtual string get_response_headers() = 0;
    // virtual void init_curl_request() = 0;
    virtual string get_response_data() =0;
};



class GETRequest : public API
{
    public:
    GETRequest(string url){
        set_url(url);
        init_curl_request(); //change made
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
    POSTRequest(string url, string data){
        set_url(url);
        set_data(data);
        cout << "Data set \n";
        init_curl_request();
    }

    void set_data(string input)
    {
        const char *data = input.c_str();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
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
        cout << "URL: " << api->get_url();
        return "Success";
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

    PostCreator(string input, string data){
        requestedURL = input;
        param = data;
    };

    API* FactoryMethod(){
        return new POSTRequest(requestedURL, param);
    }
};

void ClientCode(Creator &creator){
    
    // creator.getUrl();
    // creator.getHeaders();
    creator.getResponse();
};

int main()
{

    // Creator* creator = new GetCreator("https://api.kanye.rest");
    // ClientCode(*creator);

    Creator *creator2 = new PostCreator("https://www.google.com", "submit = 1");
    ClientCode(*creator2);

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