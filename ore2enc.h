#include <bits/stdc++.h>

//binload
/*
std::string binload(const std::string& filename) {
 std::ifstream file(filename, std::ios::binary);
 if (!file) {
  std::cerr << "Error: Cannot open file " << filename << std::endl;
  return "";
 }
 std::ostringstream buffer;
 buffer << file.rdbuf();
 return buffer.str();
}
*/

std::string binload(const std::string& filename, uint64_t bytes) {
 std::ifstream file(filename, std::ios::binary);
 if (!file) {
  std::cerr << "Error: Cannot open file " << filename << std::endl;
  return "";
 }

 // ファイルサイズを取得
 file.seekg(0, std::ios::end);
 uint64_t filesize = file.tellg();
 file.seekg(0, std::ios::beg);

 // 読み込むバイト数を決定
 uint64_t read_size = (bytes > filesize) ? filesize : bytes;

 // 指定されたバイト数だけ読み込む
 std::string buffer(read_size, '\0');
 file.read(&buffer[0], read_size);
 return buffer;
}


//ファイル名からファイルサイズを得て64bit整数で返す
uint64_t get_file_size(const std::string& filename) {
 std::ifstream file(filename, std::ios::binary | std::ios::ate);
 if (!file) {
  std::cerr << "Error: Cannot open file." << std::endl;
  return 0;
 }
 return static_cast<uint64_t>(file.tellg());
}

// 文字列をバイト単位で反転:整数から実数を作るときに使う
std::string reverse_bytes(const std::string& input){
 std::string reversed_str = input;
 std::reverse(reversed_str.begin(), reversed_str.end());
 return reversed_str;
}

//64ビット整数から[0,1]のdoubleを作る 初期値とaと連分数展開のもとへ
double convertInt64ToNormalizedDouble(uint64_t value) {
 // 64ビット整数を文字列に変換
 std::ostringstream oss;
 oss << value;
 std::string stringValue = reverse_bytes(oss.str());

 // 先頭に「0.」を追加
 std::string normalizedString = "0." + stringValue;

 // 文字列をdouble型に変換
 double normalizedDouble = std::stod(normalizedString);
 return normalizedDouble;
}

//実数値から連分数展開でブロックサイス配列のための整数列を得る
std::vector<uint64_t> continued_fraction_expansion(double in, int n) {
 std::vector<uint64_t> expansion;
 for (int i = 0; i < n; ++i) {
  if (in == 0.0){expansion.push_back(0);}
  else{
   in = 1.0 / in; // 逆数を取る
   uint64_t integer_part = static_cast<uint64_t>(in); // 逆数を整数化
   expansion.push_back(integer_part);
   in -= integer_part; // 小数部分のみを次の in にセット
  }
 }
 return expansion;
}

//2^iの配列を作る・これに乱数列を掛けたものがブロックサイズの列になる
std::vector<uint64_t> generate_powers_of_two(int n){
 std::vector<uint64_t> powers;
 uint64_t value = 2; // 2から開始
 for (int i = 0; i < n; ++i) {
  powers.push_back(value);
  value *= 2; // 次の累乗を計算
 }

 return powers;
}

//2^iの列に乱数の列をかけて返す関数
std::vector<uint64_t> elementwise_product(const std::vector<uint64_t>& vec1, const std::vector<uint64_t>& vec2) {
 size_t min_size = std::min(vec1.size(), vec2.size());
 std::vector<uint64_t> product(min_size);

 for (size_t i = 0; i < min_size; ++i) {
  product[i] = vec1[i] * vec2[i]; // 各要素の積を計算
 }
 return product;
}

//ファイルサイズより小さなブロック要素のみを取り出して部分配列にする
std::vector<uint64_t> filter_less_than(const std::vector<uint64_t>& in, uint64_t fsize) {
 std::vector<uint64_t> result;

 for (uint64_t val : in) {
  if (val >= fsize) break; // fsize 以上の要素が出たら終了（単調増加を前提）
  result.push_back(val);
 }
 return result;
}

