# DSVWriter (CDSVWriter)
The `CDSVWriter` class is responsible for writing delimiter-separated values (DSV) to a data sink. It ensures that special characters (quotes, delimiters, newlines) are handled correctly.

## 📌 Features
- Writes DSV rows with customizable delimiters.
- Handles **quoted values** properly.
- Supports writing to a `CDataSink`.

## 🛠️ Implementation Details
The writer uses a **private struct (`SImplementation`)** that contains:
- `Sink`: A shared pointer to `CDataSink` for writing output.
- `Delimiter`: The character used to separate values.
- `QuoteAll`: Boolean flag that forces all values to be quoted.

### 🔹 **Constructor**
```cpp
CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall);
