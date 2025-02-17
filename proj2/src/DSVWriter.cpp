#include "DSVWriter.h"
#include <sstream>

// constructor: initializes the sink, delimiter, and quoteAll flag
CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : Sink(std::move(sink)), Delimiter(delimiter), QuoteAll(quoteall) {}

// destructor: default as no manual memory management is required
CDSVWriter::~CDSVWriter() = default;

// formats a single cell based on DSV rules (quotes, escape handling, etc.)
std::string FormatCell(const std::string &cell, char delimiter, bool quoteAll) {
    std::string formatted = cell;
    bool needsQuotes = quoteAll || cell.find(delimiter) != std::string::npos ||
                       cell.find('"') != std::string::npos || cell.find('\n') != std::string::npos;

    if (needsQuotes) {
        std::ostringstream quotedCell;
        quotedCell << '"';
        for (char ch : formatted) {
            if (ch == '"') {
                quotedCell << "\"\""; // escape double quotes
            } else {
                quotedCell << ch;
            }
        }
        quotedCell << '"';
        formatted = quotedCell.str();
    }

    return formatted;
}

// writes a row of data to the sink, applying proper formatting
bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    if (!Sink) {
        return false; // if no valid sink, return failure
    }

    std::ostringstream rowStream;
    for (size_t i = 0; i < row.size(); i++) {
        if (i > 0) { rowStream << Delimiter; } // separate values with delimiter
        rowStream << FormatCell(row[i], Delimiter, QuoteAll); // format cell properly
    }

    std::string outputRow = rowStream.str();
    std::vector<char> outputVector(outputRow.begin(), outputRow.end());
    outputVector.push_back('\n'); // Add newline manually

    return Sink->Write(outputVector); // Now passing a vector<char>

}