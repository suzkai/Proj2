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
    if (entity.DType == SXMLEntity::EType::StartElement) {
        output << "<" << entity.DNameData;
        for (const auto &attr : entity.DAttributes) {
            output << " " << attr.first << "=\"" << attr.second << "\"";
        }

        // Check if it's a self-closing tag (no children or attributes)
        if (entity.DType == SXMLEntity::EType::StartElement) {
            output << "<" << entity.DNameData;
            for (const auto &attr : entity.DAttributes) {
                output << " " << attr.first << "=\"" << attr.second << "\"";
            }
        
            // Only self-close if there's NO text or children
            if (entity.DAttributes.empty()) {
                output << " />"; // Self-closing tag
            } else {
                output << ">"; // Properly open tag
            }
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
