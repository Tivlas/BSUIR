using System;

namespace Task
{
    public class MyCustomCollection<T> : ICustomCollection<T> where T : class
    {
        private CollectionHelper.Node<T>? _head = null;
        private CollectionHelper.Node<T>? _tail = null;
        private CollectionHelper.Node<T>? _current = null;

        public int Count { get; private set; } = 0;

        public MyCustomCollection()
        {
        }

        public T? this[int index]
        {
            get
            {
                var node = _head;
                do
                {
                    if (node == null)
                    {
                        throw new IndexOutOfRangeException();
                    }
                    node = node.Next;
                } while (index-- > 0);
                return node!.RealObject;
            }
            set
            {
                var node = _head;
                do
                {
                    if (node == null)
                    {
                        throw new IndexOutOfRangeException();
                    }
                    node = node.Next;
                } while (index-- > 0);
                node!.RealObject = value;
            }
        }

        public void Reset()
        {
            _current = _head;
        }

        public void Next()
        {
            _current = _current?.Next;
        }

        public T? Current()
        {
            return _current?.RealObject;
        }

        public void Add(T item)
        {
            var newNode = new CollectionHelper.Node<T>(item);
            if (_head == null)
            {
                _head = _tail = _current = newNode;
            }
            else
            {
                _tail!.Next = newNode;
                newNode.Prev = _tail;
                _tail = _tail.Next;
            }
            Count++;
        }

        private void RemoveNode(CollectionHelper.Node<T> node)
        {
            if (node == _head)
            {
                _head = node.Next;
                if (_head != null)
                {
                    _head.Prev = null;
                }
                else
                {
                    _tail = null;
                }
            }
            else if (node == _tail)
            {
                _tail = node.Prev;
                _tail!.Next = null;
            }
            else
            {
                node.Prev!.Next = node.Next;
                node.Next!.Prev = node.Prev;
            }
            Count--;
        }

        public void Remove(T item)
        {
            var curNode = _head;
            while (curNode != null)
            {
                if (item.Equals(curNode.RealObject))
                {
                    RemoveNode(curNode);
                    return;
                }
                curNode = curNode.Next;
            }
        }

        public T? RemoveCurrent()
        {
            if (_current == null)
            {
                throw new InvalidOperationException("Current was null");
            }
            var currentObjectCopy = _current.RealObject;
            var curNode = _current;
            _current = _current?.Next;
            RemoveNode(curNode);
            return currentObjectCopy;
        }
    }
}

namespace CollectionHelper
{
    class Node<T>
    {
        public T? RealObject { get; set; }
        public Node<T>? Next { get; set; }
        public Node<T>? Prev { get; set; }

        public Node(T? value)
        {
            RealObject = value;
        }
    }
}

