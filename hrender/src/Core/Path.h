/**
 * @file Path.h
 * プラットフォーム別ファイルパス操作ユーティリティ
 */
//
//  Path.h
//
//

#ifndef INCLUDE_PATH_H
#define INCLUDE_PATH_H

std::string getBinaryDir();
void changeFileDir(const std::string& filefullpath);
std::string convertFullpath(const std::string& path);

#endif // INCLUDE_PATH_H
