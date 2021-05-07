#include "../include/lotto.hpp"

namespace eosio {

void lotto::create(const name& issuer, const asset& maximum_supply) {
  require_auth(get_self());
  auto sym = maximum_supply.symbol;
  check(sym.is_valid(), "invalid symbol name");
  check(maximum_supply.is_valid(), "invalid supply");
  check(maximum_supply.amount > 0, "max-supply must be positive");

  stats statstable(get_self(), sym.code().raw());

  statstable.emplace(get_self(), [&](auto & s) {
    s.supply.symbol = maximum_supply.symbol;
    s.max_supply    = maximum_supply;
    s.issuer        = issuer;
  });
}

//TODO:Maybe fuck issue, asset can be removed and made const its only constexpr uint64_t
void lotto::issue(const name& to, const asset& quantity, const string& memo) {
  auto sym = quantity.symbol;
  check(sym.is_valid(), "invalid symbol name");
  check(memo.size() <= 256, "memo has more than 256 bytes");

  stats statstable(get_self(), sym.code().raw());
  auto existing = statstable.find(sym.code().raw());
  check(existing != statstable.end(), "token with symbol does not exist, create token before issue");
  const auto& st = *existing;

  check(quantity.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "must issue positive quantity");

  check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
  check(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

  statstable.modify(st, same_payer, [&](auto & s) {
    s.supply += quantity;
  });

  add_balance(to, quantity, st.issuer);
}

//TODO:Change to have const quantity
void lotto::gamble(const name& from, const asset&   quantity, const string&  memo) {
  require_auth(from);
  check(is_account(to), "to account does not exist");
  auto sym = quantity.symbol.code();
  stats statstable(get_self(), sym.raw());
  const auto& st = statstable.get(sym.raw());

  require_recipient(from);

  check(quantity.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "must transfer positive quantity");
  check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
  check(memo.size() <= 256, "memo has more than 256 bytes");

  //TODO: add some table for gamble, and than gamble with it.
  //at the end sub and add appropraite balance.
  //sub_balance(from, quantity);

  //add_balance(to, quantity, payer );
}

void lotto::sub_balance(const name& owner, const asset& value) {
  accounts from_acnts(get_self(), owner.value);

  const auto& from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
  check(from.balance.amount >= value.amount, "overdrawn balance");

  from_acnts.modify(from, owner, [&](auto & a) {
    a.balance -= value;
  });
}

void lotto::add_balance(const name& owner, const asset& value, const name& ram_payer) {
  accounts to_acnts(get_self(), owner.value);
  auto to = to_acnts.find(value.symbol.code().raw());
  if (to == to_acnts.end()) {
    to_acnts.emplace(ram_payer, [&](auto & a) {
      a.balance = value;
    });
  } else {
    to_acnts.modify(to, same_payer, [&](auto & a) {
      a.balance += value;
    });
  }
}
}
