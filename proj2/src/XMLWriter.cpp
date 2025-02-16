#include "XMLWriter.h"
#include <sstream>

std::string EscapeXML(const std::string &input) {
    std::string escaped = input;
    size_t pos = 0;

    // Escape '&' as '&amp;'
    while ((pos = escaped.find("&", pos)) != std::string::npos) {
        escaped.replace(pos, 1, "&amp;");
        pos += 5; // Skip past the escape sequence
    }

    // Escape '<' as '&lt;'
    pos = 0;
    while ((pos = escaped.find("<", pos)) != std::string::npos) {
        escaped.replace(pos, 1, "&lt;");
        pos += 4;
    }

    // Escape '>' as '&gt;'
    pos = 0;
    while ((pos = escaped.find(">", pos)) != std::string::npos) {
        escaped.replace(pos, 1, "&gt;");
        pos += 4;
    }

    // Escape '"' as '&quot;'
    pos = 0;
    while ((pos = escaped.find("\"", pos)) != std::string::npos) {
        escaped.replace(pos, 1, "&quot;");
        pos += 6;
    }

    // Escape "'" as '&apos;'
    pos = 0;
    while ((pos = escaped.find("'", pos)) != std::string::npos) {
        escaped.replace(pos, 1, "&apos;");
        pos += 6;
    }

    return escaped;
}

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Writes an XML entity to the sink
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;

    std::ostringstream output;
    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"" << EscapeXML(attr.second) << "\"";
        }

        // Check if it's a self-closing tag (no children or attributes)
        if (entity.DAttributes.empty()) {
            output << " />"; // Self-closing tag
        } else {
            output << ">";
        }
    } else if (entity.DType == SXMLEntity::EType::EndElement) {
        output << "</" << entity.DNameData << ">";
    }

    std::string outputStr = output.str();
    std::vector<char> outputVec(outputStr.begin(), outputStr.end());
    return Sink->Write(outputVec);
}

// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}
