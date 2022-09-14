namespace Task;

public interface ICustomCollection<T>
{
    T this[int index] { get; set; }
    void Reset();
    bool MoveNext();
    T Current();
    int Count { get; }
    void Add(T item);
    void Remove(T item);
    T RemoveCurrent();
}
