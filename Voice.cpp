#include "Voice.h"

void Voice::decode_base64_and_write_to_file(const std::string& base64, const std::string& file_path) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;

    // Remove newline characters and other non-base64 characters
    std::string base64_clean;
    std::copy_if(base64.begin(), base64.end(), std::back_inserter(base64_clean),
                 [](char c) { return (c != '\n' && c != '\r'); });

    // Decode the base64 string
    std::string decoded;
    try {
        std::copy(It(base64_clean.begin()), It(base64_clean.end()), std::back_inserter(decoded));
    } catch (...) {
        std::cerr << "Error decoding base64 string." << std::endl;
        return;
    }

    // Write the decoded data to a file
    std::ofstream file(file_path, std::ios::out | std::ios::binary);
    if (!file.write(decoded.data(), decoded.size())) {
        std::cerr << "Error writing to file." << std::endl;
    }
}
