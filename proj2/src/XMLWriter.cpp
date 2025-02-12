#include "XMLWriter.h"

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) : Sink(sink) {}

CXMLWriter::~CXMLWriter() {}

bool CXMLWriter::Flush() {
    return true; // Placeholder
}

bool CXMLWriter::WriteEntity(const SXMLEntity& entity) {
    return false; // Placeholder
}
