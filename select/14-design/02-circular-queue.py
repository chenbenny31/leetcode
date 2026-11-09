# LC 622 Design Circular Queue (Medium). Pattern: ring buffer, head + count.
# Fixed capacity k. en_queue / de_queue return False when full / empty; front / rear
# return -1 when empty; is_empty, is_full.


# Array + head index + count; the tail slot is (head + count) % k. T: O(1) per op,
# S: O(k).
class MyCircularQueue:
    def __init__(self, k):
        # circular storage; capacity = len(data)
        self.data = [0] * k
        # index of the front element
        self.head = 0
        # number of stored elements; tells full from empty when the tail wraps onto head
        self.count = 0

    def en_queue(self, value):
        if self.is_full():
            return False
        # first free slot after the last element
        tail = (self.head + self.count) % len(self.data)
        self.data[tail] = value
        self.count += 1
        return True

    def de_queue(self):
        if self.is_empty():
            return False
        self.head = (self.head + 1) % len(self.data)
        self.count -= 1
        return True

    def front(self):
        if self.is_empty():
            return -1
        return self.data[self.head]

    def rear(self):
        if self.is_empty():
            return -1
        last = (self.head + self.count - 1) % len(self.data)
        return self.data[last]

    def is_empty(self):
        return self.count == 0

    def is_full(self):
        return self.count == len(self.data)


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    q = MyCircularQueue(2)
    q.en_queue(1)
    q.en_queue(2)
    test("reject-full", q.en_queue(3), False)
    q.de_queue()
    q.en_queue(3)
    test("wrap-front", q.front(), 2)
    test("wrap-rear", q.rear(), 3)
    q.de_queue()
    q.de_queue()
    test("empty-front", q.front(), -1)
    test("empty-dequeue", q.de_queue(), False)


main()

# follow-up: no count field -> head + tail with one slot left unused: empty iff
#   head == tail, full iff (tail + 1) % cap == head
# follow-up: thread-safe blocking queue -> one threading.Lock with two Conditions
#   not_full / not_empty; wait in a while loop on the predicate, notify the other side
#   (queue.Queue(maxsize=k) is exactly this)
