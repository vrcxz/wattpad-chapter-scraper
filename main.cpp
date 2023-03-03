#include <iostream>
#include <conio.h>
#include <curl/curl.h>
#include <regex>
#include <string>
#include <vector>

size_t appendToString(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    try {
        s->resize(oldLength + newLength);
    } catch (std::bad_alloc &e) {
        return 0;
    }
    std::copy((char*)contents, (char*)contents + newLength, s->begin() + oldLength);
    return newLength;
}


size_t writefunc(void *ptr, size_t size, size_t nmemb, void *unused)
{
	fwrite(ptr, size, nmemb, (FILE*)unused);
	return size * nmemb;
}



int main() {
    CURL *curl;
    CURLcode res;
    std::string mainwebsite_url = "https://www.wattpad.com/story/62354932-hell-university-published";
    std::string website_data;
    std::regex href_regex("href\\s*=\\s*\"/([0-9]+-[^\"]+)\"");

    std::vector<std::string> href_links;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, mainwebsite_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &appendToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &website_data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::sregex_iterator it(website_data.begin(), website_data.end(), href_regex);
            std::sregex_iterator end;
            while (it != end) {
                std::smatch match = *it;
                std::string joined_url = mainwebsite_url + match[1].str();
                href_links.push_back(joined_url);
                ++it;
            }
        }
        
        //textcolor(YELLOW);
int i=0;
 for ( auto &link : href_links) {
        if(i>=65){return 0;}
        int start = link.find("story/");
  int end = link.find("hell-university-published");
  link.erase(start, end - start + 25);
        std::cout << link << std::endl;
        curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
     std::string filename = std::to_string(i)+"_chapter.html";
	// Open an HTML file for writing
	FILE *fp = fopen(filename.c_str(), "w");
	//set website and output to file
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	
	curl_easy_perform(curl);
        i++;
 }
        
        curl_easy_cleanup(curl);
    }

    return 0;
}
