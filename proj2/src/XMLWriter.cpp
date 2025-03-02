#include "XMLWriter.h"
#include <stack>
#include <string>
#include <sstream>

struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> Sink;
    std::stack<std::string> Stack;

    // Data Sink Consctructor
    explicit SImplementation(std::shared_ptr<CDataSink> sink) 
        : Sink(std::move(sink)) {}

    // Writes an XML entity to the sink
    bool WriteEntity(const SXMLEntity &entity) {
        std::ostringstream output;

            // opening tags
            if(entity.DType == SXMLEntity::EType::StartElement){
                output << "<" << entity.DNameData;

                // Attributes
                for (const auto &attr : entity.DAttributes) {
                    output << " " << attr.first << "=\"";
                    for (char c : attr.second) {
                        if (c == '&') {
                            output << "&amp;";
                        }
                        else if (c == '<') {
                            output << "&lt;";
                        }
                        else if (c == '>') {
                            output << "&gt;";
                        }
                        else if (c == '\"') {
                            output << "&quot;";
                        }
                        else if (c == '\'') {
                            output << "&apos;";
                        }
                        else {
                            output << c;
                        }
                    }
                    output << "\"";
                }

                output << ">";
                Stack.push(entity.DNameData);
            }

            // Closing tags
            else if(entity.DType == SXMLEntity::EType::EndElement){
                output << "</" << entity.DNameData << ">";
                if (!Stack.empty() && Stack.top() == entity.DNameData) {
                    Stack.pop();
                }
            }

            // Data in tags
            else if(entity.DType == SXMLEntity::EType::CharData){ 
                for (char c : entity.DNameData) {   
                    if (c == '&') {
                        output << "&amp;";
                    }
                    else if (c == '<') {
                        output << "&lt;";
                    }
                    else if (c == '>') {
                        output << "&gt;";
                    }
                    else if (c == '\"') {
                        output << "&quot;";
                    }
                    else if (c == '\'') {
                        output << "&apos;";
                    }
                    else {
                        output << c;
                    }
                }
            }

            // Self-closing tags
            else if(entity.DType == SXMLEntity::EType::CompleteElement){
                output << "<" << entity.DNameData;

                for (const auto &attr : entity.DAttributes) {
                    output << " " << attr.first << "=\"";
                    for (char c : attr.second) {
                        if (c == '&') {
                            output << "&amp;";
                        }
                        else if (c == '<') {
                            output << "&lt;";
                        }
                        else if (c == '>') {
                            output << "&gt;";
                        }
                        else if (c == '\"') {
                            output << "&quot;";
                        }
                        else if (c == '\'') {
                            output << "&apos;";
                        }
                        else {
                            output << c;
                        }
                    }
                    output << "\"";
                }

                output << "/>";
        }

        std::string outputStr = output.str();
        std::vector<char> outputVec(outputStr.begin(), outputStr.end());

        return Sink->Write(outputVec) > 0;
    }

    bool Flush() {
        while (!Stack.empty()) {
            std::string endTag = "</" + Stack.top() + ">";
            std::vector<char> outputVec(endTag.begin(), endTag.end());
            if (!Sink->Write(outputVec)) {
                return false;
            }
            Stack.pop();
        }
        return true;
    }
};

////////////////////////////////////////////////////////////////////////

// Constructor: initializes the XML writer
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(std::move(sink))) {}

// Destructor
CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    return DImplementation->WriteEntity(entity);
}

bool CXMLWriter::Flush() {
    return DImplementation->Flush();
}

