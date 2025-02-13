//オレオレエンコーダー＆デコーダー
//ファイルサイズ不変。サイズのみからファイル内のバイト交換パターンを作る
//メモリに乗らない大きなファイルはダメです

#include "ore2enc.h"

using namespace std;
int main(int ac, char *av[]){
 if(ac<2){cout<<"ore2enc FILENAME\n";return -1;}

 //ロード
 string data=binload(string(av[1]));

 //初期準備
 uint64_t filesize=get_file_size(std::string(av[1]));
 //Logistic写像と連分数の初期値
 double init=convertInt64ToNormalizedDouble(filesize);
 //Logistic写像の係数
 double a=3.6+init*0.4;
 int maxblock=20;//最大2^20まで
 std::vector<uint64_t> keisu=continued_fraction_expansion(init, maxblock);
 std::vector<uint64_t> powi=generate_powers_of_two(maxblock);
 std::vector<uint64_t> blocks_tmp=elementwise_product(keisu, powi);
 std::vector<uint64_t> blocks=filter_less_than(blocks_tmp, filesize);
 blocks=reverse_vector(blocks);
 uint64_t sum_blocks=sum_vector(blocks);

 //置換テーブルにLogistic写像の実数値をいれる
 std::vector<double> logistic=logistic_map(init, a, sum_blocks, 1000);
 std::vector<std::vector<double>> blockd=blocksplit(logistic, blocks);

 //実数値の順番で置換テーブルを作る
 std::vector<std::vector<int>> ranks=rank_vectors(blockd);

 std::string kekka;
 std::string o_filename;

 if(!ends_with_ore(string(av[1]))){
  //正方向の置換
  kekka=multi_reorder_by_ranks(data, ranks);
  o_filename=append_extension(av[1]);
  binsave(kekka,o_filename);
 }
  //逆方向の置換
 else{
  ranks=reverse_ranks(ranks);
  kekka=multi_reorder_by_ranks(data, ranks);
  o_filename=remove_suffix(string(av[1]));
  binsave(kekka,o_filename);
 }

 sync_file_timestamp(string(av[1]),o_filename);

 return 0;
}
