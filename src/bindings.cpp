// src/bindings.cpp
#include <pybind11/pybind11.h>
#include "Tokenizer.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pytokenizer, m) {
    py::class_<Tokenizer>(m, "Tokenizer")
        .def(py::init<>())
        .def("load_merges", &Tokenizer::load_merges)
        .def("build_token_dict", &Tokenizer::build_token_dict)
        .def("encode_to_ids", &Tokenizer::encode_to_ids)
        .def("decode_from_ids", &Tokenizer::decode)
        .def("pad_or_truncate", &Tokenizer::pad_or_truncate)
        .def("batch_encode", &Tokenizer::batch_encode)
        .def("save_vocab", &Tokenizer::save_vocab)
        .def("load_vocab", &Tokenizer::load_vocab)
        .def("save_merges", &Tokenizer::save_merges);
}
