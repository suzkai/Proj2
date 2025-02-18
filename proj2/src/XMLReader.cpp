#include "XMLReader.h"
#include "StringDataSink.h"
#include <expat.h>
#include <sstream>

// Private struct implementation (PImpl pattern)
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> Source;
    std::shared_ptr<CDataSink> Sink;  // Added Sink for writing
    XML_Parser Parser;
    bool EndOfFile = false;
    std::vector<SXMLEntity> Entities;

    SImplementation(std::shared_ptr<CDataSource> src, std::shared_ptr<CDataSink> sink = nullptr) 
        : Source(std::move(src)), Sink(std::move(sink)) {
        Parser = XML_ParserCreate(nullptr);
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharacterDataHandler);
    }

    ~SImplementation() {
        XML_ParserFree(Parser);
    }

    static void StartElementHandler(void *userData, const char *name, const char **atts) {
        auto *impl = static_cast<SImplementation *>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;

        for (int i = 0; atts[i]; i += 2) {
            entity.DAttributes.emplace_back(atts[i], atts[i + 1]);
        }

        impl->Entities.push_back(entity);
    }

    static void EndElementHandler(void *userData, const char *name) {
        auto *impl = static_cast<SImplementation *>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;

        impl->Entities.push_back(entity);
    }

    static void CharacterDataHandler(void *userData, const char *data, int len) {
        auto *impl = static_cast<SImplementation *>(userData);
        std::string text(data, len);

        if (text.find_first_not_of(" \t\n\r") != std::string::npos) {
            if (!impl->Entities.empty() && impl->Entities.back().DType == SXMLEntity::EType::CharData) {
                impl->Entities.back().DNameData += text;
            } else {
                SXMLEntity entity;
                entity.DType = SXMLEntity::EType::CharData;
                entity.DNameData = text;
                impl->Entities.push_back(entity);
            }
        }
    }

    bool ReadNextEntity(SXMLEntity &entity) {
        while (Entities.empty()) {
            if (Source->End()) {
                EndOfFile = true;
                return false;
            }

            std::vector<char> buffer;
            char ch;
            while (Source->Get(ch)) {  
                buffer.push_back(ch);
            }

            if (buffer.empty()) {
                EndOfFile = true;
                return false;
            }

            buffer.push_back('\0');

            if (!XML_Parse(Parser, buffer.data(), buffer.size() - 1, Source->End())) {
                return false;
            }
        }

        if (!Entities.empty()) {
            entity = Entities.front();
            Entities.erase(Entities.begin());
            return true;
        }

        return false;
    }

    bool WriteEntity(const SXMLEntity &entity) {
        if (!Sink) {
            return false;
        }
    
        std::ostringstream output;
        if (entity.DType == SXMLEntity::EType::StartElement) {
            output << "<" << entity.DNameData;
            for (const auto &attr : entity.DAttributes) {
                output << " " << attr.first << "=\"" << attr.second << "\"";
            }
            output << "/>";
        } 
        else if (entity.DType == SXMLEntity::EType::EndElement) {
            output << "</" << entity.DNameData << ">";
        } 
        else if (entity.DType == SXMLEntity::EType::CharData) {
            output << entity.DNameData;
        }
    
        std::string outputStr = output.str();
        std::vector<char> outputVec(outputStr.begin(), outputStr.end());
    
        return Sink->Write(outputVec) > 0;
    }
};

// Constructor for Reading Only
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(std::move(src))) {}

// Constructor for Reading & Writing
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src, std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>(std::move(src), std::move(sink))) {}

// Destructor
CXMLReader::~CXMLReader() {}

// Returns true if the end of the file has been reached
bool CXMLReader::End() const {
    return DImplementation->EndOfFile;
}

// Reads an XML entity into the provided `entity` structure
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadNextEntity(entity);
}

// Writes an XML entity to the sink
bool CXMLReader::WriteEntity(const SXMLEntity &entity) {
    return DImplementation->WriteEntity(entity);
}