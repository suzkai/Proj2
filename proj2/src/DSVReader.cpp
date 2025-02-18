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
        CurrentLine.clear(); // Reset the line buffer
        char ch;
        bool hasData = false;
    
        while (Source->Get(ch)) { // Read character by character
            hasData = true; // At least one character read
            if (ch == '\n') { // Stop at newline
                break;
            }
            CurrentLine += ch;
        }
    
        // If no data was read and the source is at the end, mark EOF
        if (!hasData && Source->End()) { 
            EndOfFile = true;
            return false;
        }
    
        // If we hit EOF after reading a line, mark EOF
        if (Source->End()) { 
            EndOfFile = true;
        }
    
        return true;
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
    row.clear(); // Clear the row for new values

    if (!DImplementation->ReadNextLine()) {
        return false;
    }

    std::string linestream = DImplementation->CurrentLine;
    char delimiter = DImplementation->Delimiter;
    bool inQuotes = false;
    std::string cell;
    size_t i = 0;

    while (i < linestream.size()) {
        char ch = linestream[i];

        if(ch == '"'){ // Handle quoted fields
            if(!inQuotes && linestream[i - 1] == delimiter || i == 0){
                //cell += "in";
                inQuotes = true; // Opening quote
            }
            else{ // Handle escaped quotes
                // if in index and next char is double quote
                if (i + 1 < linestream.size() && linestream[i + 1] == '"') {
                    cell += '"';
                    i++; // skip 2nd quote
                }
                else{
                    //cell += "out";
                    inQuotes = false; // Closing quote
                }
            }
        }
        else if(ch == delimiter && !inQuotes) { // If delimiter is found outside quotes
            row.push_back(cell);
            cell.clear();
        }
        else{
            cell += ch;
        }

        i++; //increment
    }

    row.push_back(cell);

    return true;

    // std::stringstream lineStream(DImplementation->CurrentLine);
    // std::string cell;
    // char delimiter = DImplementation->Delimiter;
    // bool inQuotes = false;

    // while (lineStream) {
    //     char ch;
    //     if (!lineStream.get(ch)) { 
    //         break; 
    //     }

    //     if (ch == '"') { // Handle quoted fields
    //         if (!inQuotes) {
    //             inQuotes = true; // Opening quote
    //         }
    //         else {
    //             if (lineStream.peek() == '"') { // Handle escaped quotes
    //                 cell += "\"";
    //                 lineStream.get(ch);
    //             }
    //             else {
    //                 inQuotes = false; // Closing quote
    //             }
    //         }
    //     } else if (ch == delimiter && !inQuotes) { // If delimiter is found outside quotes
    //         row.push_back(cell);
    //         cell.clear();
    //     } else {
    //         cell += ch;
    //     }
    // }

    // row.push_back(cell);


    // return true;
}