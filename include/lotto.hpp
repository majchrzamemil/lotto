#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosio {

using std::string;
class [[eosio::contract("lotto")]] lotto : public contract {
 public:
  using contract::contract;

  /**
   * Action to create tokken used for gambling.
   */
  [[eosio::action]]
  void create(const name& issuer, const asset& maximum_supply);

  /**
   * Issuing tokken to given user, everyone can issue tokkens, just for distribution of tokkens.
   */
  [[eosio::action]]
  void issue(const name& to, const asset& quantity, const string& memo);

  /**
   * Action for gambling in given tokken.
   */
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

  typedef eosio::multi_index< "gabmle"_n, account> gamble_pool;
  typedef eosio::multi_index< "accounts"_n, account > accounts;
  typedef eosio::multi_index< "stat"_n, currency_stats > stats;

  void sub_balance(const name& owner, const asset& value);
  void add_balance(const name& owner, const asset& value, const name& ram_payer);
};
}

