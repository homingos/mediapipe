// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.
//
// This program takes one input file and encodes its contents as a C++ string,
// which can be included in a C++ source file. It is similar to filewrapper
// (and borrows some of its code), but simpler.

#include <fstream>
#include <iostream>
#include <memory>

#include "absl/strings/escaping.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: encode_as_c_string input_file\n";
    return 1;
  }
  const std::string input_name = argv[1];
  std::ifstream input_file(input_name,
                           std::ios_base::in | std::ios_base::binary);
  if (!input_file.is_open()) {
    std::cerr << "cannot open '" << input_name << "'\n";
    return 2;
  }
  constexpr int kBufSize = 4096;
  std::unique_ptr<char[]> buf(new char[kBufSize]);
  std::cout << "\"";
  int line_len = 1;
  while (1) {
    input_file.read(buf.get(), kBufSize);
    int count = input_file.gcount();
    if (count == 0) break;
    for (int i = 0; i < count; ++i) {
      std::string out = absl::CEscape(absl::string_view(&buf[i], 1));
      if (line_len + out.size() > 79) {
        std::cout << "\"\n\"";
        line_len = 1;
      }
      std::cout << out;
      line_len += out.size();
    }
  }
  input_file.close();
  if (!input_file.eof()) {
    std::cerr << "error reading '" << input_name << "'\n";
    return 2;
  }
  std::cout << "\"\n";
  return 0;
}
