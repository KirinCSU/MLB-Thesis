#include "BookEntry.h"

MLB::BookEntry::BookEntry(OrderReferenceNumber a_orn,
                         bool a_buy,
                         int a_size,
                         double a_price)
    : orn(a_orn),
      buy(a_buy),
      size(a_size),
      price(a_price),
      prev(0),
      next(0)
{
}

MLB::BookEntry::~BookEntry()
{
}
