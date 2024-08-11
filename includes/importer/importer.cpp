#include "importer.h"

void parseFace(std::vector<std::string> face, 
               std::vector<glm::vec3> *vertexIndices,
               std::vector<glm::vec3> *textureIndices);

inline constexpr auto hash_djb2(const std::string_view sv){
    unsigned long hash = 5381;
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

std::vector<float> Importer::importOBJ(const char* file, const bool textured){
    std::vector<float> result;
    std::string lineContent;
    std::ifstream fileContent;
    const auto vertexHash = hash_djb2("v");
    const auto faceHash = hash_djb2("f");
    const auto textureHash = hash_djb2("vt");
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertexIndices;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> textureIndices;

    fileContent.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        fileContent.open(file);
        while (fileContent.peek() != EOF){
            std::getline(fileContent, lineContent);
            std::stringstream lineStream(lineContent);
            std::string token;
            std::vector<std::string> tokens;
            while (std::getline(lineStream, token, ' ')) {
                tokens.push_back(token);
            }
            switch(hash_djb2(tokens[0])){
                case vertexHash:
                    vertices.push_back(glm::vec3(std::stof(tokens[1]), 
                                                 std::stof(tokens[2]), 
                                                 std::stof(tokens[3])));
                    break;
                case faceHash:
                    tokens.erase(tokens.begin());
                    parseFace(tokens, &vertexIndices, textured ? &textureIndices : nullptr);

                    break;
                case textureHash:
                    if (textured){
                        textureCoordinates.push_back(glm::vec2(std::stof(tokens[1]),
                                                               std::stof(tokens[2])));
                    }
                    break;
            }
        }
        fileContent.close();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Error: import file reading failed: " << e.what() << std::endl;
    }
    for (int j = 0; j < vertexIndices.size(); j++){
        for (int i = 0; i < 3; i++){
            result.push_back(vertices[vertexIndices[j][i]].x);
            result.push_back(vertices[vertexIndices[j][i]].y);
            result.push_back(vertices[vertexIndices[j][i]].z);
            if (textured){
                result.push_back(textureCoordinates[textureIndices[j][i]].x);
                result.push_back(textureCoordinates[textureIndices[j][i]].y);
            }
        }
    }
    return result;
}

void parseFace(std::vector<std::string> face, 
               std::vector<glm::vec3> *vertexIndices,
               std::vector<glm::vec3> *textureIndices){
    std::vector<glm::vec3> result;
    std::vector<int> vertexIntIndices;
    std::vector<int> textureIntIndices;
    for (std::string token: face){
        std::stringstream tokenStream(token);
        std::string vertexIndex;
        std::getline(tokenStream, vertexIndex, '/');
        vertexIntIndices.push_back(std::stoi(vertexIndex));
        if (textureIndices){
            std::string textureIndex;
            std::getline(tokenStream, textureIndex, '/');
            textureIntIndices.push_back(std::stoi(textureIndex));
        }
    }
    unsigned short int offset = 0;
    while(++offset + 2 <= vertexIntIndices.size()){
        vertexIndices->push_back(glm::vec3(vertexIntIndices[0] - 1, 
                                           vertexIntIndices[offset] - 1, 
                                           vertexIntIndices[offset + 1] - 1));
        if (textureIndices){
            textureIndices->push_back(glm::vec3(textureIntIndices[0] - 1, 
                                                textureIntIndices[offset] - 1, 
                                                textureIntIndices[offset + 1] - 1));
        }
    }
}
