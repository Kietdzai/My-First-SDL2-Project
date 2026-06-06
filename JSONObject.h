#pragma once
#include <iostream> // using for most of functions (std::cerr)
#include <map> // using to stores key(s) and value(s)
#include <string> // a datatype
#include <fstream> // using for open() function in the class
#include <variant> // using for Value in each pair
#include <windows.h> // using for SetUnicodeForTheOutput() function
#include <stdexcept>
#include <cstdint>

#ifndef VALUE_AND_MAP
#define VALUE_AND_MAP
struct Value; 

using ___MAPTYPE___ = std::map<std::string, Value>;
struct Value {
    using VariantType = std::variant<long long, double, bool, std::string, std::nullptr_t, ___MAPTYPE___>;
    VariantType data;
    explicit Value() : data(nullptr) {}
    template<typename T>
    explicit Value(T&& val) : data(std::forward<T>(val)) {}
    bool operator==(const Value& other) const{
    	return this->data == other.data;
    }
};

using ___VALUETYPE___ = Value;
#endif
void IndentSpacesAndPrint(const long long& spaces);
void IndentTabsAndPrint(const long long& tabs);
void IndentTabsAndWrite(const long long& tabs, std::ofstream& fout);
bool HasNonSpaceChar(const std::string& line);
std::string codepoint_to_utf8(uint32_t cp);

std::string decode_uXXXX(const std::string& s);
std::string decode_uXXXX_uXXXX(const std::string& s);

std::string ConvertVariantToString(const ___VALUETYPE___& target);
bool ConvertVariantToBool(const ___VALUETYPE___& target);
long long ConvertVariantToLL(const ___VALUETYPE___& target);
double ConvertVariantToDouble(const ___VALUETYPE___& target);
std::nullptr_t ConvertVariantToNullptr(const ___VALUETYPE___& target);
___MAPTYPE___ ConvertVariantToMap(const ___VALUETYPE___& target);
// Warning: you use these function when you're ensure that it is not a NESTED map
std::map<std::string, long long> ConvertMapToLL(const ___MAPTYPE___& target);
std::map<std::string, std::string> ConvertMapToString(const ___MAPTYPE___& target);
std::map<std::string, std::nullptr_t> ConvertMapToNullptr(const ___MAPTYPE___& target);
std::map<std::string, double> ConvertMapToDouble(const ___MAPTYPE___& target);

