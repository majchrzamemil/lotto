#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

namespace eosio {

  using std::string;
  class [[eosio::contract("lotto")]] lotto : public contract {
    public:
      using contract::contract;

      [[eosio::action]] void create(const name& issuer, const asset& maximum_supply);

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

  };
}

