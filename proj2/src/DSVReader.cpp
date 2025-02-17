#include "DSVReader.h"
#include "DataSource.h"
#include <sstream>
#include <vector>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> Source; // data source for reading input
    char Delimiter; // separate values in the dsv file
    std::string CurrentLine; 
    bool EndOfFile = false; 

    // constructor for SImplementation, initializes source and delimiter
    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : Source(std::move(src)), Delimiter(delimiter) {}

    // reads the next line from the data source and updates the current line
    // returns false if end of file is reached
    bool ReadNextLine() {
        CurrentLine.clear(); // reset the line buffer
        char ch;

        while (Source->Get(ch)) { // read character by character
            if (ch == '\n') { // stop at newline
                break;
            }
            CurrentLine += ch;
        }

        // if nothing was read and end of file is reached, mark it
        if (CurrentLine.empty() && Source->End()) { 
            EndOfFile = true;
            return false;
        }

        return true; // return true if data was read
    }
};

// store everything inside SImplementation and use a std::unique_ptr to deal w/ cleaup
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(std::make_unique<SImplementation>(std::move(src), delimiter)) {}

CDSVReader::~CDSVReader() = default;

// returns true if the end of the file has been reached
bool CDSVReader::End() const {
    return DImplementation->EndOfFile; 
}

// reads a row from the dsv file and fills vector with values
bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    row.clear(); // clear the row for new values
    
    // return false if no more data to read on next line
    if (!DImplementation->ReadNextLine()) {
        // DImplementation->EndOfFile = true;  removed this
        return false;
    }
    

    std::stringstream lineStream(DImplementation->CurrentLine); // use stringstream to parse line
    std::string cell; 
    char delimiter = DImplementation->Delimiter;
    bool inQuotes = false; 

    // loop through each character in the line
    while (lineStream) {
        char ch;
        if (!lineStream.get(ch)) { 
            break; 
        }

        if (ch == '"') { // for handling of quoted fields
            if (!inQuotes) {
                inQuotes = true; // opening quote found
            } else {
                if (lineStream.peek() == '"') { // check if next char is another quote
                    lineStream.get(ch); 
                    cell += '"'; 
                } else {
                    inQuotes = false; // closing quote detected
                }
            }
        } else if (ch == delimiter && !inQuotes) { // if delimiter is found outside quotes
            row.push_back(cell); // add completed cell to row
            cell.clear(); // reset cell for next value
        } else {
            cell += ch; 
        }
    }
    
    row.push_back(cell); 
    return true; // return true if row was successfully read
}
