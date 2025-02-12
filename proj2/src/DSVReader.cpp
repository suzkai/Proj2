#include "DSVReader.h"

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : Source(src), Delimiter(delimiter) {}

CDSVReader::~CDSVReader() {}

bool CDSVReader::End() const {
    return true; // Placeholder
}

bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    return false; // Placeholder
}
