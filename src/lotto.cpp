#include "../include/lotto.hpp"

namespace eosio {

void lotto::create(const name& issuer, const asset& maximum_supply) {
  require_auth(get_self());
  auto sym = maximum_supply.symbol;
  check( sym.is_valid(), "invalid symbol name" );
  check( maximum_supply.is_valid(), "invalid supply");
  check( maximum_supply.amount > 0, "max-supply must be positive");

  stats statstable( get_self(), sym.code().raw() );

  statstable.emplace( get_self(), [&]( auto& s ) {
     s.supply.symbol = maximum_supply.symbol;
     s.max_supply    = maximum_supply;
     s.issuer        = issuer;
  });
}
}
