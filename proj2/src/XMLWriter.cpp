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

    std::ostringstream output;
    std::ostringstream attrstr;

    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;

        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"";
            for (char c : attr.second) {
                if (c == '&') {
                    output << "&amp;";
                    attrstr << "&amp;";
                }
                else if (c == '<') {
                    output << "&lt;";
                    attrstr << "&lt;";
                }
                else if (c == '>') {
                    output << "&gt;";
                    attrstr << "&gt;";
                }
                else if (c == '\"') {
                    output << "&quot;";
                    attrstr << "&quot;";
                }
                else if (c == '\'') {
                    output << "&apos;";
                    attrstr << "&apos;";
                }
                else output << c; 
            }
            output << "\"";
        }

        // ✅ Fix: Remove space before self-closing `/ >`
        if (entity.DAttributes.empty() && entity.DAttributes.empty()) {
            output << "/>";  // ✅ No space before `/>`
        } else {
            Stack.push_back(entity.DNameData);
            output << ">";
            output << attrstr.str();
        }
    } 
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        if (!Stack.empty() && Stack.back() == entity.DNameData) {
        output << "</" << entity.DNameData << ">";
        Stack.pop_back();
        }
    }

    std::string outputStr = output.str();
    std::vector<char> outputVec(outputStr.begin(), outputStr.end());

    return Sink->Write(outputVec) > 0;
}

// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}
