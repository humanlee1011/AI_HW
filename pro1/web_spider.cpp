#include <iostream>
#include <string>
#include <sstream>
/* using libcurl to finish http get request*/
#include "curl/curl.h"
#include <regex>
#include <vector>
#include <map>

using namespace std;

size_t write_data(void *ptr, size_t size, size_t nmemb, void* stream) {
  string data((const char*) ptr, (size_t) size * nmemb);
  *((stringstream*) stream) << data << endl;
  return size* nmemb;
}

vector<string> extractHtml(string& html) {
  string reg("href=\"([^ ])\"$");
  smatch m;
  vector<string> urls;
  while (regex_search(html, m, reg))
    urls.push_back(m.suffix().str());
  return urls;
}


vector<string> getUrls(string& url) {
  CURL *curl = curl_easy_init();
  stringstream out;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
    CURLcode res = curl_easy_perform(curl);
  }
  return extractHtml(out.str());
}

bool dfs(string& node, vector<string>& path) {

}

bool dfs_limit(string& root) {

}

bool search(string& source, string& sink, vector<string>& ans) {
  dfs
}

void testURL() {
  string s("www.baidu.com");
  vector<string> urls = getUrls(s);
  for (auto url: urls)
    cout << url << endl;
}

int main() {
  testURL();
  // string sink, source;
  // getline(cin, source);
  // getline(cin, sink);

  //search(source, sink, ans);

}