namespace ntk {
class JSONObject {
private:
	std::string context = "";
	std::string raw_file_context;
	std::map<unsigned long long, std::string> raw_file;
	const std::string file_name;
	unsigned long long errs = 0;
	unsigned long long lines = 0; // How many lines are there in raw_file
	std::string formatter(); // forward declaration
	bool CheckBeforeFormatRawFileContext(){
		// Init Area
		bool is_in = false;
		std::map<unsigned long long, std::string> raw_file = this-> raw_file; // Backup raw_file variable
		long long counter = 0; // Count how many double qoutes
		bool checked = false; // Mark if checked is this JSON object opens with a '{'
		bool close_check = false; // Same as how it works as above variable but check if this JSON object closes with a '}'
		bool has_error = false;
		// Code Area
		// Check if this JSON Object is empty
		for (auto& pair : raw_file){
			pair.second = formatter(pair.second);
			if (pair.second != std::string("")){
				if (pair.second[0] == '{' && !checked) {checked = true;}
				else if (pair.second[0] != '{' && !checked) {std::cerr << "\033[1m" << file_name << ":" << pair.first << ":\033[0m \033[1;31merror:\033[0m A JSON object must be open with a curly brace ('{')." << std::endl; errs++; checked = true; has_error = true;}
			}
			if (pair.second[0] == ',') {std::cerr << "\033[1m" << file_name << ":" << pair.first << ":\033[0m \033[1;31merror:\033[0m A comma must be followed by a valid JSON value." << std::endl; errs++; has_error = true;} // Comma checker, if it before another pair, this line is going to error ~
			for (unsigned long long pos = 0; pos < pair.second.length(); pos++){
				if (pos > 2){
					if (pair.second[pos] == '"' && pair.second[pos - 1] != '\\' && !is_in) {is_in = true; counter++;}
					else if (pair.second[pos] == '"' && (pair.second[pos - 1] != '\\' || pair.second[pos - 2] == '\\') && is_in) {is_in = false; counter--;}
				}
				else if (pos > 0){
					if (pair.second[pos] == '"' && pair.second[pos - 1] != '\\' && !is_in) {is_in = true; counter++;}
					else if (pair.second[pos] == '"' && pair.second[pos - 1] != '\\' && is_in) {is_in = false; counter--;}
				}
				if (pos == 0){
					if (pair.second[pos] == '"') is_in = !is_in;
				}
			}
			if (counter != 0 && is_in && pair.second[0] == '"') {std::cerr << "\033[1m" << file_name << ":" << pair.first << ":\033[0m \033[1;31merror:\033[0m A key or a value does not close with a double qoute ('\"')." << std::endl; errs++; has_error = true;} // Check if string key/value doesn't close with a quote
		}
		for (long long line = lines; line >= 0; line--){
			if (raw_file[line] != std::string("") && !close_check){
				if (raw_file[line][raw_file[line].length() - 1] == '}') {break;}
				else if (raw_file[line][raw_file[line].length() - 1] != '}') {std::cerr << "\033[1m" << file_name << ":" << line << ":\033[0m \033[1;31merror:\033[0m A JSON object must be close with a right brace ('}')." << std::endl; errs++; close_check = true; has_error = close_check; break;}
			}
		}
		return !(has_error);
	}
	std::string formatter(const std::string& raw_file_context){
		// Init Area
		std::string result = "";
		bool in = false;
		// Code Area
		for (unsigned long long pos = 0; pos < raw_file_context.length(); pos++){
			if (raw_file_context[pos] == '"' && !in && raw_file_context[pos - 1] != '\\') in = true;
			else if (raw_file_context[pos] == '"' && in && (raw_file_context[pos - 1] != '\\' || raw_file_context[pos - 2] == '\\')) in = false;

			if ((raw_file_context[pos] != ' ' && raw_file_context[pos] != '	') && !in) result += raw_file_context[pos];
			else if ((raw_file_context[pos] == ' ' || raw_file_context[pos] == '	') && in) result += raw_file_context[pos];
			else if ((raw_file_context[pos] != ' ' && raw_file_context[pos] != '	') && in) result += raw_file_context[pos];
		}
		return result;
	}
	bool IsNumber(const char& num){
		if (num == '1') return true;
		if (num == '2') return true;
		if (num == '3') return true;
		if (num == '4') return true;
		if (num == '5') return true;
		if (num == '6') return true;
		if (num == '7') return true;
		if (num == '8') return true;
		if (num == '9') return true;
		if (num == '0') return true;
		if (num == '-') return true;
		return false;
	}
	bool IsNull(const char& n, const char& u, const char& l, const char& l_){
		if (n == 'n' && u == 'u' && l == 'l' && l_ == 'l') return true;
		return false;
	}
	bool IsBool(const char& t, const char& r, const char& u, const char& e, const char& s = 's'){
		if (t == 't' && r == 'r' && u == 'u' && e == 'e') return true;
		if (t == 'f' && r == 'a' && u == 'l' && e == 's' && s == 'e') return true;
		return false;
	}
	void RawStringKey_Print(const std::string& target){
		for (unsigned long long i = 0; i < target.length(); i++){
			switch (target[i]){
				case '\n':
					std::cerr << "\\" << "n";
					continue;
				case '\f':
					std::cerr << "\\" << "f";
					continue;
				case '\r':
					std::cerr << "\\" << "r";
					continue;
				case '\t':
					std::cerr << "\\" << "t";
					continue;
				case '\b':
					std::cerr << "\\" << "b";
					continue;
				case '\\':
					if (target[i + 1] != '"') {std::cerr << "\\\\";}
					if (target[i + 1] == '\\') {i++; continue;}
					else std::cerr << "\\";
					continue;
				default:
					std::cerr << target[i];
			}
		}
	}
	void RawStringKey_Write(const std::string& target, std::ofstream& fout){
		for (unsigned long long i = 0; i < target.length(); i++){
			switch (target[i]){
				case '\n':
					fout << "\\" << "n";
					continue;
				case '\f':
					fout << "\\" << "f";
					continue;
				case '\r':
					fout << "\\" << "r";
					continue;
				case '\t':
					fout << "\\" << "t";
					continue;
				case '\b':
					fout << "\\" << "b";
					continue;
				case '\\':
					if (target[i + 1] != '"') {fout << "\\\\";}
					if (target[i + 1] == '\\'){i++; continue;}
					else fout << "\\";
					continue;
				default:
					fout << target[i];
			}
		}
	}
	bool See(const ___MAPTYPE___& target) {
		if (errs > 0) {
			std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Can not see inside this JSON object because it is has ";
			if (errs == 1) std::cerr << "an error already.";
			else if (errs > 1) std::cerr << errs << " errors already.";
			std::cerr << std::endl;
			errs++;
			return false;
		}
		if (target.size() == 0){std::cerr << "{}"; return true;}
		// Init Area
		static unsigned long long tabs = 0;
		unsigned long long commas = 0;
		// Code Area
		std::cerr << "{" << std::endl; tabs++;
		for (const auto& pair : target){
			IndentTabsAndPrint(tabs);
			std::cerr << "\"";
			RawStringKey_Print(pair.first);
			std::cerr << "\": ";
			ConvertAndPrintVariant(pair.second);
			if (!(commas == target.size() - 1)) {std::cerr << ","; commas++;}
			std::cerr << '\n';
		}
		tabs--; IndentTabsAndPrint(tabs); std::cerr << "}";
		return true;
	}
	void ConvertAndPrintVariant(const ___VALUETYPE___& target){
		if (auto converted = std::get_if<long long>(&target.data)){
			std::cerr << *converted;
		} if (auto converted = std::get_if<double>(&target.data)){
			std::cerr << *converted;
		} if (auto converted = std::get_if<bool>(&target.data)){
			std::cerr << (*converted ? "true" : "false");
		} if (auto converted = std::get_if<std::nullptr_t>(&target.data)){
			std::cerr << "null";
			(void) converted;
		} if (auto converted = std::get_if<std::string>(&target.data)){
			std::cerr << "\"";
			std::string idk = *converted;
			for (unsigned long long pos = 0; pos < idk.length(); pos++){
				switch (idk[pos]){
					case '\n':
						std::cerr << "\\" << "n";
						continue;
					case '\f':
						std::cerr << "\\" << "f";
						continue;
					case '\r':
						std::cerr << "\\" << "r";
						continue;
					case '\t':
						std::cerr << "\\" << "t";
						continue;
					case '\b':
						std::cerr << "\\" << "b";
						continue;
					case '\\':
						if (idk[pos + 1] != '"') std::cerr << "\\\\";
						else std::cerr << "\\";
						continue;
					default:
						std::cerr << idk[pos];
				}
			}
			std::cerr << "\"";
		} if (auto converted = std::get_if<___MAPTYPE___>(&target.data)){
			See(*converted);
		}
	}
	void ConvertAndWriteVariant(const ___VALUETYPE___& target, std::ofstream& fout){
		if (auto converted = std::get_if<long long>(&target.data)){
			fout << *converted;
		} if (auto converted = std::get_if<double>(&target.data)){
			fout << *converted;
		} if (auto converted = std::get_if<bool>(&target.data)){
			fout << (*converted ? "true" : "false");
		} if (auto converted = std::get_if<std::nullptr_t>(&target.data)){
			fout << "null";
			(void) converted;
		} if (auto converted = std::get_if<std::string>(&target.data)){
			fout << "\"";
			std::string idk = *converted;
			for (unsigned long long pos = 0; pos < idk.length(); pos++){
				switch (idk[pos]){
					case '\n':
						fout << "\\" << "n";
						continue;
					case '\f':
						fout << "\\" << "f";
						continue;
					case '\r':
						fout << "\\" << "r";
						continue;
					case '\t':
						fout << "\\" << "t";
						continue;
					case '\b':
						fout << "\\" << "b";
						continue;
					case '\\':
						if (idk[pos + 1] != '"') fout << "\\\\";
						else fout << "\\";
						continue;
					default:
						fout << idk[pos];
				}
			}
			fout << "\"";
		} if (auto converted = std::get_if<___MAPTYPE___>(&target.data)){
			See(*converted);
		}
	}
	___MAPTYPE___ parse(const std::string& context) {
		// Init Area
		std::string key = "";
		___VALUETYPE___ value;
		int counter = 0; // first time i used long long, but long long is so big in your RAM (stack level) so i choose int
		bool in_key = false;
		bool in_value = false;
		std::string datatype = "none";
		std::string who = "key";
		unsigned long long pos_start = 0, pos_end = 0, length = 0;
		___MAPTYPE___ map = this-> map;
		unsigned long long bracket_counter = 0;
		bool in = false; // For minimal loop below
		unsigned long long temp = 0; // Temp value
		std::string idkname = "";
		std::string converted_to_utf8 = "";
		bool has_change = false;
		std::string value_string = "";
		std::string temp1 = "";
		bool in_json_object = false;
		bool firstjson = false;
		std::map<std::string, unsigned long long> noname = {}; // Use to check two keys same as their name
		// Code Area
		for (unsigned long long pos = 0; pos < context.length(); pos++){
			if (!in_key && !in_value && context[pos] == '{' && counter == 0 && !in_json_object && !firstjson) {counter++; in_json_object = true; firstjson = true;} // counter plus to 1 just once time
			else if (context[pos] == '}' && counter == 1 && ((!in_key && !in_value) || (datatype == "int" || datatype == "double"))) {counter--; in_json_object = false; if (datatype != "int" && datatype != "double") who = "none";} // just once time

			if ((!in_key && !in_value && context[pos] != ',' && context[pos] != ':' && context[pos] != '{' && context[pos] != '}' && context[pos] != '"' && !IsNumber(context[pos]) && context[pos] != '.' && context[pos] != 'n' && context[pos] != 'u' && context[pos] != 'l' && context[pos] != 't' && context[pos] != 'r' && context[pos] != 'u' && context[pos] != 'e' && context[pos] != 'f' && context[pos] != 'a' && context[pos] != 'l' && context[pos] != 's' && context[pos] != 'e')) {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Unknown character '" << context[pos] << "'." << std::endl; errs++;}

			// Key handler
			if (who == "key"){
				if (context[pos] == '"' && (context[pos - 1] != '\\' || context[pos - 2] == '\\') && !in_key) {has_change = false; in_key = true; pos_start = pos + 1; datatype = "std::string";}
				else if (context[pos] == '"' && context[pos - 1] != '\\' && in_key) {
					in_key = false; pos_end = pos;
					length = pos_end - pos_start;
					key = context.substr(pos_start, length);
					// Browse for escape codes
					for (unsigned long long i = 0; i < key.length(); i++){
						if (key[i] == '\\'){
							if (key.length() >= 6){
								if (key[i + 1] == 'u'){
									pos_start = i;
									pos_end = i + 6;
									length = pos_end - pos_start;
									idkname = key.substr(pos_start, length);
									temp1 = decode_uXXXX(idkname);
									if (temp1 != "Oh no!"){converted_to_utf8 += temp1;}
									else {
										if (key.length() >= 12){
											pos_end = i + 12;
											length = pos_end - pos_start;
											idkname = key.substr(pos_start, length);
											converted_to_utf8 += decode_uXXXX_uXXXX(idkname);
											i += 11;
											has_change = true;
											continue;
										}
									}
									i += 5;
									has_change = true;
									continue;
								}
							}
							if (key.length() >= 2){
								switch (key[i + 1]){
									case 'n':
										converted_to_utf8 += '\n';
										i++;
										has_change = true;
										continue;
									case 'f':
										converted_to_utf8 += '\f';
										i++;
										has_change = true;
										continue;
									case 'r':
										converted_to_utf8 += '\r';
										i++;
										has_change = true;
										continue;
									case 't':
										converted_to_utf8 += '\t';
										i++;
										has_change = true;
										continue;
									case 'b':
										converted_to_utf8 += '\b';
										i++;
										has_change = true;
										continue;
									case '\\':
										converted_to_utf8 += '\\';
										has_change = true;
										continue;
								}
							}
						}
						converted_to_utf8 += key[i];
					}
					if (has_change) {key = converted_to_utf8;}
					converted_to_utf8 = "";
					//std::cerr << "key = " << key << std::endl; // Debug
					noname[key]++;
					if (noname[key] > 1){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;33mwarning:\033[0m Duplicate key found: '" << key << "'. The last value maybe not used." << std::endl;}
					who = "value";
					if (context[pos + 1] != ':') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a colon(':') after key '" << key << "'." << std::endl; errs++;}
					if (context[pos + 1] == ':' && context[pos + 2] == ':') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Unexcepted character ':' before key '" << key << "'." << std::endl; errs++;}
				}
			}
			// Value handler
			else if (who == "value"){
				// String datatype
				if (context[pos] == '"' && context[pos - 1] != '\\' && !in_value) {has_change = false; in_value = true; pos_start = pos + 1; datatype = "std::string";}
				else if (context[pos] == '"' && (context[pos - 1] != '\\' || context[pos - 2] == '\\') && in_value && datatype == "std::string"){
					in_value = false; pos_end = pos;
					length = pos_end - pos_start;
					value = Value(context.substr(pos_start, length));
					// Browse for escape codes
					value_string = ConvertVariantToString(value);
					
					for (unsigned long long i = 0; i < value_string.length(); i++){
						if (value_string[i] == '\\'){
							if (value_string.length() >= 6){
								if (value_string[i + 1] == 'u'){
									pos_start = i;
									pos_end = i + 6;
									length = pos_end - pos_start;
									idkname = value_string.substr(pos_start, length);
									temp1 = decode_uXXXX(idkname);
									if (temp1 != "Oh no!"){converted_to_utf8 += temp1;}
									else {
										if (value_string.length() >= 12){
											pos_end = i + 12;
											length = pos_end - pos_start;
											idkname = value_string.substr(pos_start, length);
											converted_to_utf8 += decode_uXXXX_uXXXX(idkname);
											i += 11;
											has_change = true;
											continue;
										}
									}
									i += 5;
									has_change = true;
									continue;
								}
							}
							if (value_string.length() >= 2){
								switch (value_string[i + 1]){
									case 'n':
										converted_to_utf8 += '\n';
										i++;
										has_change = true;
										continue;
									case 'f':
										converted_to_utf8 += '\f';
										i++;
										has_change = true;
										continue;
									case 'r':
										converted_to_utf8 += '\r';
										i++;
										has_change = true;
										continue;
									case 't':
										converted_to_utf8 += '\t';
										i++;
										has_change = true;
										continue;
									case 'b':
										converted_to_utf8 += '\b';
										i++;
										has_change = true;
										continue;
									case '\\':
										converted_to_utf8 += '\\';
										i++;
										has_change = true;
										continue;
								}
							}
						}
						converted_to_utf8 += value_string[i];
					}
					if (has_change){value = Value(converted_to_utf8);}
					converted_to_utf8 = "";
					// std::cerr << "value = "; ConvertAndPrintVariant(value); std::cerr << std::endl; // Debug
					who = "key";
					map[key] = value;
					datatype = "none";
					if (context[pos + 1] != ',' && context[pos + 1] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value '"; ConvertAndPrintVariant(value); std::cerr << "' has key is '" << key << "'." << std::endl; errs++;}
					if (context[pos + 1] == ',' && context[pos + 2] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is '" << key << "' because it in a final pair in this object." << std::endl; errs++;}
				}
				// JSON datatype
				if (context[pos] == '{' && !in_value){
					pos_start = pos;
					for (unsigned long long i = pos_start; i < context.length(); i++){
						temp = i;
						if (context[i] == '"' && context[i - 1] != '\\') in = !in;

						if (context[i] == '{' && !in) {bracket_counter++;}
						if (context[i] == '}' && !in) {bracket_counter--;}

						if (bracket_counter == 0){
							pos_end = i + 1;
							break;
						}
					}
					length = pos_end - pos_start;
					value = Value(parse(context.substr(pos_start, length)));
					map[key] = value;
					who = "key";
					pos = pos_end - 1;
					if (context[temp + 1] != ',' && context[temp + 1] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value '"; ConvertAndPrintVariant(value); std::cerr << "' has key is \"" << key << "\"." << std::endl; errs++;}
					if (context[temp + 1] == ',' && context[temp + 2] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is \"" << key << "\" because it in a final pair in this object." << std::endl; errs++;}
				}
				// Double
				if (context[pos] == '.' && !in_value && datatype != "int"){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A decimal point ('.') in a pair has key is '" << key << "' found in first of a decimal number." << std::endl; errs++;}
				if (context[pos] == '.' && datatype == "int" && in_value){datatype = "double"; if (context[pos + 1] == '.'){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A decimal point ('.') in a pair has key is '" << key << "' must be appear just once time or between 2 numbers." << std::endl; errs++;}}
				if (!IsNumber(context[pos]) && datatype == "double" && context[pos] != '.' && in_value){
					in_value = false; pos_end = pos;
					length = pos_end - pos_start;
					try {
						value = Value(stod(context.substr(pos_start, length)));
						// std::cerr << "value = "; ConvertAndPrintVariant(value); std::cerr << std::endl; // Debug
						who = "key";
						map[key] = value;
						datatype = "none";
						if (context[pos] != ',' && context[pos] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value '"; ConvertAndPrintVariant(value); std::cerr << "' has key is \"" << key << "\"." << std::endl; errs++;}
						if (context[pos] == ',' && context[pos + 1] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is \"" << key << "\" because it in a final pair in this object." << std::endl; errs++;}
					} catch (const std::out_of_range& e){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A decimal number '" << context.substr(pos_start, length) << "' that very large, double can't restore and handle in it." << std::endl; errs++;}
				}
				// Number datatype (I am using long long)
				if (IsNumber(context[pos]) && !in_value){pos_start = pos; datatype = "int"; in_value = true; if (context[pos + 1] == '-') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A negative sign ('-') in a pair has key is '" << key << "' must be appear just once time or in front of a number." << std::endl; errs++;}}
				if (!IsNumber(context[pos]) && datatype == "int" && in_value){
					in_value = false; pos_end = pos;
					length = pos_end - pos_start;
					try {
						value = Value(stoll(context.substr(pos_start, length)));
						// std::cerr << "value = "; ConvertAndPrintVariant(value); std::cerr << std::endl; // Debug
						who = "key";
						map[key] = value;
						datatype = "none";
						if (context[pos] != ',' && context[pos] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value '"; ConvertAndPrintVariant(value); std::cerr << "' has key is \"" << key << "\"." << std::endl; errs++;}
						if (context[pos] == ',' && context[pos + 1] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is \"" << key << "\" because it in a final pair in this object." << std::endl; errs++;}
					} catch (const std::out_of_range& e){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A number '" << context.substr(pos_start, length) << "' that very large, long long can't restore and handle in it." << std::endl; errs++;}
				}
				// Bool datatype (true/false)
				if (!in_key && !in_value && context[pos] == 't'){
					if (IsBool(context[pos], context[pos + 1], context[pos + 2], context[pos + 3])){
						value = Value(true);
						map[key] = value;
						datatype = "bool (true)";
						who = "key";
						pos += 3; // Skip out the value
						if (context[pos + 1] != ',' && context[pos + 1] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value 'true', key is \"" << key << "\"." << std::endl; errs++;}
						if (context[pos + 1] == ',' && context[pos + 2] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is \"" << key << "\" because it in a final pair in this object." << std::endl; errs++;}
					} else {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Unknown bool datatype (true) in a pair has key is '" << key << "'. Did you mean 'true' ?" << std::endl; errs++;}
				} if (!in_key && !in_value && context[pos] == 'f'){
					if (IsBool(context[pos], context[pos + 1], context[pos + 2], context[pos + 3], context[pos + 4])){
						value = Value(false);
						map[key] = value;
						datatype = "bool (false)";
						who = "key";
						pos += 4; // Skip out the value
						if (context[pos + 1] != ',' && context[pos + 1] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value 'false', key is \"" << key << "\"." << std::endl; errs++;}
						if (context[pos + 1] == ',' && context[pos + 2] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is \"" << key << "\" because it in a final pair in this object." << std::endl; errs++;}
					} else {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Unknown bool datatype (false) in a pair has key is '" << key << "'. Did you mean 'false' ?" << std::endl; errs++;}
				}

				// Null datatype
				if (!in_key && !in_value && context[pos] == 'n'){
					if (IsNull(context[pos], context[pos + 1], context[pos + 2], context[pos + 3])){
						value = Value(nullptr);
						map[key] = value;
						datatype = "null";
						who = "key";
						pos += 3; // Skip out the value
						if (context[pos + 1] != ',' && context[pos + 1] != '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Missing a comma(',') after value 'null', key is \"" << key << "\"." << std::endl; errs++;}
						if (context[pos + 1] == ',' && context[pos + 2] == '}') {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A comma(',') must not be after value '"; ConvertAndPrintVariant(value); std::cerr << "', key is \"" << key << "\" because it in a final pair in this object." << std::endl; errs++;}
					} else {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Unknown null datatype in a pair has key is '" << key << "'. Did you mean 'null' ?" << std::endl; errs++;}
				}
			}
			if (context[pos] == '{' && !in_json_object && who == "none" && firstjson){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m Outside this JSON object has another JSON object." << std::endl; errs++;}
		}
		if (counter == 1){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m An open brace that never closes." << context << std::endl; errs++;}
		else if (counter > 1){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m " << counter << " open braces that never close." << std::endl; errs++;}

		else if (counter == -1){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m A right brace that never opens." << std::endl; errs++;}
		else if (counter < -1){std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m " << -counter << " right braces that never open." << std::endl; errs++;}
		return map;
	}
public:
	___MAPTYPE___ map = {};
	explicit JSONObject(const std::string& file) : file_name(file){}
	explicit JSONObject(const std::string& file, const ___MAPTYPE___& amap) : file_name(file), map(amap){}
	explicit JSONObject(const ___MAPTYPE___& amap) : map(amap){}
	explicit JSONObject() {}
	JSONObject& operator=(const JSONObject& other){
		this->map = other.map;
		return *this;
	}
	JSONObject& operator=(const ___MAPTYPE___& other){
		this-> map = other;
		return *this;
	}
	bool operator==(const JSONObject& other) const {
		return this->map == other.map;
	}
	bool operator!=(const JSONObject& other) const {
		return !(this->map == other.map);
	}
	friend std::ostream& operator<<(std::ostream& os, const JSONObject& obj);
	friend JSONObject& operator>>(const ___MAPTYPE___& giver, JSONObject& receiver);
	void ResetErr(){errs = 0;}
	bool open(const bool& RemoveUnneededLines = false){
		if (file_name == "") throw std::runtime_error("The file is not assigned for its name.");
		// Init Variables Area
		std::ifstream file(file_name);
		std::string line;
		long long linenum = 0;
		bool state = true;
		// Code Area
		if (!file.is_open()){
			std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m File not found." << std::endl; errs++;
			errs++;
			return false;
		}
		while (getline(file, line)){
			raw_file_context.append(line);
			if (RemoveUnneededLines){
				if (line != "" || ::HasNonSpaceChar(line)) {linenum++; raw_file[linenum] = line;}
				continue;
			}
			else {linenum++; raw_file[linenum] = line;}
		}
		lines = linenum;
		file.close();
		state = CheckBeforeFormatRawFileContext();
		if (state) context = formatter(raw_file_context);
		if (context == "") {std::cerr << "\033[1m" << file_name << ":\033[0m \033[1;31merror:\033[0m This file is empty or does not contain a valid JSON." << std::endl; errs++; throw std::runtime_error("The file is empty.");}
		return state;
	}
	void SeeInsideFileAsRaw(){
		// You can see inside a file if you called open() function
		// Init Area
		std::string spaces = std::to_string(lines);
		// Code Area
		std::cerr << "\033[1m" << file_name << "\033[0m: raw view" << std::endl;
		for (const auto& pair : raw_file){
			std::string now_number = std::to_string(pair.first);
			std::cerr <<  "	";
			IndentSpacesAndPrint(spaces.length() - now_number.length());
			std::cerr <<  pair.first << " | " << pair.second << std::endl;
 		}
	}
	___MAPTYPE___ dump(){
		map = parse(context);
		return map;
	}
	bool write(const ___MAPTYPE___& target){
		// Use when variable 'map' is not have data
		// Init Area
		std::ofstream fout(file_name);
		if (!fout.is_open()){
			std::cerr << "\033[1;31mJSONError:\033[0m File '" << file_name << "' not found! Try to check if it exists and try again.";
			errs++;
			return false;
		}
		if (target.size() == 0) {std::cerr << "{}"; return true;}
		static long long tabs = 0;
		unsigned long long commas = 0;
		// Code Area
		fout << "{" << std::endl; tabs++;
		for (const auto& pair : target){
			IndentTabsAndWrite(tabs, fout);
			fout << "\"";
			RawStringKey_Write(pair.first, fout);
			std::cerr << "\": ";
			ConvertAndWriteVariant(pair.second, fout);
			if (!(commas == target.size() - 1)) {fout << ","; commas++;}
			std::cerr << '\n';
		}
		tabs--; IndentTabsAndWrite(tabs, fout); fout << "}";
		fout.close();
		return true;
	}
	bool SeeInsideThisObject() {
		if (See(map)){std::cerr << std::endl; return true;}
		return false;
	}
	~JSONObject(){
		if (errs == 1) {std::cerr << std::endl << "\033[1;31mJSONErrorReporter:\033[0m " << errs << " error generated." << std::endl;}
		if (errs > 1) {std::cerr << std::endl << "\033[1;31mJSONErrorReporter:\033[0m " << errs << " errors generated." << std::endl;}
	}
};
}
std::ostream& operator<<(std::ostream& os, ntk::JSONObject& obj);
ntk::JSONObject& operator>>(const ___MAPTYPE___& giver, ntk::JSONObject& receiver);
void SetUnicodeForTheOutput();