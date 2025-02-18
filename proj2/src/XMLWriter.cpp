#include "XMLWriter.h"
#include <sstream>

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Function to escape XML special characters
std::string EscapeXML(const std::string &str) {
    std::ostringstream escaped;
    for (char c : str) {
        switch (c) {
            case '&': escaped << "&amp;"; break;
            case '"': escaped << "&quot;"; break;
            case '\'': escaped << "&apos;"; break;
            case '<': escaped << "&lt;"; break;
            case '>': escaped << "&gt;"; break;
            default: escaped << c;
        }
    }
    return escaped.str();
}

// Writes an XML entity to the sink
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;

    std::ostringstream output;

    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;

        // Write attributes with correct escaping
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"" << EscapeXML(attr.second) << "\"";
        }

        // ✅ Fix: Ensure self-closing elements are correctly formatted
        if (entity.DAttributes.empty() && entity.DNameData != "") {  
            output << "/>";  // Proper self-closing format
        } else {
            output << ">";
        }
    } 
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        output << "</" << entity.DNameData << ">";
    } 
    else if (entity.DType == SXMLEntity::EType::CharData) {
        output << EscapeXML(entity.DNameData);
    }

    std::string outputStr = output.str();
    std::vector<char> outputVec(outputStr.begin(), outputStr.end());

    return Sink->Write(outputVec) > 0;
}

// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}
