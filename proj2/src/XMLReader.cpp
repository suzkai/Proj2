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
        Parser = XML_ParserCreate(nullptr);
        XML_SetUserData(Parser, this);
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(Parser, CharacterDataHandler);
        std::cout << "[DEBUG] XML Parser Created.\n";
    }

    ~SImplementation() {
        XML_ParserFree(Parser);
        std::cout << "[DEBUG] XML Parser Destroyed.\n";
    }

    static void StartElementHandler(void *userData, const char *name, const char **atts) {
        auto *impl = static_cast<SImplementation *>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;

        std::cout << "[DEBUG] Start Element: " << name << "\n";

        for (int i = 0; atts[i]; i += 2) {
            entity.DAttributes.emplace_back(atts[i], atts[i + 1]);
            std::cout << "  ├── Attribute: " << atts[i] << " = " << atts[i + 1] << "\n";
        }

        impl->Entities.push_back(entity);
    }

    static void EndElementHandler(void *userData, const char *name) {
        auto *impl = static_cast<SImplementation *>(userData);
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;

        std::cout << "[DEBUG] End Element: " << name << "\n";

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
            std::cout << "[DEBUG] Character Data: " << text << "\n";
        }
    }

    bool ReadNextEntity(SXMLEntity &entity) {
        while (Entities.empty()) {
            if (Source->End()) {
                EndOfFile = true;
                std::cout << "[DEBUG] End of File Reached.\n";
                return false;
            }
    
            std::vector<char> buffer;  // Instead of a fixed-size buffer, read everything
    
            // Read the entire XML data at once
            char ch;
            while (Source->Get(ch)) {  
                buffer.push_back(ch);
            }
    
            if (buffer.empty()) {
                std::cout << "[DEBUG] No More Data to Read.\n";
                EndOfFile = true;
                return false;
            }
    
            buffer.push_back('\0'); // Ensure null termination for Expat
    
            std::cout << "[DEBUG] XML Chunk Read (" << buffer.size() - 1 << " bytes):\n"
                      << std::string(buffer.begin(), buffer.end() - 1) << "\n";
    
            if (!XML_Parse(Parser, buffer.data(), buffer.size() - 1, Source->End())) {
                std::cerr << "[ERROR] XML Parsing Error: " 
                          << XML_ErrorString(XML_GetErrorCode(Parser)) << "\n";
                return false;
            }
        }
    
        if (!Entities.empty()) {
            entity = Entities.front();
            Entities.erase(Entities.begin());
    
            std::cout << "[DEBUG] Returning Entity: " << entity.DNameData 
                      << " (Type: " << (entity.DType == SXMLEntity::EType::StartElement ? "Start" : 
                                      (entity.DType == SXMLEntity::EType::EndElement ? "End" : "CharData")) << ")\n";
    
            return true;
        }
    
        return false;
    }    
};    

// Constructor
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)
    : DImplementation(std::make_unique<SImplementation>(std::move(src))) {
    std::cout << "[DEBUG] XMLReader Initialized.\n";
}

// Destructor
CXMLReader::~CXMLReader() {
    std::cout << "[DEBUG] XMLReader Destroyed.\n";
}

// Returns true if the end of the file has been reached
bool CXMLReader::End() const {
    return DImplementation->EndOfFile;
}

// Reads an XML entity into the provided `entity` structure
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    std::cout << "[DEBUG] ReadEntity Called.\n";
    return DImplementation->ReadNextEntity(entity);
}
