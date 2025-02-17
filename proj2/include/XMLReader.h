// #ifndef XMLREADER_H
// #define XMLREADER_H

// #include <memory>
// #include "XMLEntity.h"
// #include "DataSource.h"

// class CXMLReader{
//     private:
//         struct SImplementation;
//         std::unique_ptr<SImplementation> DImplementation;

//         std::shared_ptr< CDataSource > Source;
        
//     public:
//         CXMLReader(std::shared_ptr< CDataSource > src);
//         ~CXMLReader();
        
//         bool End() const;
//         bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
// };

// #endif

#ifndef XMLREADER_H
#define XMLREADER_H

#include <memory>
#include <vector>
#include <string>
#include "DataSource.h"
#include "DataSink.h"
#include "XMLEntity.h"

class CXMLReader {
private:
    struct SImplementation;
    std::unique_ptr<SImplementation> DImplementation;

public:
    // Constructor for Reading Only
    CXMLReader(std::shared_ptr<CDataSource> src);

    // Constructor for Reading & Writing
    CXMLReader(std::shared_ptr<CDataSource> src, std::shared_ptr<CDataSink> sink);

    ~CXMLReader();

    bool End() const;
    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);
    
    // ✅ Added WriteEntity to the public interface
    bool WriteEntity(const SXMLEntity &entity);
};

#endif

