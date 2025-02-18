#include "XMLWriter.h"
#include <sstream>

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Function to properly escape XML special characters
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

// Writes only the main XML tag, ignoring children
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;

    std::ostringstream output;

    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;

        // Write attributes with correct escaping
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"" << EscapeXML(attr.second) << "\"";
        }

        // ✅ Self-close the tag since we are ignoring children
        output << "/>";
    } 
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        // ❌ Skip writing end elements since we're self-closing
        return true;
    }
    else if (entity.DType == SXMLEntity::EType::CharData) {
        // ❌ Ignore any child text content
        return true;
    }

    std::string outputStr = output.str();
    std::vector<char> outputVec(outputStr.begin(), outputStr.end());

    return Sink->Write(outputVec) > 0;
}

// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}
