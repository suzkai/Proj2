# DSVReader (CDSVReader)
The `CDSVReader` class is responsible for reading delimiter-separated values (DSV) from a given data source. It supports handling quoted fields and parsing structured text from a data stream.

## 📌 Features
- Reads DSV files with customizable delimiters.
- Handles quoted fields correctly (`""` inside quoted text is unescaped as `"`).
- Detects and properly marks the end of the file.

## 🛠️ Implementation Details
The reader uses a **private struct (`SImplementation`)** that contains:
- `Source`: A shared pointer to `CDataSource` for reading input.
- `Delimiter`: The character used to separate values.
- `CurrentLine`: Stores the current line being processed.
- `EndOfFile`: Boolean flag to track whether the file has been completely read.

### 🔹 **Constructor**
```cpp
CDSVReader(std::shared_ptr<CDataSource> src, char delimiter);
