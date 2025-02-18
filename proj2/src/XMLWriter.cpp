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
        // Only write <osm> if the entity is "osm"
        if (entity.DNameData == "osm") {
            output << "<" << entity.DNameData;

            // Write the attributes for the <osm> tag
            for (const auto &attr : entity.DAttributes) {
                output << " " << attr.first << "=\"" << attr.second << "\"";
            }
            output << ">";
        }
    } 
    else if (entity.DType == SXMLEntity::EType::EndElement) {
        // Only write the closing tag for <osm>
        if (entity.DNameData == "osm") {
            output << "</" << entity.DNameData << ">";
        }
    }

    std::string outputStr = output.str();

    // If output contains something (in case of start element or end element)
    if (!outputStr.empty()) {
        std::vector<char> outputVec(outputStr.begin(), outputStr.end());
        return Sink->Write(outputVec) > 0;
    }

    return false;
}


// Flushes remaining data (not needed for simple cases)
bool CXMLWriter::Flush() {
    return true;
}