//要素の反転：大きなブロック順にする
std::vector<uint64_t> reverse_vector(const std::vector<uint64_t>& in) {
 std::vector<uint64_t> reversed = in; // 元のベクターをコピー
 std::reverse(reversed.begin(), reversed.end()); // 反転
 return reversed;
}

//要素数の合計を出す関数：この長さのLogisticカオスを求める
uint64_t sum_vector(const std::vector<uint64_t>& vec) {
 return std::accumulate(vec.begin(), vec.end(), static_cast<uint64_t>(0));
}

//sum_of_powersのサイズのlogistic写像を作る
std::vector<double> logistic_map(double init, double a, uint64_t len, uint64_t discard) {
 std::vector<double> sequence;
 double x = init;
 for (int i = 0; i < discard; ++i) {
  x = a * x * (1.0 - x);
 }
 for (int i = 0; i < len; ++i) {
  x = a * x * (1.0 - x);
  sequence.push_back(x);
 }
 return sequence;
}



//生成されたlogistic実数列を分割する
std::vector<std::vector<double>> blocksplit(const std::vector<double>& input, std::vector<uint64_t>& split_sizes) {
 //std::vector<int> split_sizes = {3, 5, 7, 11, 13, 17, 19};
 std::vector<std::vector<double>> result;
 size_t index = 0;
 size_t n = input.size();
 for (uint64_t size : split_sizes) {
  if (index >= n) break;
  size_t end = std::min(index + size, n);
  result.emplace_back(input.begin() + index, input.begin() + end);
  index = end;
 }
 return result;
}

//vectorの順番を求める
std::vector<int> rank_vector(const std::vector<double>& input) {
 std::vector<int> ranks(input.size());
 std::vector<std::pair<double, int>> indexed_data;
 for (size_t i = 0; i < input.size(); ++i) {
  indexed_data.emplace_back(input[i], i);
 }
 std::sort(indexed_data.begin(), indexed_data.end());
 for (size_t rank = 0; rank < indexed_data.size(); ++rank) {
  ranks[indexed_data[rank].second] = static_cast<int>(rank);
 }
 return ranks;
}

//順番を全vectorに適用する
std::vector<std::vector<int>> rank_vectors(const std::vector<std::vector<double>>& input){
 std::vector<std::vector<int>> ranked_data;
 for (const auto& vec : input) {
  ranked_data.push_back(rank_vector(vec));
 }
 return ranked_data;
}

//----------------------------

//ファイル名の末尾が".ore"だったらtrue、そうでないならfalseを返す
bool ends_with_ore(const std::string& input) {
 const std::string suffix = ".ore";
 if (input.size() < suffix.size()) {
  return false;
 }
 return input.compare(input.size() - suffix.size(), suffix.size(), suffix) == 0;}


//----------------------------

//正方向の処理単位
std::string reorder_by_rank(const std::string& data, const std::vector<int>& rank) {
 size_t block_size = rank.size();
 std::string result = data; // 結果を格納する文字列
 size_t total_size = data.size();

 for (size_t i = 0; i + block_size <= total_size; i += block_size) {
  std::string temp_block = data.substr(i, block_size); // ブロックを取得
  for (size_t j = 0; j < block_size; ++j) {
   result[i + rank[j]] = temp_block[j]; // 指定された順番で入れ替え
  }
 }
 return result;
}

//正方向の全体的処理：vector<vector<int>> の各要素を rank として作用
std::string multi_reorder_by_ranks(const std::string& data, const std::vector<std::vector<int>>& ranks) {
 std::string result = data;
 for (const auto& rank : ranks) {
  result = reorder_by_rank(result, rank);
 }
 return result;
}

//ファイル名の末尾に".ore"をつける
std::string append_extension(const char* input) {
 return std::string(input) + ".ore";
}

