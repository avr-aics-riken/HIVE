/*
Copyright (c) 2015, Light Transport Entertainment, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Light Transport Entertainment, Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <cassert>
#include <algorithm>

#include "tiny_vtkxml.h"

//#define DEBUG

namespace lte {
namespace tinyvtkxml{

namespace {

unsigned int SwapByteOrder(unsigned int x) {
  return ((0x000000FF & x) << 24) +
         ((0x0000FF00 & x) << 8) +
         ((0x00FF0000 & x) >> 8) +
         ((0xFF000000 & x) >> 24);
}

}  // namespace

Node::~Node() {
  for (std::vector<Node*>::iterator it = children_.begin();
      it != children_.end(); ++it) {
    delete *it;
  }
}

const std::string& Node::GetName() {
  return name_;
}

bool Node::HasAttribute(const std::string& key) {
  return attributes_.count(key);
}

const std::string& Node::GetAttribute(const std::string& key) {
  assert(attributes_.count(key));
  return attributes_[key];
}
int Node::GetAttributeInt(const std::string& key) {
  const std::string& value = GetAttribute(key);

  int result = 0;
  for (int i = 0; i < value.size(); ++i) {
    assert('0' <= value[i] && value[i] <= '9');

    result *= 10;
    result += (value[i] - '0');
  }

  return result;
}

Node* Node::GetChild(int index) {
  return children_[index];
}

int Node::CountChildren() {
  return children_.size();
}

Node* Node::FindChild(const std::string& name) {
  for (std::vector<Node*>::iterator it = children_.begin();
      it != children_.end(); ++it) {
    if ((*it)->GetName() == name) {
      return *it;
    }
  }
  
  assert(false && "child with given name not found");
  return NULL;
}

std::vector<Node*> Node::FindChildren(const std::string& name) {
  std::vector<Node*> result;

  for (std::vector<Node*>::iterator it = children_.begin();
      it != children_.end(); ++it) {
    if ((*it)->GetName() == name) {
      result.push_back(*it);
    }
  }

  return result;
}

unsigned long long Node::GetPosition() {
  return position_;
}

unsigned int Node::GetLength() {
  return length_;
}

void Node::SetName(const std::string& name) {
  name_ = name;
}

void Node::AppendAttribute(const std::string& key, const std::string& value) {
  attributes_[key] = value;
}

void Node::AppendChild(Node* child) {
  children_.push_back(child);
}

void Node::SetPosition(unsigned long long position) {
  position_ = position;
}

void Node::SetLength(unsigned int length) {
  length_ = length;
}

TinyVTKXML::TinyVTKXML(std::string filename)
    : filename_(filename)
    , parsed_(NULL)
    , file_(NULL)
    , position_(0)
    , is_big_endian_(false) {
  std::fill(next_, next_ + sizeof(next_) / sizeof(next_[0]), -1);
}

TinyVTKXML::~TinyVTKXML() {
  delete parsed_;
}

bool TinyVTKXML::Parse() {
  assert(parsed_ == NULL && "do not reuse the TinyVTKXML instance");
  if ((file_ = fopen(filename_.c_str(), "rb")) == NULL) {
    return true;
  }

  parsed_ = ParseNode();

  SetOffsetPositions(parsed_);

  fclose(file_);
  return false;
}

Node* TinyVTKXML::GetRoot() {
  assert(parsed_ != NULL);
  return parsed_;
}

char TinyVTKXML::Char(int index) {
  assert(index < sizeof(next_) / sizeof(next_[0]));

  if (next_[index] < 0) {
    for (int i = 0; i <= index; ++i) {
      if (next_[i] < 0) {
        next_[i] = fgetc(file_);
      }
    }
  }

  return next_[index];
}

void TinyVTKXML::Consume() {
  assert(next_[0] >= 0);
  for (int i = 1; i < sizeof(next_) / sizeof(next_[0]); ++i) {
    next_[i - 1] = next_[i];
  }
  next_[sizeof(next_) / sizeof(next_[0]) - 1] = -1;
  ++position_;
}

std::string TinyVTKXML::Identifier() {
  std::string result;

  while (('a' <= Char() && Char() <= 'z')
      || ('A' <= Char() && Char() <= 'z')
      || (Char() == '_')) {
    result += Char();
    Consume();
  }

  return result;
}

std::string TinyVTKXML::String() {
  std::string result;

  Assert('"');
  while (Char() != '"') {
    result += Char();
    Consume();
  }
  Assert('"');

  return result;
}

unsigned int TinyVTKXML::UInt32() {
  unsigned int result = 0;

  for (int i = 0; i < 4; ++i) {
    result <<= 8;
    result += static_cast<unsigned char>(Char());
    Consume();
  }

  if (!is_big_endian_) {
    result = SwapByteOrder(result);
  }

#ifdef DEBUG
  fprintf(stderr, "length = %x\n", result);
#endif

  return result;
}

void TinyVTKXML::Trim() {
  while (Char() == '\r' || Char() == '\n' || Char() == ' ' || Char() == '\t') {
    Consume();
  }
}

void TinyVTKXML::Assert(char c) {
  if (Char() != c) {
    fprintf(stderr, "Assertion failing at %c in %lluth byte (expected %c)\n", Char(), position_, c);
  }
  assert(Char() == c);
  Char();
  Consume();
}

void TinyVTKXML::Skip(unsigned int length) {
  assert(next_[0] == -1);
  fseek(file_, length, SEEK_CUR);
  position_ += length;
}

void TinyVTKXML::SkipAppendedData() {
  Assert('_');

  appended_offset_ = position_;

  while (Char() != '<') {
    const unsigned long long start_position = position_;
    const unsigned int length = UInt32();

    Skip(length);

    lengths_[start_position] = length;

    // This is dirty solution
    if (Char() == '\n') {
      Consume();
    }
  }
}

Node* TinyVTKXML::ParseNode() {
  Trim();

  Assert('<');

  Node* node = new Node();

  node->SetName(Identifier());

#ifdef DEBUG
  fprintf(stderr, "parsing node %s\n", node->GetName().c_str());
#endif

  Trim();

  while (Char() != '>' && Char() != '/') {
    const std::string key = Identifier();
    Assert('=');
    const std::string value = String();

    node->AppendAttribute(key, value);

    Trim();
  }

  if (Char() == '/') {
    Assert('/');
    Assert('>');

    Trim();
  } else if (Char() == '>') {
    Assert('>');

    Trim();

    if (node->GetName() == "VTKFile") {
      if (node->HasAttribute("byte_order") &&
          node->GetAttribute("byte_order") == "BigEndian") {
        is_big_endian_ = true;
      }

      if (node->HasAttribute("header_type") &&
          node->GetAttribute("header_type") != "UInt32") {
        assert(false && "unsupported VTKFile header_type");
      }
    }

    if (node->GetName() == "AppendedData") {
      if (!node->HasAttribute("encoding") || node->GetAttribute("encoding") != "raw") {
        assert(false && "unsupported AppendedData encoding");
      }

      SkipAppendedData();
    }

    while (!(Char(0) == '<' && Char(1) == '/')) {
      node->AppendChild(ParseNode());
      Trim();
    }
    Assert('<');
    Assert('/');
    const std::string closing = Identifier();
    assert(node->GetName() == closing && "Invalid closing tag");
    Assert('>');
  } else {
    assert(false && "/ or > expected");
  }

  Trim();

  return node;
}


void TinyVTKXML::SetOffsetPositions(Node* current) {
  assert(current != NULL);
  const int length = current->CountChildren();
  for (int i = 0; i < length; ++i) {
    SetOffsetPositions(current->GetChild(i));
  }

  if (current->GetName() == "DataArray" &&
      current->HasAttribute("format") &&
      current->GetAttribute("format") == "appended" &&
      current->HasAttribute("offset")) {
    current->SetPosition(appended_offset_ + current->GetAttributeInt("offset"));
    current->SetLength(lengths_[current->GetPosition()]);
  }

}

}  // namespace tinyvtkxml
}  // namespace lte

