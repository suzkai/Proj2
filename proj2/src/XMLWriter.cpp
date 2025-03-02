#include "XMLWriter.h"
#include <sstream>

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Writes an XML entity to the sink
#include "XMLWriter.h"
#include <sstream>

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Writes an XML entity to the sink
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;

    if (entity.DNameData == "node") {
        return true;
    }

    std::vector<char> output;

    if (entity.DType == SXMLEntity::EType::StartElement) {
        // Beginning tag
        output.push_back('<');
        for (char c : entity.DNameData) {
            output.push_back(c);
        }

        // Attributes
        for (const auto &attr : entity.DAttributes) {
            output.push_back(' ');
            for (char c : attr.first) {
                output.push_back(c);
            }
            output.push_back('=');
            output.push_back('\"');
            for (char c : attr.second) {
                if (c == '&') {
                    for (char d : "&amp;"){
                        output.push_back(d);
                    }
                }
                else if (c == '<') {
                    for (char d : "&lt;"){
                        output.push_back(d);
                    }
                }
                else if (c == '>') {
                    for (char d : "&gt;"){
                        output.push_back(d);
                    }
                }
                else if (c == '\"') {
                    for (char d : "&quot;"){
                        output.push_back(d);
                    }
                }
                else if (c == '\'') {
                    for (char d : "&apos;"){
                        output.push_back(d);
                    } 
                }
                else {
                    output.push_back(c);
                }
            }
            output.push_back('\"');
        }

        // Close beginning tag
        if(entity.DAttributes.empty()){
            output.push_back('/');
            output.push_back('>');
        } 
        else{
            output.push_back('>');
        }
    } 
    // End tag
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        output.push_back('<');
        output.push_back('/');
        for (char c : entity.DNameData) {
            output.push_back(c);
        }
        output.push_back('>');
    }

    return Sink->Write(output) > 0;
}
bool CXMLWriter::Flush() {
    return true;
}