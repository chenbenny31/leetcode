# 13 · Stock Price Fluctuation (phone screen, Medium) — LC 2034
# records (timestamp, price) arrive out of order; a repeated timestamp corrects the
# earlier price. current() = price at the max timestamp; maximum() / minimum() = over
# the cur records
#
# clarify:
# queries before any update -> not called (given)
# correction of the latest timestamp -> current() returns the corrected price
# calls -> up to 1e5, so each op must be ~O(log n)

import heapq


# core idea: dict time -> price for corrections; a min-heap and a max-heap of
# (price, time); a top is stale if its time now has another price -> pop it lazily
# T: update O(log n), current O(1), maximum / minimum O(log n) amortized; S: O(updates)
class StockPrice:
    def __init__(self):
        # timestamp -> its latest price
        self.price_at = {}
        # (price, time), smallest price on top; may hold stale entries
        self.min_heap = []
        # (-price, time), largest price on top; may hold stale entries
        self.max_heap = []
        self.latest = 0

    def update(self, timestamp, price):
        self.price_at[timestamp] = price
        self.latest = max(self.latest, timestamp)
        # the old entry of a corrected timestamp stays in the heaps; it goes stale
        heapq.heappush(self.min_heap, (price, timestamp))
        heapq.heappush(self.max_heap, (-price, timestamp))

    def current(self):
        return self.price_at[self.latest]

    def maximum(self):
        neg_price, time = self.max_heap[0]
        while self.price_at[time] != -neg_price:
            heapq.heappop(self.max_heap)
            neg_price, time = self.max_heap[0]
        return -neg_price

    def minimum(self):
        price, time = self.min_heap[0]
        while self.price_at[time] != price:
            heapq.heappop(self.min_heap)
            price, time = self.min_heap[0]
        return price


def check(label, ok):
    print(label, "PASS" if ok else "FAIL")


def main():
    stock = StockPrice()
    stock.update(2, 200)
    stock.update(1, 100)
    check("current-latest", stock.current() == 200 and stock.maximum() == 200)
    stock.update(2, 50)
    check("correct-latest", stock.current() == 50 and stock.maximum() == 100)

    same = StockPrice()
    same.update(1, 7)
    same.update(2, 7)
    same.update(1, 9)
    check("duplicate-price-kept", same.minimum() == 7 and same.maximum() == 9)


main()

# follow-up:
# 1. why lazy pop -> a heap can't delete from the middle; skip stale tops instead
# 2. price range over a time window -> sorted times + segment tree over prices
# 3. many readers, one writer -> one lock around all three structures (change together)
