/**
 * Utility for converting utc timestamps to readable strings.
 */

#include <cctz/civil_time.h>
#include <cctz/time_zone.h>
#include <iostream>
#include <sstream>

using namespace cctz;
using namespace std;


/**
 * Attempt to interpret a string as a UTC timestamp and return
 * it in a human-readable form or return the string unchanged.
 */
string try_convert_utc(const string &str, const time_zone &tz)
{
    try {
        size_t dot = str.find('.');
        string int_part, frac_part;
        if (dot == string::npos) {
            int_part = str;
        }
        else {
            int_part = str.substr(0, dot);
            frac_part = str.substr(dot + 1);
        }

        int l = int_part.size();
        if (l < 10 || l % 3 != 1) {
            return str;
        }

        string sec_str = int_part.substr(0, 10);
        size_t ix;
        long utc = stol(sec_str, &ix);
        if (ix != sec_str.size()) {
            return str;
        }

        string frac_sec_str = int_part.substr(10) + frac_part;
        while (frac_sec_str.size() % 3 != 0) {
            frac_sec_str += '0';
        }

        chrono::time_point<chrono::system_clock, chrono::seconds>
            tp((chrono::seconds(utc)));
        if (frac_sec_str.empty()) {
            return format("%Y%m%d %H:%M:%S %Z", tp, tz);
        }
        else {
            return format("%Y%m%d %H:%M:%S." + frac_sec_str + " %Z", tp, tz);
        }
    }
    catch (exception &e) {
        return str;
    }
}


/**
 * Attempt to identify and convert UTC timestamps in the given
 * input stream.
 */
void process_stream(istream &fp, const time_zone &tz)
{
    bool word_break = true;
    bool found_period = false;
    char c;
    string number;
    while (fp.get(c)) {
        if (number.empty()) {
            if (word_break) {
                if (isdigit(c)) {
                    // Starting to read a number
                    number += c;
                    found_period = false;
                }
                else {
                    cout << c;
                }
            }
            else {
                cout << c;
            }
            word_break = !isalpha(c);
        }
        else {
            // We are reading a number
            if (isdigit(c)) {
                number += c;
            }
            else if (c == '.' && !found_period) {
                number += c;
                found_period = true;
            }
            else if (isalpha(c)) {
                // This was not a number after all
                cout << number << c;
                found_period = false;
                number.clear();
            }
            else {
                // Finished reading a number-like string
                cout << try_convert_utc(number, tz) << c;
                word_break = true;
                found_period = false;
                number.clear();
            }
        }
    }

    if (!number.empty()) {
        cout << try_convert_utc(number, tz);
    }
}


/**
 * Entry point.
 */
int main(int argc, char **argv)
{
    string tz_str = "America/New_York";
    bool process_input = true;
    bool stop_processing_arguments = false;
    stringstream text;
    for (int i = 1; i < argc; i++) {
        const string arg(argv[i]);
        if (arg == "--") {
            process_input = false;
            stop_processing_arguments = true;
            continue;
        }
        if (!stop_processing_arguments) {
            if (arg == "--tz" && i + 1 < argc) {
                tz_str = argv[++i];
                continue;
            }
            if (arg == "--help" || arg == "-h") {
                cout << "Convert utc-like numbers in arguments or input to human-readable timestamps\n"
                        "\n"
                        "Usage: utc [--tz timezone] [--] [text...]\n"
                        "\n"
                        "  --tz timezone    Timezone to use for presenting converted timestamps.\n"
                        "                   Defaults to " + tz_str + "\n"
                        "\n"
                        "Arguments given on the command line will be treated as text to be\n"
                        "processed. Otherwise, text will be read from the standard input.\n"
                        "Numbers that appears to be a potential UTC timestamp with second,\n"
                        "millisecond, microsecond, or nanosecond precision will be converted\n"
                        "to a human-readable format using the specified timezone.\n";
                return 2;
            }
        }
        if (text.tellp() > 0) {
            text << ' ';
        }
        text << arg;
        process_input = false;
    }

    time_zone tz;
    if (!load_time_zone(tz_str, &tz)) {
        cerr << "Error querying timezone '" + tz_str + "'\n";
        return 1;
    }

    if (process_input) {
        process_stream(cin, tz);
    }
    else {
        process_stream(text, tz);
        cout << "\n";
    }
    return 0;
}
