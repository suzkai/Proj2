#include "XMLReader.h"

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) : Source(src) {}

CXMLReader::~CXMLReader() {}

bool CXMLReader::End() const {
    return true; // Placeholder
}

bool CXMLReader::ReadEntity(SXMLEntity& entity, bool skipcdata) {
    return false; // Placeholder
}
