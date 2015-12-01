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
#ifndef TINY_VTKXML_H__
#define TINY_VTKXML_H__

#include <vector>
#include <map>
#include <cstdio>
#include <string>

namespace lte {
namespace tinyvtkxml{

class Node {
 public:
  ~Node();

  const std::string& GetName();
  bool HasAttribute(const std::string& key);
  const std::string& GetAttribute(const std::string& key);
  int GetAttributeInt(const std::string& key);
  Node* GetChild(int index);
  Node* FindChild(const std::string& name);
  std::vector<Node*> FindChildren(const std::string& name);
  int CountChildren();

  // Get the offset position of appended binary data (absolute value in the file)
  unsigned long long GetPosition();
  unsigned int GetLength();

  void SetName(const std::string& name);
  void AppendAttribute(const std::string& key, const std::string& value);
  void AppendChild(Node* child);
  void SetPosition(unsigned long long position);
  void SetLength(unsigned int length);

 private:
  std::string name_;
  std::map<std::string, std::string> attributes_;
  std::vector<Node*> children_;
  unsigned long long position_;
  unsigned int length_;
};

class TinyVTKXML {
 public:
  TinyVTKXML(std::string filename);
  ~TinyVTKXML();

  // Parses VTK XML file. Returns true if failed.
  bool Parse();

  Node* GetRoot();

 private:
  std::string Identifier();
  std::string String();
  char Char(int index = 0);
  void Consume();
  void Trim();
  void Assert(char c);
  unsigned int UInt32();
  void Skip(unsigned int length);
  void SkipAppendedData();
  void SetOffsetPositions(Node* current);

  Node* ParseNode();

  std::string filename_;
  Node* parsed_;
  FILE *file_;
  int next_[2];
  unsigned long long position_;
  unsigned long long appended_offset_;
  std::map<unsigned long long, unsigned int> lengths_;

  bool is_big_endian_;
};

}  // namespace tinyvtkxml
}  // namespace lte

#endif
