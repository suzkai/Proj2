#include "DSVWriter.h"

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : Sink(sink), Delimiter(delimiter), QuoteAll(quoteall) {}

CDSVWriter::~CDSVWriter() {}

bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {
    return false; // Placeholder
}