//セーブする
void binsave(const std::string& data, const std::string& o_filename) {
 std::ofstream outfile(o_filename, std::ios::binary);
 if (!outfile) {
  std::cerr << "Error: Cannot open file " << o_filename << " for writing." << std::endl;
  return;
 }
 outfile.write(data.c_str(), data.size()); // 文字列をバイナリとして書き込む
 outfile.close();
}


//----------------------------

//順番を求める逆変換
std::vector<int> inv_rank(const std::vector<int>& rank) {
 std::vector<int> inverse(rank.size());
 for (size_t i = 0; i < rank.size(); ++i) {
  inverse[rank[i]] = static_cast<int>(i);
 }
 return inverse;
}

//vector<vector<int>> の各要素の順を反転し、inv_rank を適用する関数
std::vector<std::vector<int>> reverse_ranks(const std::vector<std::vector<int>>& ranks) {
 std::vector<std::vector<int>> transformed_ranks = ranks;
 std::reverse(transformed_ranks.begin(), transformed_ranks.end());
 for (auto& rank : transformed_ranks) {
  rank = inv_rank(rank); // 各要素に inv_rank を適用
 }
 return transformed_ranks;
}

//ファイル名の末尾から".ore"をとる
std::string remove_suffix(const std::string& input) {
 const std::string suffix = ".ore";
 if (input.size() >= suffix.size() && input.compare(input.size() - suffix.size(), suffix.size(), suffix) == 0) {
  return input.substr(0, input.size() - suffix.size()); // 末尾を削除
 }
 return input; // 変更なしで返す
}



//filename1のサイズを調べ、inputsize より大きければ、その超過部分をfilename2の末尾に追加
void appendfile(const std::string& filename1, const std::string& filename2, uint64_t inputsize) {
 // ファイル1を開く（バイナリモードで読み取り専用）
 std::ifstream file1(filename1, std::ios::binary);
 if (!file1) {
  std::cerr << "Error: Cannot open file " << filename1 << std::endl;
  return;
 }

 // ファイル1のサイズを取得
 file1.seekg(0, std::ios::end);
 uint64_t filesize1 = file1.tellg();
 if (filesize1 <= inputsize) {
  std::cerr << "Info: File size of " << filename1 << " is not larger than " << inputsize << ". No data appended.\n";
  return;
 }

 // 読み込むべきサイズを計算
 uint64_t append_size = filesize1 - inputsize;

 // ファイル1の読み込み位置をinputsizeの位置にセット
 file1.seekg(inputsize, std::ios::beg);

 // ファイル2を開く（バイナリモードで追記）
 std::ofstream file2(filename2, std::ios::binary | std::ios::app);
 if (!file2) {
  std::cerr << "Error: Cannot open file " << filename2 << std::endl;
  return;
 }

 // データをバッファに読み込んでファイル2へ書き込む
 std::string buffer(append_size, '\0');
 file1.read(&buffer[0], append_size);
 file2.write(buffer.data(), append_size);
}

//ディレクトリがなければ作る
void createDirectoryIfNotExists(const std::string& path) {
 std::filesystem::path dirPath(path);
 if (!std::filesystem::exists(dirPath)) {
  std::filesystem::create_directories(dirPath);
 }
}

//フルパスからディレクトリだけ取り出す
std::string getDirectoryPath(const std::string& fullPath) {
 std::filesystem::path filePath(fullPath);
 return filePath.parent_path().string();
}

//ファイルのタイムスタンプを合わせる
void sync_file_timestamp(const std::string& filename1, const std::string& filename2) {
 // filename1 の最終更新時刻を取得
 std::filesystem::file_time_type timestamp = std::filesystem::last_write_time(filename1);
 // filename2 の最終更新時刻を filename1 と同じに設定 
 std::filesystem::last_write_time(filename2, timestamp);
}

