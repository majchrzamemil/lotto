#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosio {

using std::string;
class [[eosio::contract("lotto")]] lotto : public contract {
 public:
  using contract::contract;

  [[eosio::action]]
  void create(const name& issuer, const asset& maximum_supply);

  [[eosio::action]]
  void issue(const name& to, const asset& quantity, const string& memo);
  [[eosio::action]]
  void gamble(const name& from, const asset&   quantity,       const string&  memo);
 private:
  struct [[eosio::table]] account {
    asset balance;
    uint64_t primary_key() const {
      return balance.symbol.code().raw();
    }
  };

  struct [[eosio::table]] currency_stats {
    asset supply;
    asset max_supply;
    name issuer;
    uint64_t primary_key() const {
      return supply.symbol.code().raw();
    }
  };

  typedef eosio::multi_index< "accounts"_n, account > accounts;
  typedef eosio::multi_index< "stat"_n, currency_stats > stats;

  void sub_balance(const name& owner, const asset& value);
  void add_balance(const name& owner, const asset& value, const name& ram_payer);
};
}

