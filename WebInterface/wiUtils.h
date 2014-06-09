// created by i-saint
// distributed under Creative Commons Attribution (CC BY) license.
// https://github.com/i-saint/WebInterface

#ifndef wiUtils_h
#define wiUtils_h


inline void wiGetDecodedRequestBody(HTTPServerRequest &request, std::string &out)
{
    if (!request.hasContentLength() || request.getContentLength() > 1024 * 64) {
        return;
    }
    size_t size = (size_t)request.getContentLength();
    std::istream& stream = request.stream();
    std::string encoded_data;
    encoded_data.resize(size);
    stream.read(&encoded_data[0], size);
    Poco::URI::decode(encoded_data, out);
}

template<class F>
inline void wiEachFormData(std::string &form_fata, const F &f)
{
    size_t pos = 0;
    size_t sep = 0;
    for (;;) {
        sep = form_fata.find_first_of('&', pos);
        if (sep == std::string::npos) {
            sep = form_fata.size();
            break;
        }
        f(&form_fata[pos], sep - pos);
        pos = sep + 1;
    }
    f(&form_fata[pos], sep - pos);
}

inline void wiBuildKeyValuePairs(std::string &data, wiKeyValueCont &pairs)
{
    wiEachFormData(data, [&](char *str, size_t len){
        wiKeyValue kv;
        char *k = str;
        char *v = std::find(str, str + len, '=');
        k[len] = '\0';
        *v = '\0';
        ++v;
        kv.key = k;
        kv.value = v;
        pairs.push_back(kv);
    });
}

inline void wiBuildEventData(HTTPServerRequest &request, wiEventPtr evt)
{
    std::string &data = evt->getData();
    wiGetDecodedRequestBody(request, data);
    if (!data.empty()) {
        wiBuildKeyValuePairs(data, evt->getPairs());
    }
}


inline void wiMiliSleep(uint32 millisec)
{
    ::Sleep(millisec);
}

#define wiSPrintf sprintf
#define wiDebugPrint printf
#define wiAssert assert

#endif // wiUtils_h
