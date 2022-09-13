namespace Task;

public interface ICustomCollection<T>
{
    T this[int index] { get; set; }
    void Reset();
    bool Next();
    T Current();
    int Count { get; }
    void Add(T item);
    void Remove(T item);
    T RemoveCurrent();
}
