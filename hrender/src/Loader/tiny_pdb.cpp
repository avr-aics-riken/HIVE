#define _FILE_OFFSET_BITS 64

#include <cstdio>
#include <cassert>

#include "tiny_pdb.h"
#include "tiny_pdb_hashgrid.hxx"

namespace tinypdb {

/**
 * コンストラクタ
 * @param filename ファイルパス
 */
TinyPDB::TinyPDB(std::string filename)
    : parsed_(false)
    , filename_(filename)
    , fp_(NULL)
    , content_(NULL)
    , length_(-1)
    , pos_(0) {
}

/// デストラクタ
TinyPDB::~TinyPDB() {
}

/**
 * PDBデータのParse
 * @param isBondGenerationEnabled 接点作成フラグ
 * @retval true 成功
 * @retval false 失敗
 */
bool TinyPDB::Parse(bool isBondGenerationEnabled) {
  assert(!parsed_ && "do not reuse TinyPDB object");

  parsed_ = true;

  if ((fp_ = fopen(filename_.c_str(), "rb")) == NULL) {
    fprintf(stderr, "cannot open file %s\n", filename_.c_str());
    return true;
  }

  fseeko(fp_, 0, SEEK_END);
  length_ = ftello(fp_);
  fseeko(fp_, 0, SEEK_SET);

  content_ = new char[length_];
  if (content_ == NULL) {
    fprintf(stderr, "cannot allocate memory\n");
    return true;
  }

  fread(content_, 1, length_, fp_);

  fclose(fp_);
  fp_ = NULL;


  while (Char() != -1) {
    if (ParseLine()) {
      return true;
    }
  }

  delete[] content_;
  content_ = NULL;

  if (isBondGenerationEnabled) {
    GenerateBonds();
  }

  return false;
}

/**
 * PDBデータの線データParse
 * @retval true 成功
 * @retval false 失敗
 */
bool TinyPDB::ParseLine() {
  Trim();
  while (Char() == '\r' || Char() == '\n') {
    Consume();
  }

  if (Match("ATOM")) {
    atoms_.push_back(ParseAtom());
    return false;
  }

  SkipLine();

  return false;
}

/**
 * PDBデータのAtomデータ取得
 * @retval atom Atomデータ
 */
Atom TinyPDB::ParseAtom() {
  Atom atom;

  Trim();
  atom.SetSerialNumber(Integer());                     // 7 - 11

  FixedIdentifier(atom.MutableName(),             5, false);  // 13 - 16 // read constant width character for atom name.
  FixedIdentifier(atom.MutableIsomer(),           1);  // 17
  FixedIdentifier(atom.MutableResidueName(),      4);  // 18 - 20
  FixedIdentifier(atom.MutableChainName(),        1);  // 22
  Trim();
  atom.SetResidueNumber(Integer());                    // 23 - 26
  FixedIdentifier(atom.MutableResidueInsertion(), 1);  // 27

  Trim();
  atom.SetX(Double());
  Trim();
  atom.SetY(Double());
  Trim();
  atom.SetZ(Double());
  Trim();
  atom.SetOccupancy(Double());
  Trim();
  atom.SetTFactor(Double());

  Trim();
  if (('A' <= Char() && Char() <= 'Z') ||
      ('0' <= Char() && Char() <= '9')) {
    Identifier(atom.MutableElementSymbol());
  }

  SkipLine();

  return atom;
}

/**
 * PDBデータの線データParse
 * @param identifier 接点作成フラグ
 */
void TinyPDB::Identifier(std::string& identifier) {
  identifier.clear();
  while (('A' <= Char() && Char() <= 'Z') ||
         ('0' <= Char() && Char() <= '9')) {
    identifier += Char();
    Consume();
  }
}

/**
 * PDBデータの固定長Identifier取得
 * @param identifier 取得するIdentifierとなるstd::stringへの参照
 * @param length     取得するIdentifierの長さ
 * @param skipWhiteSpace Parse時空白Skip実施フラグ
 */
void TinyPDB::FixedIdentifier(std::string& identifier, int length, bool skipWhiteSpace) {
  identifier.clear();

  if (skipWhiteSpace) {

    while (' ' == Char() && length > 0) {
      Consume();
      --length;
    }

    // @todo { Read non-space characters. }
    while ((('A' <= Char() && Char() <= 'Z') ||
           ('0' <= Char() && Char() <= '9') || (Char() == '\'')) &&
           length > 0) {
      identifier += Char();
      Consume();
      --length;
    }

    while (' ' == Char() && length > 0) {
      Consume();
      --length;
    }

  } else {

    // may contain white space
    while (length > 0) {
      identifier += Char();
      Consume();
      --length;
    }

  }
}

/**
 * PDBデータParse時の整数取得
 * @retval result 取得した整数値
 */
int TinyPDB::Integer() {
  int sign = 1;

  if (Char() == '-') {
    sign = -1;
    Consume();
  }

  int result = 0;
  while ('0' <= Char() && Char() <= '9') {
    result *= 10;
    result += Char() - '0';
    Consume();
  }

  result *= sign;

  return result;
}

/**
 * PDBデータParse時の倍精度浮動小数点値取得
 * @retval result 取得した倍精度浮動小数点値
 */
double TinyPDB::Double() {
  double sign = 1.0;
  if (Char() == '-') {
    sign = -1;
    Consume();
  }

  double result = 0;
  while ('0' <= Char() && Char() <= '9') {
    result *= 10;
    result += Char() - '0';
    Consume();
  }

  if (Char() == '.') {
    Consume();

    double decimal = 0;
    double digits = 1;

    while ('0' <= Char() && Char() <= '9') {
      decimal *= 10;
      digits *= 10;
      decimal += Char() - '0';
      Consume();
    }

    result += decimal / digits;
  }

  result *= sign;

  return result;
}

/// PDBデータParse時、空白、タブスキップ
void TinyPDB::Trim() {
  while (Char() == ' ' || Char() == '\t') {
    Consume();
  }
}

/// PDBデータParse時に行単位でスキップ
void TinyPDB::SkipLine() {
  while (Char() != '\r' && Char() != '\n' && Char() != -1) {
    Consume();
  }
  while (Char() == '\r' || Char() == '\n') {
    Consume();
  }
}

/**
 * PDBデータParseの文字列一致チェック
 * @param keyword 検索する文字列
 * @retval true マッチした
 * @retval false マッチしなかった
 */
bool TinyPDB::Match(const char* keyword) {
  int i = 0;

  while (keyword[i] != 0) {
    if (Char(i) != keyword[i]) {
      return false;
    }
    ++i;
  }

  Consume(i);

  return true;
}

/**
 * PDBデータParse時、Readしたバッファの位置の文字を直接取得
 * @param index バッファ中のByteIndex
 * @return indexで指定された1Byte文字が返却される。indexの値が不正の場合-1を返却する。
 */
char TinyPDB::Char(int index) {
  if (pos_ + index < length_) {
    return content_[pos_ + index];
  } else {
    return -1;
  }
}

void TinyPDB::Consume(int amount) {
  if (pos_ < length_) {
    pos_ += amount;
  }
}

namespace {

/**
 * GetBondLengthLimit
 * @param a チェック対象Atom
 * @param b チェック対象Atom
 * @return a, b両方のAtomデータの3文字目がSだった場合は「1.9」、それ以外は1.7
 */
float GetBondLengthLimit(const Atom& a, const Atom& b) {
  if ((a.GetName().length() > 2 && b.GetName().length() > 2) && (a.GetName()[2] == 'S' || b.GetName()[2] == 'S')) {
    return 1.9;
  } else {
    return 1.7;
  }
}

/// HashGrid用クエリI/F
class HashGridQuery {
 public:
  HashGridQuery(Atom& current)
      : current_(current) {
  }

  Vec3f GetPosition() const {
    return current_.GetPosition();
  }

  void Process(const Atom& bonded) {
    if (&current_ == &bonded) {
      return;
    }

    if (current_.GetDistance(bonded) >= GetBondLengthLimit(current_, bonded)) {
      return;
    }

    if (current_.GetChainName() != bonded.GetChainName()) {
      return;
    }

    current_.AddBond(const_cast<Atom*>(&bonded));
  }

 private:
  Atom& current_;
};

}  // namespace

/// PDBデータの接点作成
void TinyPDB::GenerateBonds() {
  HashGrid hashGrid;

  hashGrid.Reserve(atoms_.size() / 16);
  hashGrid.Build(atoms_, 2.0);

  for (std::vector<Atom>::iterator it = atoms_.begin();
       it != atoms_.end(); ++it) {
    HashGridQuery query(*it);

    hashGrid.Process(atoms_, query);
  }
}

}  // namespace tinypdb
