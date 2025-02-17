#include "XMLReader.h"
#include <expat.h>
#include <sstream>
#include <iostream>

// Private struct implementation (PImpl pattern)
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> Source;
    XML_Parser Parser;
    bool EndOfFile = false;
    std::vector<SXMLEntity> Entities;
    
    SImplementation(std::shared_ptr<CDataSource> src) : Source(std::move(src)) {
        Parser = XML_ParserCreate(nullptr); // Create an Expat XML parser
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharacterDataHandler); // added this
    }

    ~SImplementation() {
        XML_ParserFree(Parser); // Cleanup Expat parser
    }

    // added this
    static void CharacterDataHandler(void *userData, const char *data, int len) {
        auto *impl = static_cast<SImplementation *>(userData);
        std::string text(data, len);
        
        // ignore any empty or whitespace-only text nodes
        if (text.find_first_not_of(" \t\n\r") != std::string::npos) {
            SXMLEntity entity;
            entity.DType = SXMLEntity::EType::CharData;
            entity.DNameData = text;
            impl->Entities.push_back(entity);
        }
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

    bool ReadNextEntity(SXMLEntity &entity) {
        if (Entities.empty()) {
            if (Source->End()) {
                EndOfFile = true;  // Set the EOF flag correctly
                return false;
            }
    
            std::vector<char> buffer(1024);
            size_t bytesRead = Source->Read(buffer, buffer.size());
    
            if (!XML_Parse(Parser, buffer.data(), bytesRead, Source->End())) {
                std::cerr << "XML Parsing Error: " << XML_ErrorString(XML_GetErrorCode(Parser)) << std::endl;
                return false; // Parsing error
            }
        }
    
        if (!Entities.empty()) {
            entity = Entities.front();
            Entities.erase(Entities.begin());
    
            // If this was the last entity, mark as EOF
            if (Entities.empty() && Source->End()) {
                EndOfFile = true;
            }
    
            return true;
        }
    
        return false;
    }    
};

// Constructor: initializes the XML reader
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(std::move(src))) {}

// Destructor
CXMLReader::~CXMLReader() = default;

// Returns true if the end of the file has been reached
bool CXMLReader::End() const {
    return DImplementation->EndOfFile;
}

// Reads an XML entity into the provided `entity` structure
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadNextEntity(entity);
}
