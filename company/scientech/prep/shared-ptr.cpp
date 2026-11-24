// SharedPtr<T> = shared ownership, copies share obj and +1 count
// obj is deleted when last owner goes

// thread-safe count not need -> use atomic if need
// no weak_ptr, custom deleter, make_shared, arrays
// empty SharedPtr -> ptr = nullptr with use_count=0
// self-assignment -> safe


