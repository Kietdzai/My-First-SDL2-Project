#include "JSONObject.h"

void IndentSpacesAndPrint(const long long& spaces){
	for (long long i = 1; i <= spaces; i++){
		std::cerr << " ";
	}
}
void IndentTabsAndPrint(const long long& tabs){
	for (long long i = 1; i <= tabs; i++){
		std::cerr << "	";
	}
}
void IndentTabsAndWrite(const long long& tabs, std::ofstream& fout){
	for (long long i = 1; i <= tabs; i++){
		fout << "	";
	}
}
bool HasNonSpaceChar(const std::string& line){
	for (char ch : line){
		if (ch != ' ') return true;
	}
	return false;
}
std::string codepoint_to_utf8(uint32_t cp) {
    std::string out;
    if (cp <= 0x7F) {
        out.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
        out.push_back(0xC0 | (cp >> 6));
        out.push_back(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        out.push_back(0xE0 | (cp >> 12));
        out.push_back(0x80 | ((cp >> 6) & 0x3F));
        out.push_back(0x80 | (cp & 0x3F));
    } else {
        out.push_back(0xF0 | (cp >> 18));
        out.push_back(0x80 | ((cp >> 12) & 0x3F));
        out.push_back(0x80 | ((cp >> 6) & 0x3F));
        out.push_back(0x80 | (cp & 0x3F));
    }
    return out;
}

std::string decode_uXXXX(const std::string& s) {
    uint32_t idkname = std::stoul(s.substr(2, 4), nullptr, 16);

    // Basic Multilingual Plane (on Google)
    if (idkname < 0xD800 || idkname > 0xDBFF)
        return codepoint_to_utf8(idkname);
    return "Oh no!";
}
std::string decode_uXXXX_uXXXX(const std::string& s) {
    // Supplementary Planes (idk mean of this word, I just found it on Google)
    if (s.size() != 12 || s[0] != '\\' || s[1] != 'u' || s[6] != '\\' || s[7] != 'u') throw std::runtime_error("Invalid \\uXXXX\\uXXXX escape");
    uint32_t high = std::stoul(s.substr(2, 4), nullptr, 16);
    uint32_t low  = std::stoul(s.substr(8, 4), nullptr, 16);

    // Validate surrogate ranges
    if (high < 0xD800 || high > 0xDBFF)
        throw std::runtime_error("Invalid high surrogate");

    if (low < 0xDC00 || low > 0xDFFF)
        throw std::runtime_error("Invalid low surrogate");

    uint32_t codepoint =
        0x10000 +
        ((high - 0xD800) << 10) +
        (low - 0xDC00);

    return codepoint_to_utf8(codepoint);
}

std::string ConvertVariantToString(const ___VALUETYPE___& target){
	if (auto temp = std::get_if<std::string>(&target.data)) return *temp;
	throw std::invalid_argument("Argument is not a std::string type.");
}
bool ConvertVariantToBool(const ___VALUETYPE___& target){
	if (auto temp = std::get_if<bool>(&target.data)) return *temp;
	throw std::invalid_argument("Argument is not a boolean type.");
}
long long ConvertVariantToLL(const ___VALUETYPE___& target){
	if (auto temp = std::get_if<long long>(&target.data)) return *temp;
	throw std::invalid_argument("Argument is not a long long type.");
}
double ConvertVariantToDouble(const ___VALUETYPE___& target){
	if (auto temp = std::get_if<double>(&target.data)) return *temp;
	throw std::invalid_argument("Argument is not double type.");
}
std::nullptr_t ConvertVariantToNullptr(const ___VALUETYPE___& target){
	if (auto temp = std::get_if<std::nullptr_t>(&target.data)) return nullptr;
	throw std::invalid_argument("Argument is not a nullptr type.");
}
___MAPTYPE___ ConvertVariantToMap(const ___VALUETYPE___& target){
	if (auto temp = std::get_if<___MAPTYPE___>(&target.data)) return *temp;
	throw std::invalid_argument("Argument is not a std::variant type to cast to std::map type.");
}
// Warning: you use these function when you're ensure that it is not a NESTED map
std::map<std::string, long long> ConvertMapToLL(const ___MAPTYPE___& target){
	// Use when you known this map only have value datatype is long long
	// Init Area
	std::map<std::string, long long> map = {};
	// Code Area
	for (const auto& pair : target){
		map[pair.first] = ConvertVariantToLL(pair.second);
	}
	return map;
}
std::map<std::string, std::string> ConvertMapToString(const ___MAPTYPE___& target){
	// Use when you known this map only have value datatype is std::string
	// Init Area
	std::map<std::string, std::string> map = {};
	// Code Area
	for (const auto& pair : target){
		map[pair.first] = ConvertVariantToString(pair.second);
	}
	return map;
}
std::map<std::string, std::nullptr_t> ConvertMapToNullptr(const ___MAPTYPE___& target){
	// Use when you known this map only have value datatype is std::nullptr_t
	// Init Area
	std::map<std::string, std::nullptr_t> map = {};
	// Code Area
	for (const auto& pair : target){
		map[pair.first] = ConvertVariantToNullptr(pair.second);
	}
	return map;
}
std::map<std::string, double> ConvertMapToDouble(const ___MAPTYPE___& target){
	// Use when you known this map only have value datatype is double
	// Init Area
	std::map<std::string, double> map = {};
	// Code Area
	for (const auto& pair : target){
		map[pair.first] = ConvertVariantToDouble(pair.second);
	}
	return map;
}

std::ostream& operator<<(std::ostream& os, ntk::JSONObject& obj){
    obj.SeeInsideThisObject();

    return os;
}
ntk::JSONObject& operator>>(const ___MAPTYPE___& giver, ntk::JSONObject& receiver){
	receiver.map = giver;

	return receiver;
}
void SetUnicodeForTheOutput(){
	SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}