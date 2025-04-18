#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Tokenizer {
public:
    std::vector<std::pair<std::string, std::string>> merges;
    std::unordered_map<std::string, int> token_to_id;
    std::unordered_map<int, std::string> id_to_token;

    void load_merges(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open BPE merges file: " + filename);
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string a, b;
            if (iss >> a >> b) {
                merges.emplace_back(a, b);
            }
        }
    }

    void build_token_dict() {
        token_to_id.clear();
        id_to_token.clear();
        std::unordered_set<std::string> unique_tokens;
        int id = 0;

        for (char c = 0; c < 127; ++c) {
            std::string s(1, c);
            token_to_id[s] = id;
            id_to_token[id] = s;
            unique_tokens.insert(s);
            id++;
        }

        for (const auto& merge : merges) {
            std::string merged = merge.first + merge.second;
            if (unique_tokens.count(merged) == 0) {
                token_to_id[merged] = id;
                id_to_token[id] = merged;
                unique_tokens.insert(merged);
                id++;
            }
        }

        token_to_id["<pad>"] = id;
        id_to_token[id++] = "<pad>";
        token_to_id["<unk>"] = id;
        id_to_token[id++] = "<unk>";
        token_to_id["<mask>"] = id;
        id_to_token[id++] = "<mask>";
        token_to_id["<cls>"] = id;
        id_to_token[id++] = "<cls>";
        token_to_id["<sep>"] = id;
        id_to_token[id++] = "<sep>";
    }

    std::vector<std::string> encode(const std::string& word) {
        std::vector<std::string> tokens;
        for (char c : word) tokens.emplace_back(1, c);

        for (const auto& merge : merges) {
            for (size_t i = 0; i < tokens.size() - 1; ++i) {
                if (tokens[i] == merge.first && tokens[i + 1] == merge.second) {
                    tokens[i] = tokens[i] + tokens[i + 1];
                    tokens.erase(tokens.begin() + i + 1);
                    i = std::max<int>(0, i - 2);
                }
            }
        }

        return tokens;
    }

    std::vector<int> encode_to_ids(const std::string& text) {
        auto tokens = encode(text);
        std::vector<int> ids;
        for (const auto& t : tokens) {
            if (token_to_id.count(t)) ids.push_back(token_to_id[t]);
            else ids.push_back(token_to_id["<unk>"]);
        }
        return ids;
    }

    std::string decode(const std::vector<int>& ids) {
        std::string result;
        for (int id : ids) {
            if (id_to_token.count(id)) {
                const auto& tok = id_to_token[id];
                if (tok[0] != '<') result += tok;
            }
        }
        return result;
    }

    std::vector<int> pad_or_truncate(const std::vector<int>& ids, size_t max_len) {
        std::vector<int> out = ids;
        if (out.size() < max_len)
            out.resize(max_len, token_to_id["<pad>"]);
        else if (out.size() > max_len)
            out.resize(max_len);
        return out;
    }

    std::vector<std::vector<int>> batch_encode(const std::vector<std::string>& texts, size_t max_len) {
        std::vector<std::vector<int>> batch;
        for (const auto& t : texts)
            batch.push_back(pad_or_truncate(encode_to_ids(t), max_len));
        return batch;
    }

    void save_vocab(const std::string& filename) {
        json j;
        for (const auto& kv : token_to_id)
            j[kv.first] = kv.second;
        std::ofstream out(filename);
        out << j.dump(2);
    }

    void load_vocab(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            throw std::runtime_error("Cannot open vocab JSON file: " + filename);
        }
        json j;
        in >> j;
        token_to_id.clear();
        id_to_token.clear();
        for (auto it = j.begin(); it != j.end(); ++it) {
            token_to_id[it.key()] = it.value();
            id_to_token[it.value()] = it.key();
        }
    }

    void save_merges(const std::string& filename) {
        std::ofstream out(filename);
        for (const auto& m : merges)
            out << m.first << " " << m.second << "\n";
    }

    void auto_init(const std::string& vocab_path, const std::string& merges_path = "") {
        std::ifstream test(vocab_path);
        if (test.good()) {
            load_vocab(vocab_path);
        } else if (!merges_path.empty()) {
            load_merges(merges_path);
            build_token_dict();
        } else {
            throw std::runtime_error("Neither vocab.json nor merges.bpe could be loaded.");
        }
    }
};

#endif
