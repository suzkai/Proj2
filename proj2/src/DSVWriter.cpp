#include "DSVWriter.h"
#include "StringDataSink.h"

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    char Delimiter;
    bool QuoteAll;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
        : Sink(sink), Delimiter(delimiter), QuoteAll(quoteall) {}
};

//================================================================

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>(sink, delimiter, quoteall)) {}

CDSVWriter::~CDSVWriter() {}

bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {

    // > An empty line is a valid row where there are no values.
    std::vector<char> buf;
    if(row.size() == 0){
        return DImplementation->Delimiter;
    }

    std::string ret;

    for(size_t i = 0; i < row.size(); i++){
        if(i != 0 && row[i].size() > 0){
            ret += DImplementation->Delimiter;
        }
        
    
        // if QuoteAll or delimiter, quotes, or newline in element
        if(DImplementation->QuoteAll || 
            row[i].find(DImplementation->Delimiter) != std::string::npos ||
            row[i].find('"') != std::string::npos || 
            row[i].find('\n') != std::string::npos)   
        {
            ret += '"';
            const std::string &element = row[i];
            for(size_t j = 0; j < element.size(); j++){

                // Values that have either the delimiter,
                // double quote character ‘"’, or newline must be
                // quoted with double quotes.
                const char c = element[j];
                if(c == '"'){
                    ret += "\"\"";
                }
                else{
                    ret += c;
                }
            }
            ret += "\"";
        }

        else{
            ret += row[i];
        }
    } // for loop

    ret += "\n";

    buf = {ret.begin(), ret.end()};
    return DImplementation->Sink->Write(buf) > 0;
}
