#include "XMLWriter.h"
#include <sstream>
#include <iostream>

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : Sink(std::move(sink)) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

// Helper function to escape special characters in XML (attributes or text)
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

// Writes an XML entity to the sink
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!Sink) return false;

    std::ostringstream output;

    // Process start element
    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;

        // Process attributes
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"" << EscapeXML(attr.second) << "\""; // Escape attribute values
        }

        // If no attributes or children, self-close the tag
        if (entity.DAttributes.empty()) {
            output << " />"; // Self-closing tag
        } else {
            output << ">";  // Open the element
        }
    }
    // Process end element
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        output << "</" << entity.DNameData << ">";  // Close the element
    }

    // Store the generated XML in the buffer
    buffer_ += output.str();

    return true;
}

// Flushes the accumulated data to the sink
bool CXMLWriter::Flush() {
    if (!Sink) return false;

    // Write all the accumulated content to the sink
    std::vector<char> outputVec(buffer_.begin(), buffer_.end());
    bool result = Sink->Write(outputVec);

    // Clear the buffer after writing
    buffer_.clear();

    return result;
}
