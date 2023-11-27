#include "Voice.h"
#include <iostream>
#include <string>

// curl -s -X POST \
//   -H "Content-Type: application/json" \
//   -d '{"version": "4d50797290df275329f202e48c76360b3f22b08d28c196cbc54600319435f8d2", "input": {"audio": "blob:https://hop.cheap/d4979630-2e11-4b5e-94c0-6ff3c02c39ba"}}' \
//   -H "Authorization: Token r8_X1ErZq5FO5J4XZc00HphUvYTWLr3zdP1utzio" \
//   "https://api.replicate.com/v1/predictions"


// receive the blob object sent from the webpage
// and convert it to a mp3 file using FFMPEG
void Voice::convertBlob(std::string blobPath) {
    std::cout << blobPath << std::endl;
}